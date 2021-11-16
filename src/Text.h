#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "ShaderUtil.h"

using namespace std;

class Text{
  public:
    Text(string text, glm::vec3 position, glm::vec3 color, int size);
    ~Text();
    bool init();
    void draw(string text = "blabla");
    void update();
    void free();

  private:
    TTF_Font *_font;
    SDL_Surface *res_texture;
    SDL_Color _Color;
    string _text;
    glm::vec3 _position, _color;
    int _size;
    unsigned int _VBO = 0, _EBO = 0, _texture_id, _fragmentShader, _vertexShader, _shaderProgram;
};
