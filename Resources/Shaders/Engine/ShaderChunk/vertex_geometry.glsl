vec4 worldPosition = modelMatrix * vec4(transformed, 1);

geometry.position = worldPosition.xyz / worldPosition.w;
geometry.normal = normal;
geometry.viewDir = normalize(cameraPosition);
