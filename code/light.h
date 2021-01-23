#ifndef LIGHT_H
#define LIGHT_H

#include "math.h"


struct PointLight 
{
    v3 position;
    
    float constant;
    float linear;
    float quadratic;

    v3 ambient;
    v3 diffuse;
    v3 specular;
};

struct DirLight 
{
    v3 direction;

    v3 ambient;
    v3 diffuse;
    v3 specular;
};

#endif
