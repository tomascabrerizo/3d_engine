#include "material.h"

Material material_create(TextureIndex t_index, TextureIndex ts_index, float shininess)
{
    Material m = {};
    m.texture_index = t_index;
    m.texture_specular_index = ts_index;
    m.shininess = shininess;
    return m;
}
