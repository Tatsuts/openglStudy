#version 130
out vec4 FragColor;
in vec2 texCoord;
uniform vec3 OurColor;
uniform sampler2D mytexture;
uniform float time;
void main(){
  vec2 flip = vec2(texCoord.x,1.0 - texCoord.y );
  FragColor = texture2D(mytexture, flip) * vec4((OurColor.r * (cos(time + 6.0) + 1.0) * 1.0),
                                                (OurColor.g * (cos(time + 8.0) + 1.0) * 1.3),
                                                (OurColor.b * (cos(time + 4.0) + 1.0) * 1.5),1.0);
  //FragColor = vec4(OurColor,1.0);
};
