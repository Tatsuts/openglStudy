#pragma once

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

using namespace std;

char* file_read(const char* filename);

void print_log(GLuint object);

unsigned int create_shader(const char* filename, GLenum type);
