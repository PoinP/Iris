#version 330 core

in vec2 v_TexCoord;

out vec4 fragColor;

uniform sampler2D u_TextureSampler;

void main()
{
    fragColor = texture(u_TextureSampler, v_TexCoord);
}