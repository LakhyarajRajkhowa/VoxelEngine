#version 330 core 

in vec2 fragmentUV;
flat in int isLight;
in vec4 FragPosViewSpace; // View-space position from vertex shader

out vec4 FragColor;

uniform sampler2D textures;
uniform vec3 ambientLightColor; // Color of ambient light
uniform float ambientStrength;  // Intensity of ambient light

// Fog uniforms
uniform vec3 fogColor; // Color of the fog
uniform float fogStart; // Distance where fog starts
uniform float fogEnd;   // Distance where fog fully covers

void main() {
    vec4 textureColor = texture(textures, fragmentUV);

    // Apply ambient lighting if the block is a light source
    if (isLight != 0) {	
        vec3 ambient = ambientStrength * ambientLightColor;
        vec3 finalColor = ambient * textureColor.rgb;
        FragColor = vec4(finalColor, textureColor.a);
    } else {
        FragColor = textureColor;
    }

    // === FOG CALCULATION ===
    float distance = abs(FragPosViewSpace.z); // Distance from camera in view space
    float fogFactor = clamp((fogEnd - distance) / (fogEnd - fogStart), 0.0, 1.0);
    
    // Blend fog with final color
    FragColor = mix(vec4(fogColor, 1.0), FragColor, fogFactor);
}
