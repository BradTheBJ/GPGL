#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform int u_useTexture;

void main()
{
    if (u_useTexture == 1) {
        FragColor = texture(ourTexture, TexCoord);
    } else {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Default orange color
    }
}