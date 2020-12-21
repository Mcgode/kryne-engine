vec4 worldPosition = modelMatrix * vec4(transformed, 1);

geometry.position = worldPosition.xyz / worldPosition.w;
geometry.normal = normalize(normalMatrix * normal);
geometry.viewDir = normalize(cameraPosition - geometry.position);
