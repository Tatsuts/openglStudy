#pragma once
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

int random_int(int min, int max);

float dot(glm::vec3 a, glm::vec3 b);

float mag(glm::vec3 a);

float angle_between(glm::vec2 a, glm::vec2 b);

float angle_between3d(glm::vec3 a, glm::vec3 b);
