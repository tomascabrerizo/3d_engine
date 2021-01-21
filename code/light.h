#ifndef LIGHT_H
#define LIGHT_H

#include "math.h"

//TODO(tomi):Later light will have popsition and will be renderables
// for now we just use a simple renderable with mesh and set light position manualy
struct Light 
{
    v3 ambient;
    v3 diffuse;
    v3 specular;
};

Light light_create(v3 ambient, v3 diffuse, v3 specular);

#endif
