//Includes of libraries.
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>

#include "Cube.h"
#include "Camera.h"
#include "Text.h"

//Initialization of Global variables.
glm::vec3 cameraFront;

const int FPS = 10;
const int frameDelay = 1000 / FPS;
Uint32 frameStart = SDL_GetTicks();
int frameTime;
string globalString = "Global streing porra";
float delta;


//Render function to render things.
void render(SDL_Window *window, vector <Cube *>Cube_test, vector <Text *>Text_test){
  //Clear the framebuffer and applies a color.
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


  //Draw all objects of Cube_test.
  for(int i = 0; i < (int)Cube_test.size(); i++){
    Cube_test[i]->draw();
  }
  float newdelta = floorf(delta * 100)/100;
  for(int j = 0; j < (int)Text_test.size(); j++){
    Text_test[j]->draw("FPS: " + to_string((int)newdelta));
  }
  //renderText(to_string(delta));

  //Update the OpenGl Context.
  SDL_GL_SwapWindow(window);
}

//The main loop of program.
void mainLoop(SDL_Window *window, vector <Cube *>Cube_test, vector <Text *>Text_test, Camera camera){
  //Warp the mouse in the window center.
  //SDL_WarpMouseInWindow(window, 640 / 2, 480 / 2);


  while(true){
    //Get the processor clock in the start of loop.
    Uint64 start = SDL_GetPerformanceCounter();

    //Start input events.
    SDL_Event ev;
    while(SDL_PollEvent(&ev)){
      //Check if the window has been closed.
      if(ev.type == SDL_QUIT) return;

      //Check mouse motion events.
      if (ev.type == SDL_MOUSEMOTION){
        //Disable the vision of cursor and update the camera.
        SDL_ShowCursor(SDL_DISABLE);
        SDL_WarpMouseInWindow(window, 640 / 2, 480 / 2);
        camera.update((float)ev.motion.x,(float)ev.motion.y,window);
      }
      //Check if the window has been resized and change the viewport to the new size.
      if(ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) glViewport(0, 0, ev.window.data1, ev.window.data2);

    }

    //Start the input function of the camera.
    camera.input(0.005f);

    //Start the render function.
    render(window, Cube_test, Text_test);

    //Start the update function of the objects.
    for(int i = 0; i < (int)Cube_test.size(); i++){
      Cube_test[i]->update(camera.getView(),camera.getProjection(),camera.getPosition());
    }
    for(int j = 0; j < (int)Text_test.size(); j++){
      Text_test[j]->update(to_string(delta));
    }


    //Get the processor clock in the end of loop.
    Uint64 end = SDL_GetPerformanceCounter();
    //Get the frametime and limit it to 60 frames per second.
    float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
    delta = (1.0f / elapsed);
    //cout << "Current FPS: " << to_string(1.0f / elapsed) << endl;
    SDL_Delay(elapsed);
  }
}

