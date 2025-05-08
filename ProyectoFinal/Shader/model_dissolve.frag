#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1; // Textura del modelo
uniform sampler2D noiseMap;         // Textura de ruido
uniform float dissolveAmount;       // Valor de disolución (0.0 a 1.0)

void main()
{
    float noiseVal = texture(noiseMap, TexCoords).r;

    // Si el valor de ruido es menor que el umbral, descartamos el fragmento (efecto de "desintegración")
    if (noiseVal < dissolveAmount)
        discard;

    vec4 texColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(texColor.rgb, 1.0); // Puedes agregar iluminación si lo deseas
}
