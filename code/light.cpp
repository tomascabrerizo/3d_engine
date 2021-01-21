#include "light.h"

Light light_create(v3 ambient, v3 diffuse, v3 specular)
{
    Light l = {};
    l.ambient = ambient;
    l.diffuse = diffuse;
    l.specular = specular;
    return l;
}
