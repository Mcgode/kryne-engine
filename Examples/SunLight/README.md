# SunLight project

The objective with this project is to experiment open scene rendering, with external sunlight.

## Status

This project is still a work in progress, but I'm pausing it while finishing up the shader chunk system.

Next objective will probably be HDR + tone mapping 

## Todo list

### Mandatory
- [X] Basic Phong lighting and scene setup
- [X] Skybox
- [X] Shadow mapping
- [X] Cascaded shadow mapping
- [ ] Shadow map soft edges
- [ ] Soft shadow mapping
- [ ] HDR (high dynamic range) and tone mapping
- [ ] Bloom
- [ ] Lens flare
- [ ] SAO (scalable ambient obscurance) implementation
- [ ] PBR (physics based rendering) implementation

### Optional / Maybe somewhere down the line
- [ ] Transparency support for shadow mapping
- [ ] Screen-space reflections in the stained glass


## Screenshots

![Screenshot](../../Screenshots/SunLight/BasicLighting.png?raw=true "Basic phong lighting and scene setup")

![Screenshot](../../Screenshots/SunLight/Skybox.png?raw=true "Skybox added to the scene")

![Screenshot](../../Screenshots/SunLight/ShadowMap.png?raw=true "Shadow maps implemented and added to the scene")

![Screenshot](../../Screenshots/SunLight/CascadedShadowMap.png?raw=true "Cascaded shadow maps implemented and added to the scene")
