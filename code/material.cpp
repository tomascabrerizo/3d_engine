#include "material.h"

Material material_create(TextureIndex t_index, TextureIndex ts_index, float shininess)
{
    Material m = {};
    m.texture_index = t_index;
    m.texture_specular_index = ts_index;
    m.shininess = shininess;
    return m;
}

Material material_create(v3 ambient, v3 diffuse, v3 specular, float shininess)
{
    Material m = {};
    m.texture_index = TEXTURE_EMPTY;
    m.texture_specular_index = TEXTURE_EMPTY;
    m.ambient = ambient;
    m.diffuse = diffuse;
    m.specular = specular;
    m.shininess = shininess;
    return m;
}
