#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D textureSampler;

uniform vec2 u_dimensions;
uniform vec2 u_border;

float map(float value, float originalMin, float originalMax, float newMin, float newMax)
{
    return (value - originalMin) / (originalMax - originalMin) * (newMax - newMin) + newMin;
}

float processAxis(float coord, float textureBorder, float windowBorder)
{
    if (coord < windowBorder)
        return map(coord, 0, windowBorder, 0, textureBorder);
    if (coord < 1 - windowBorder) 
        return map(coord,  windowBorder, 1 - windowBorder, textureBorder, 1 - textureBorder);
    return map(coord, 1 - windowBorder, 1, 1 - textureBorder, 1);
}

void main()
{
	vec2 newUV = vec2(
        processAxis(texCoord.x, u_border.x, u_dimensions.x),
        processAxis(texCoord.y, u_border.y, u_dimensions.y)
    );

	vec4 texColor = texture2D(textureSampler, newUV);
	if(texColor.a < 0.1)
        discard;

	FragColor = texColor;
}