int main(int argc, char *argv[]){
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


  //Set the swap interval.
  SDL_GL_SetSwapInterval(1);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);


  //Create a camera object.
  Camera cam(glm::vec3(0.0,0.0,0.0), glm::vec3(1.0,0.0,0.0));


  //Store the vertices of the cube.
  float vertices[] = {
    // front
    -1.0, -1.0,  1.0,     0.0, 0.0,   0.0f, 0.0f, 1.0f,
     1.0, -1.0,  1.0,     1.0, 0.0,   0.0f, 0.0f, 1.0f,
     1.0,  1.0,  1.0,     1.0, 1.0,   0.0f, 0.0f, 1.0f,
    -1.0,  1.0,  1.0,     0.0, 1.0,   0.0f, 0.0f, 1.0f,
    // top
    -1.0,  1.0,  1.0,     0.0, 0.0,   0.0f, 1.0f,  0.0f,
     1.0,  1.0,  1.0,     1.0, 0.0,   0.0f, 1.0f,  0.0f,
     1.0,  1.0, -1.0,     1.0, 1.0,   0.0f, 1.0f,  0.0f,
    -1.0,  1.0, -1.0,     0.0, 1.0,   0.0f, 1.0f,  0.0f,
    // back
     1.0, -1.0, -1.0,     0.0, 0.0,   0.0f, 0.0f, -1.0f,
    -1.0, -1.0, -1.0,     1.0, 0.0,   0.0f, 0.0f, -1.0f,
    -1.0,  1.0, -1.0,     1.0, 1.0,   0.0f, 0.0f, -1.0f,
     1.0,  1.0, -1.0,     0.0, 1.0,   0.0f, 0.0f, -1.0f,
    // bottom
    -1.0, -1.0, -1.0,     0.0, 0.0,   0.0f, -1.0f, 0.0f,
     1.0, -1.0, -1.0,     1.0, 0.0,   0.0f, -1.0f, 0.0f,
     1.0, -1.0,  1.0,     1.0, 1.0,   0.0f, -1.0f, 0.0f,
    -1.0, -1.0,  1.0,     0.0, 1.0,   0.0f, -1.0f, 0.0f,
    // left
    -1.0, -1.0, -1.0,     0.0, 0.0,  -1.0f,  0.0f,  0.0f,
    -1.0, -1.0,  1.0,     1.0, 0.0,  -1.0f,  0.0f,  0.0f,
    -1.0,  1.0,  1.0,     1.0, 1.0,  -1.0f,  0.0f,  0.0f,
    -1.0,  1.0, -1.0,     0.0, 1.0,  -1.0f,  0.0f,  0.0f,
    // right
     1.0, -1.0,  1.0,     0.0, 0.0,   1.0f,  0.0f,   0.0f,
     1.0, -1.0, -1.0,     1.0, 0.0,   1.0f,  0.0f,   0.0f,
     1.0,  1.0, -1.0,     1.0, 1.0,   1.0f,  0.0f,   0.0f,
     1.0,  1.0,  1.0,     0.0, 1.0,   1.0f,  0.0f,   0.0f
  };

  //Create a vector object of the cube and instantiates it.
  vector <Cube *>Cube_test;
  Cube_test.push_back(new Cube("blabla", glm::vec3(2.0,0.0,0.0), 0.0, glm::vec3(1.0, 1.0, 1.0)));
  Cube_test.push_back(new Cube("blabla", glm::vec3(0.0,0.0,2.0), 0.0, glm::vec3(1.0, 1.0, 1.0)));
  Cube_test.push_back(new Cube("blabla", glm::vec3(-2.0,0.0,0.0), 0.0, glm::vec3(1.0, 0.5, 0.5)));
  Cube_test.push_back(new Cube("blabla", glm::vec3(0.0,0.0,-2.0), 0.0, glm::vec3(0.0, 1.0, 0.0)));

  vector <Text *>Text_test;
  Text_test.push_back(new Text("teste2", glm::vec3(0.1,0.0,0.0), glm::vec3(1.0, 1.0, 1.0), 25));

  //Start the initialize function of the objects
  for(int j = 0; j < (int)Cube_test.size(); j++){
    if (!Cube_test[j]->init(vertices,sizeof(vertices))){
      cout << "Fail" << endl;
      return EXIT_FAILURE;
    };
  }

  for(int k = 0; k < (int)Text_test.size(); k++){
    if (!Text_test[k]->init()){
      cout << "Fail" << endl;
      return EXIT_FAILURE;
    };
  }

  //Enable the depth test of Opengl;
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

  //Start the main loop of the program;
  mainLoop(window, Cube_test, Text_test, cam);


  //Free all the memory allocated of the objects;
  for(int i = 0; i < (int)Cube_test.size(); i++){
     Cube_test[i]->free();
  }
  for(int o = 0; o < (int)Text_test.size(); o++){
     Text_test[o]->free();
  }

  return EXIT_SUCCESS;
}
