#version 130
in vec3 aPos;
in vec2 aCoord;

out vec2 texCoord;
uniform mat4 m_transform;

void main(){
  gl_Position = m_transform * vec4(aPos, 1.0);

  texCoord = aCoord;
}
