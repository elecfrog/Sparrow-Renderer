#version 450

precision highp float;

uniform float blur_sample_scale;
uniform float blurStrength;
uniform sampler2D samplerColor;

uniform bool blurdirection;
uniform mat3 blur_kernel;

out vec4 outFragColor;
in vec2 TexCoords;

const int LOOPS = 3;

float k_weights[9] = float[](
    blur_kernel[0][0], blur_kernel[0][1], blur_kernel[0][2], 
    blur_kernel[1][0], blur_kernel[1][1], blur_kernel[1][2], 
    blur_kernel[2][0], blur_kernel[2][1], blur_kernel[2][2]
);


void main() 
{
    vec3 color = texture(samplerColor, TexCoords).rgb;

    vec2 tex_offset = 1.0 / textureSize(samplerColor, 0) * blur_sample_scale; // gets size of single texel
    vec3 result = texture(samplerColor, TexCoords).rgb * k_weights[4]; // current fragment's contribution

    for (int j = 1; j <= 4; ++j)
    {
        for (int i = 1; i <= LOOPS; ++i)
        {
            vec2 offset = tex_offset * float(i * j);
            if (blurdirection)
            {
                // H
                result += texture(samplerColor, TexCoords + vec2(offset.x, 0.0)).rgb * k_weights[4 - j] * blurStrength;
                result += texture(samplerColor, TexCoords - vec2(offset.x, 0.0)).rgb * k_weights[4 - j] * blurStrength;
            }
            else
            {
                // V
                result += texture(samplerColor, TexCoords + vec2(0.0, offset.y)).rgb * k_weights[4 - j] * blurStrength;
                result += texture(samplerColor, TexCoords - vec2(0.0, offset.y)).rgb * k_weights[4 - j] * blurStrength;
            }
        }
    }

    outFragColor = vec4(result , 1.0);
}
