/**
 * @file
 * @author Max Godefroy
 * @date 07/04/2021.
 */

#include "kryne-engine/Rendering/Additional/PMREMGenerator.hpp"


PMREMGenerator::PMREMGenerator(GraphicContext *context)
{
    this->framebuffer = context->makeFramebuffer(ivec2(32));
}


bool PMREMGenerator::isProcessing(const shared_ptr<EnvironmentMap> &map)
{
    scoped_lock<mutex> l(this->dequeMutex);
    return find(this->mapsToProcess.begin(), this->mapsToProcess.end(), map) != this->mapsToProcess.end();
}


void PMREMGenerator::processMap(const shared_ptr<EnvironmentMap> &map)
{
    if (!this->isProcessing(map) && (**map)->getId() != 0)
    {
        scoped_lock<mutex> l(this->dequeMutex);
        this->mapsToProcess.push_back(map);
    }
}


void PMREMGenerator::runProcessing(LoopRenderer *renderer)
{
    assertIsMainThread();

    shared_ptr<EnvironmentMap> map;
    {
        scoped_lock<mutex> l(this->dequeMutex);

        if (this->mapsToProcess.empty())
            return;

        map = this->mapsToProcess.front();
        this->mapsToProcess.pop_front();
    }

    auto irradianceCubeTexture = make_shared<CubeTexture>(
            ivec2(32),
            Texture::Formats::RGB,
            Texture::InternalSizes::HalfFloat,
            Texture::DataTypes::Float
    );

    irradianceCubeTexture->setWrap(GL_CLAMP_TO_EDGE);
    irradianceCubeTexture->setFiltering(GL_LINEAR, GL_LINEAR);

    auto irradianceShader = make_unique<Shader>("Engine/IrradianceConvolution");
    auto irradianceMaterial = make_unique<ShaderMaterial>(move(irradianceShader));

    irradianceMaterial->setUniform("envMap", **map);
    irradianceMaterial->setSide(BackSide);
    irradianceMaterial->setDepthTest(false);
    irradianceMaterial->setWriteDepth(false);

    this->framebuffer->setAsRenderTarget();
    renderer->renderCubeTexture(this->framebuffer.get(), irradianceMaterial.get(), irradianceCubeTexture.get());

    map->setIBL(irradianceCubeTexture);
}
