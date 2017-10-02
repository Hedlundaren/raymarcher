#include "ray.h"


Ray::Ray(){
    direction = glm::vec3(3, 2, 1);
    radiosity = 5.5f;
}

float Ray::dummy(){
    return radiosity;
}