#include "Camera.h"

#define CTR_X (640 / 2)
#define CTR_Y (480 / 2)

extern float delta;

//Private functions
glm::vec3 Camera::polarVector(float p, float y){
    // this form is already normalized
    return glm::vec3(std::cos(y) * std::cos(p),
                     std::sin(p),
                     std::sin(y) * std::cos(p));
}

float Camera::clampPitch(float p){
    return p > 46.0f ? 45.0f : (p < -45.0f ? -45.0f : p);
}

float Camera::clampYaw(float y){
    float temp = (y + 180.0f) / 360.0f;
    return y - ((int)temp - (temp < 0.0f ? 1 : 0)) * 360.0f;
}

//Public functions
Camera::Camera(glm::vec3 position, glm::vec3 cameraFront){
  _position = position;
  _cameraFront = cameraFront;
  _view = glm::mat4(1.0f);
  _projection = glm::mat4(1.0f);
}

void Camera::update(float mousex, float mousey, SDL_Window *window){
  //float angle;

  // if(state[SDL_SCANCODE_Q]){
  //   angle += 1;
  //   cout << angle << endl;
  // }
  // if(state[SDL_SCANCODE_E]){
  //   angle -= 1;
  //   cout << angle << endl;
  // }

  float deltaX = mousex - CTR_X;
  float deltaY = mousey - CTR_Y;

  _yaw = clampYaw(_yaw + _sensitivity * deltaX);
  _pitch = clampPitch(_pitch - _sensitivity * deltaY);

  _cameraFront = polarVector(glm::radians(_pitch), glm::radians(_yaw));
  //glm::vec3 _cameraFront = glm::rotateY(glm::vec3(0.0,0.0,-1.0),glm::radians(angle));

  //cout << _cameraFront.x << " " << _cameraFront.y << " " << _cameraFront.z << endl;
  SDL_WarpMouseInWindow(window, CTR_X, CTR_Y);


  //_projection = glm::ortho((float)0.0, (float)320.0f, (float)0.0f, (float)240, (float)0.2f, (float)20.0f);
}

void Camera::input(float speed){
  float deltaTime = 1000.0f / delta;
  const Uint8* state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_W]){
    _position.x += (_cameraFront.x) * speed * deltaTime;
    _position.z += (_cameraFront.z) * speed * deltaTime;
    //cout << _position.x << " " << _position.y << " " << _position.z << endl;
  }
  if(state[SDL_SCANCODE_D]){
    _position.x += (glm::cross(_cameraFront,glm::vec3(0.0,1.0,0.0)).x) * speed * deltaTime;
    _position.z += (glm::cross(_cameraFront,glm::vec3(0.0,1.0,0.0)).z) * speed * deltaTime;
    //cout << _position.x << " " << _position.y << " " << _position.z << endl;
  }
  if(state[SDL_SCANCODE_A]){
    _position.x -= (glm::cross(_cameraFront,glm::vec3(0.0,1.0,0.0)).x) * speed * deltaTime;
    _position.z -= (glm::cross(_cameraFront,glm::vec3(0.0,1.0,0.0)).z) * speed * deltaTime;
    //cout << _position.x << " " << _position.y << " " << _position.z << endl;
  }
  if(state[SDL_SCANCODE_S]){
    _position.x -= (_cameraFront.x) * speed * deltaTime;
    _position.z -= (_cameraFront.z) * speed * deltaTime;
    //cout << _position.x << " " << _position.y << " " << _position.z << endl;
  }
  if(state[SDL_SCANCODE_R]){
    _position = glm::vec3(0.0,0.0,0.0);
    //cout << _position.x << " " << _position.y << " " << _position.z << endl;
  }
  if(state[SDL_SCANCODE_SPACE]){
    _position.y += speed * deltaTime;
    //cout << _position.x << " " << _position.y << " " << _position.z << endl;
  }
  if(state[SDL_SCANCODE_LSHIFT]){
    _position.y -= speed * deltaTime;
    //cout << _position.x << " " << _position.y << " " << _position.z << endl;
  }

  glm::vec3 _up = glm::vec3(0.0,1.0,0.0);
  _view = glm::lookAt(_position, _position + _cameraFront, _up);

  _projection = glm::perspective(45.0f, (float)(320)/(float)(240), 0.2f, 200.0f);
}

glm::mat4 Camera::getView(){
  return _view;
}

glm::mat4 Camera::getProjection(){
  return _projection;
}

glm::mat4 Camera::getMatrix(){
  return _view * _projection;
}

glm::vec3 Camera::getCameraFront(){
  return _cameraFront;
}
glm::vec3 Camera::getPosition(){
  return _position;
}
