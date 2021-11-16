#include "Util.h"

int random_int(int min, int max){
   static bool first = true;
   if (first)
   {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

float dot(glm::vec3 a, glm::vec3 b){
   return a.x * b.x + a.y * b.y + a.z * b.z;
}

float mag(glm::vec3 a)  //calculates magnitude of a
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float angle_between(glm::vec2 a, glm::vec2 b){
   float angle = atan2(a.y - b.y, a.x - b.x);

   return angle;
}

float angle_between3d(glm::vec3 a, glm::vec3 b){
    float dist=sqrt(pow(a.x - b.x,2) + pow(a.y - b.y,2) + pow(a.z - b.z,2));
    float dist2=sqrt(pow(a.x - b.x,2) + pow(a.z - b.z,2));
    return acos(dist2/dist);
}
