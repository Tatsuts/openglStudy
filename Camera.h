#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <SDL2/SDL.h>

using namespace std;

class Camera{
  public:
    Camera(glm::vec3 position, glm::vec3 cameraFront);
    void update(float mousex, float mousey, SDL_Window *window);
    void input(float speed);
    glm::mat4 getView();
    glm::mat4 getProjection();
    glm::mat4 getMatrix();


  private:
    glm::vec3 polarVector(float p, float y);
    float clampPitch(float p);
    float clampYaw(float y);
    glm::mat4 _view, _projection;
    glm::vec3 _cameraFront, _position;
    float _sensitivity = 0.1f, _pitch = 0.0f, _yaw = 90.0f;

};
