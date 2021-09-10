#version 130
out vec4 FragColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;
uniform vec3 LightPos;
uniform vec3 OurColor;
uniform sampler2D mytexture;
uniform float time;
void main(){

  float Distance = length(LightPos - FragPos);

  float attenuation = 1.0 / (1.0 + 0.1*Distance + 0.032*(Distance*Distance));

  vec3 LightColor = vec3(1.0,1.0,1.0);
  float diffuseStrength = 2.0;
  vec3 norm = normalize(Normal);
  vec3 LightDir = normalize((LightPos) - FragPos);
  float diff = max(dot(norm, LightDir), 0.0);

  vec3 diffuse = diff * LightColor * diffuseStrength;
  diffuse *= attenuation;

  float specularStrength = 5.0;

  vec3 reflectDir = reflect(-LightDir, norm);
  float spec = pow(max(dot(LightDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * LightColor;

  specular *= attenuation;

  vec3 result = diffuse + specular;

  vec2 flip = vec2(texCoord.x,1.0 - texCoord.y );
  // FragColor = texture2D(mytexture, flip) * vec4((OurColor.r * (cos(time + 6.0) + 1.0) * 1.0),
  //                                               (OurColor.g * (cos(time + 8.0) + 1.0) * 1.3),
  //                                               (OurColor.b * (cos(time + 4.0) + 1.0) * 1.5),1.0);
  //FragColor = vec4(OurColor,1.0);
  vec3 col = 0.5 + 0.5*cos(time+flip.xyx+vec3(0,2,4));

  FragColor = texture2D(mytexture, flip) * vec4(OurColor, 1.0) * vec4(result,1.0);
};
