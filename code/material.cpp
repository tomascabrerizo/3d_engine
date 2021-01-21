#include "material.h"

Material material_create(v3 ambient, v3 diffuse, v3 specular, float shininess)
{
    Material m = {};
    m.ambient = ambient;
    m.diffuse = diffuse;
    m.specular = specular;
    m.shininess = shininess;
    return m;
}
