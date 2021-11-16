#pragma once

#include <iostream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ShaderUtil.h"
#include "Util.h"

using namespace std;

class Cube{
  public:
    Cube(string name, glm::vec3 positon, float angle, glm::vec3 color);
    ~Cube();
    bool init(float vertices[], int bytesize);
    void draw();
    void free();
    void update(glm::mat4 view, glm::mat4 projection, glm::vec3 lightpos);

  private:
    string teste;
    glm::vec3 _position,_color,_rotation_axis;
    float _angle, _time = 0;
    unsigned int _VBO = 0, _EBO = 0, _texture_id, _fragmentShader, _vertexShader, _shaderProgram;
};
