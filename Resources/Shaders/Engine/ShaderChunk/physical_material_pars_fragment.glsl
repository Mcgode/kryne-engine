
uniform float roughness;

#ifdef USE_ROUGHNESS_MAP

uniform sampler2D roughnessMap;

#endif


uniform float metalness;

#ifdef USE_METALNESS_MAP

uniform sampler2D metalnessMap;

#endif
