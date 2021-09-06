#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Triangle.h"
#include "Camera.h"

glm::vec3 cameraFront;

const int FPS = 10;
const int frameDelay = 1000 / FPS;
Uint32 frameStart = SDL_GetTicks();
int frameTime;
string globalString = "Global streing porra";
float delta;

//https://gamedev.stackexchange.com/questions/30946/camera-rotation-first-person-camera-using-glm

void RenderText(const char* message, SDL_Color color, int x, int y, int size) {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  TTF_Font * font = TTF_OpenFont("pixelplay.ttf", size);
  SDL_Surface * sFont = TTF_RenderText_Blended(font, message, color);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);

  glBegin(GL_QUADS);
  {
    glTexCoord2f(0,0); glVertex2f(x, y);
    glTexCoord2f(1,0); glVertex2f(x + sFont->w, y);
    glTexCoord2f(1,1); glVertex2f(x + sFont->w, y + sFont->h);
    glTexCoord2f(0,1); glVertex2f(x, y + sFont->h);
  }
  glEnd();

  glDeleteTextures(1, &texture);
  TTF_CloseFont(font);
  SDL_FreeSurface(sFont);
}

void render(SDL_Window *window, vector <Triangle *>triangle_test){
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  SDL_Color color = {255, 0, 0, 0};
  //RenderText("teste blabla", color, 0, 0, 10);
  for(int i = 0; i < (int)triangle_test.size(); i++){
    triangle_test[i]->draw();
  }
  SDL_GL_SwapWindow(window);
}

void mainLoop(SDL_Window *window, vector <Triangle *>triangle_test, Camera camera){
  SDL_WarpMouseInWindow(window, 640 / 2, 480 / 2);
  while(true){
    Uint64 start = SDL_GetPerformanceCounter();

  	// Do event loop

  	// Do physics loop

  	// Do rendering loop

    SDL_Event ev;
    while(SDL_PollEvent(&ev)){
      if(ev.type == SDL_QUIT) return;

      if (ev.type == SDL_MOUSEMOTION){
        SDL_ShowCursor(SDL_DISABLE);
        //SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_WarpMouseInWindow(window, 640 / 2, 480 / 2);
        camera.update((float)ev.motion.x,(float)ev.motion.y,window);
      }
      if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) glViewport(0, 0, ev.window.data1, ev.window.data2);

    }
    camera.input(0.005f);

    render(window, triangle_test);
    for(int i = 0; i < (int)triangle_test.size(); i++){
      triangle_test[i]->update(camera.getView(),camera.getProjection());
    }
    Uint64 end = SDL_GetPerformanceCounter();

    float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
    delta = (1.0f / elapsed);
    cout << "Current FPS: " << to_string(1.0f / elapsed) << endl;
    SDL_Delay(elapsed);
  }
}

int main(int argc, char *argv[]){
  // float vertices1[] = {
  //    0.0f,  0.0f,  0.0f,
  //    0.0f,  1.0f,  0.0f,
  //    1.0f,  0.0f,  0.0f,
  //    0.0f,  0.0f,  0.0f
  // };

  //Init Window and Opengl Context.
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Olá mundo porra",
      500, SDL_WINDOWPOS_CENTERED,
      640, 480,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  SDL_GL_CreateContext(window);

  GLenum glew_status = glewInit();
  if(glew_status != GLEW_OK){
    cerr << "Error: glewInit: " <<glewGetErrorString(glew_status) << endl;
    return EXIT_FAILURE;
  }
  SDL_GL_SetSwapInterval(1);

  Camera cam(glm::vec3(0.0,0.0,0.0), glm::vec3(1.0,0.0,0.0));
  float vertices[] = {
    // front
    -1.0, -1.0,  1.0,     0.0, 0.0,
     1.0, -1.0,  1.0,     1.0, 0.0,
     1.0,  1.0,  1.0,     1.0, 1.0,
    -1.0,  1.0,  1.0,     0.0, 1.0,
    // top
    -1.0,  1.0,  1.0,     0.0, 0.0,
     1.0,  1.0,  1.0,     1.0, 0.0,
     1.0,  1.0, -1.0,     1.0, 1.0,
    -1.0,  1.0, -1.0,     0.0, 1.0,
    // back
     1.0, -1.0, -1.0,     0.0, 0.0,
    -1.0, -1.0, -1.0,     1.0, 0.0,
    -1.0,  1.0, -1.0,     1.0, 1.0,
     1.0,  1.0, -1.0,     0.0, 1.0,
    // bottom
    -1.0, -1.0, -1.0,     0.0, 0.0,
     1.0, -1.0, -1.0,     1.0, 0.0,
     1.0, -1.0,  1.0,     1.0, 1.0,
    -1.0, -1.0,  1.0,     0.0, 1.0,
    // left
    -1.0, -1.0, -1.0,     0.0, 0.0,
    -1.0, -1.0,  1.0,     1.0, 0.0,
    -1.0,  1.0,  1.0,     1.0, 1.0,
    -1.0,  1.0, -1.0,     0.0, 1.0,
    // right
     1.0, -1.0,  1.0,     0.0, 0.0,
     1.0, -1.0, -1.0,     1.0, 0.0,
     1.0,  1.0, -1.0,     1.0, 1.0,
     1.0,  1.0,  1.0,     0.0, 1.0,
  };

  vector <Triangle *>triangle_test;
  triangle_test.push_back(new Triangle("blabla", glm::vec3(2.0,0.0,0.0), 0.0, glm::vec3(1.0, 0.5, 0.5)));
  triangle_test.push_back(new Triangle("blabla", glm::vec3(0.0,0.0,2.0), 0.0, glm::vec3(0.0, 1.0, 0.0)));
  triangle_test.push_back(new Triangle("blabla", glm::vec3(-2.0,0.0,0.0), 0.0, glm::vec3(1.0, 0.5, 0.5)));
  triangle_test.push_back(new Triangle("blabla", glm::vec3(0.0,0.0,-2.0), 0.0, glm::vec3(0.0, 1.0, 0.0)));
  //triangle_test.push_back(new Triangle("blabla2", glm::vec3(1.0,0.0,0.0), 90.0));

  for(int j = 0; j < (int)triangle_test.size(); j ++){
    if (!triangle_test[j]->init(vertices,sizeof(vertices))){
      cout << "Fail" << endl;
      return EXIT_FAILURE;
    };
  }


  //triangle_test[1]->init(vertices,sizeof(vertices));

  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  glEnable(GL_DEPTH_TEST);


  mainLoop(window,triangle_test, cam);



  for(int i = 0; i < (int)triangle_test.size(); i++){
     triangle_test[i]->free();
  }

  //free_resources();

  return EXIT_SUCCESS;
}
