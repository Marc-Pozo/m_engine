#version 450 core

uniform float verticalOffset;
uniform float horizontalOffset;

out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor.x * horizontalOffset, ourColor.y * verticalOffset, ourColor.z, 1.0);
}