#version 410 core

precision highp float;


// The Emssive Image from MRT Pass
uniform sampler2D uBlurSourceImage;
// The Blur Direction, Horizontal or Vertical
uniform bool uBlurDirection;
// The Down Sampling Scale to the Source Image
uniform float uBlurSamplingScale;
// The Blur Strength to the Source Image
uniform float uBlurStrength;
// The Blur Kernerl represented in a mat3
uniform mat3 uBlurKernel;

/* OUTPUT */
out vec4 outFragColor;

/* INPUT */
in vec2 TexCoords;


const int LOOPS = 3;

float k_weights[9] = float[](
uBlurKernel[0][0], uBlurKernel[0][1], uBlurKernel[0][2],
uBlurKernel[1][0], uBlurKernel[1][1], uBlurKernel[1][2],
uBlurKernel[2][0], uBlurKernel[2][1], uBlurKernel[2][2]
);


void main()
{
    vec3 color = texture(uBlurSourceImage, TexCoords).rgb;

    vec2 tex_offset = 1.0 / textureSize(uBlurSourceImage, 0) * uBlurSamplingScale;// gets size of single texel
    vec3 result = texture(uBlurSourceImage, TexCoords).rgb * k_weights[4];// current fragment's contribution

    for (int j = 1; j <= 4; ++j)
    {
        for (int i = 1; i <= LOOPS; ++i)
        {
            vec2 offset = tex_offset * float(i * j);
            if (uBlurDirection)
            {
                // H
                result += texture(uBlurSourceImage, TexCoords + vec2(offset.x,  - offset.y / 2.f)).rgb * k_weights[4 - j] * uBlurStrength / 2.f;
                result += texture(uBlurSourceImage, TexCoords + vec2(offset.x,  offset.y / 2.f)).rgb * k_weights[4 - j] * uBlurStrength  / 2.f;
                result += texture(uBlurSourceImage, TexCoords - vec2(offset.x,  - offset.y / 2.f)).rgb * k_weights[4 - j] * uBlurStrength  / 2.f;
                result += texture(uBlurSourceImage, TexCoords - vec2(offset.x,  offset.y / 2.f)).rgb * k_weights[4 - j] * uBlurStrength  / 2.f;
            }
            else
            {
                // V
                result += texture(uBlurSourceImage, TexCoords + vec2(- offset.x/2.f, offset.y)).rgb * k_weights[4 - j] * uBlurStrength /2.f;
                result += texture(uBlurSourceImage, TexCoords + vec2(offset.x/2.f, offset.y)).rgb * k_weights[4 - j] * uBlurStrength /2.f;
                result += texture(uBlurSourceImage, TexCoords - vec2( - offset.x/2.f, offset.y)).rgb * k_weights[4 - j] * uBlurStrength /2.f;
                result += texture(uBlurSourceImage, TexCoords - vec2( offset.x/2.f, offset.y)).rgb * k_weights[4 - j] * uBlurStrength /2.f;
                
            }
        }
    }

    outFragColor = vec4(result, 1.0);
}
