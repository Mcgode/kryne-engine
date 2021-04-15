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
    }

    if (this->currentLOD < LOD_MIN)
    {
        this->currentLOD = LOD_MAX;
        PMREMGenerator::initializeProcess(map);
    }


#if KRYNE_ENGINE_USE_DEDICATED_IRRADIANCE_CONVOLUTION == 1

    auto &material = this->currentLOD == LOD_MIN ? this->irradianceConvolutionShader : this->preFilteringShader;

#else

    auto &material = this->preFilteringShader;

#endif

    material->setUniform("envMap", **map);

#if KRYNE_ENGINE_USE_DEDICATED_IRRADIANCE_CONVOLUTION == 1

    if (this->currentLOD > LOD_MIN)
    {
        float roughness = 1.f - (float) (this->currentLOD - LOD_MIN) / (LOD_MAX - LOD_MIN);
        material->setUniform("roughness", roughness);
    }

#else

    float roughness = 1.f - (float) (this->currentLOD - LOD_MIN) / (LOD_MAX - LOD_MIN);
    material->setUniform("roughness", roughness);

#endif

    this->framebuffer->setSize(ivec2(pow(2, this->currentLOD)));
    this->framebuffer->setAsRenderTarget();
    renderer->renderCubeTexture(this->framebuffer.get(), material.get(),
                                map->iblEnvMap.get(), LOD_MAX - this->currentLOD);

    this->currentLOD--;


    if (this->currentLOD < LOD_MIN)
    {
        map->iblReady = true;
        scoped_lock<mutex> l(this->dequeMutex);
        this->mapsToProcess.pop_front();
    }
}


void PMREMGenerator::initializeProcess(const shared_ptr<EnvironmentMap> &map)
{
    map->environmentMap->setFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    map->environmentMap->generateMipMaps();

    map->iblEnvMap = make_shared<CubeTexture>(
            ivec2(pow(2, LOD_MAX)),
            Texture::Formats::RGB,
            Texture::InternalSizes::HalfFloat,
            Texture::DataTypes::Float
    );
    map->iblEnvMap->setWrap(GL_CLAMP_TO_EDGE);
    map->iblEnvMap->setFiltering(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    map->iblEnvMap->generateMipMaps();

#if KRYNE_ENGINE_USE_DEDICATED_IRRADIANCE_CONVOLUTION == 1

    if (this->irradianceConvolutionShader == nullptr)
    {
        auto shader = make_unique<Shader>("Engine/IrradianceConvolution");
        this->irradianceConvolutionShader = make_unique<ShaderMaterial>(move(shader));

        this->irradianceConvolutionShader->setSide(BackSide);
        this->irradianceConvolutionShader->setDepthTest(false);
        this->irradianceConvolutionShader->setWriteDepth(false);
    }

#endif

    if (this->preFilteringShader == nullptr)
    {
        auto shader = make_unique<Shader>("Engine/EnvMapPreFiltering");
        this->preFilteringShader = make_unique<ShaderMaterial>(move(shader));

        this->preFilteringShader->setSide(BackSide);
        this->preFilteringShader->setDepthTest(false);
        this->preFilteringShader->setWriteDepth(false);
    }
}
