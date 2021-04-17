/**
 * @file
 * @author Max Godefroy
 * @date 14/03/2021.
 */

#include "kryne-engine/Rendering/LoopRenderer.h"


LoopRenderer::LoopRenderer(GraphicContext *context,
                           unique_ptr<Framebuffer> screenFramebuffer,
                           unique_ptr<Framebuffer> readFramebuffer,
                           unique_ptr<Framebuffer> writeFramebuffer,
                           const ivec2 &size) :
   screenFramebuffer(move(screenFramebuffer)),
   readFramebuffer(move(readFramebuffer)),
   writeFramebuffer(move(writeFramebuffer)),
   rendererSize(size),
   context(context)
{
    auto geometry = make_shared<BoxBufferGeometry>();
    geometry->removeAttribute("normal");
    geometry->removeAttribute("uv");
    geometry->removeAttribute("tangent");
    this->cubeGeometry = geometry;

    this->pmremGenerator = make_unique<PMREMGenerator>(this->context);
}


LoopRenderer::FrustumCullingData::FrustumCullingData(Camera *camera) :
    frustum(camera->getProjectionMatrix() * camera->getViewMatrix())
{}


void LoopRenderer::prepareFrame()
{
    {
        scoped_lock<mutex> l(this->meshesMutex);
        this->meshesForFrame.clear();
    }

    {
        scoped_lock<mutex> l(this->frustumCullingMutex);
        this->frustumCulled.clear();

        FrustumCullingData mainCamFCD(mainCamera);
        this->frustumCulled.emplace(mainCamera, mainCamFCD);
    }

    this->framePostProcessPasses.clear();
    for (const auto &pass : this->postProcessPasses)
    {
        if (pass->isEnabled())
            this->framePostProcessPasses.push_back(pass.get());
    }
}


void LoopRenderer::registerMesh(RenderMesh *mesh)
{
    {
        scoped_lock<mutex> l(this->meshesMutex);
        this->meshesForFrame.push_back(mesh);
    }

    this->computeFrustumCulling(mesh);
}


void LoopRenderer::computeFrustumCulling(RenderMesh *mesh)
{
    if (!mesh->isFrustumCull())
        return;

    for (auto &pair : this->frustumCulled)
    {
        bool culled = !pair.second.frustum.sphereIntersects(mesh->getBoundingSphere());
        {
            scoped_lock<mutex> l(this->frustumCullingMutex);
            pair.second.culledMeshes.emplace(mesh, culled);
        }
    }
}


void LoopRenderer::addPass(unique_ptr<PostProcessPass> pass)
{
    auto it = this->postProcessPasses.begin();

    while (this->postProcessPasses.end() != it)
    {
        if ((*it)->getPriority() >= pass->getPriority())
            break;
    }

    this->postProcessPasses.insert(it, move(pass));
}


bool LoopRenderer::addPassAfter(unique_ptr<PostProcessPass> pass, const string &name)
{
    auto it = this->postProcessPasses.begin();

    for (; it != this->postProcessPasses.end(); it++)
    {
        if ((*it)->getName() == name)
            break;
    }

    if (it == this->postProcessPasses.end())
        return false;

    if ((*it)->getPriority() > pass->getPriority())
        return false;

    it++;
    this->postProcessPasses.insert(it, move(pass));
    return true;
}


bool LoopRenderer::addPassBefore(unique_ptr<PostProcessPass> pass, const string &name)
{
    auto it = this->postProcessPasses.begin();
    auto before = it;

    for (; it != this->postProcessPasses.end(); it++)
    {
        if ((*it)->getName() == name)
            break;
        before = it;
    }

    if (it == this->postProcessPasses.end())
        return false;

    if (it != this->postProcessPasses.begin() && (*before)->getPriority() > pass->getPriority())
        return false;

    this->postProcessPasses.insert(it, move(pass));
    return true;
}


unique_ptr<PostProcessPass> LoopRenderer::removePass(const string &name)
{
    auto it = this->postProcessPasses.begin();
    for (; it != this->postProcessPasses.end(); ++it)
    {
        if ((*it)->getName() == name)
            break;
    }

    if (it == this->postProcessPasses.end())
        return unique_ptr<PostProcessPass>();

    auto p = move(*it);
    this->postProcessPasses.erase(it);
    return p;
}


void LoopRenderer::updateRendererSize(const ivec2 &size)
{
    this->rendererSize = size;
    this->writeFramebuffer->setSize(size);
    this->readFramebuffer->setSize(size);
}


void LoopRenderer::addProcess(unique_ptr<RenderingProcess> process)
{
    this->processes.insert(this->processes.end(), move(process));
}


bool LoopRenderer::addProcessAfter(unique_ptr<RenderingProcess> process, const string &name)
{
    auto it = this->processes.begin();

    for (; it != this->processes.end(); it++)
    {
        if ((*it)->getName() == name)
            break;
    }

    if (it == this->processes.end())
        return false;

    it++;
    this->processes.insert(it, move(process));
    return true;
}


bool LoopRenderer::addProcessBefore(unique_ptr<RenderingProcess> process, const string &name)
{
    auto it = this->processes.begin();
    auto before = it;

    for (; it != this->processes.end(); it++)
    {
        if ((*it)->getName() == name)
            break;
        before = it;
    }

    if (it == this->processes.end())
        return false;

    this->processes.insert(it, move(process));
    return true;
}
