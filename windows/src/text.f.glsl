#version 130
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D mytexture;
void main()
{
    FragColor = texture2D(mytexture, texCoord);
    //FragColor = vec4(0.0,0.0,0.0,1.0);

}
