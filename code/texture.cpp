#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "texture.h"

Texture texture_create(const char* path)
{
    Texture new_texture = {};

    glGenTextures(1, &new_texture.id);
    glBindTexture(GL_TEXTURE_2D, new_texture.id);
    SDL_Surface* image = SDL_LoadBMP(path);
    assert(image->format->BytesPerPixel == 4);
    new_texture.w = image->w; 
    new_texture.h = image->h; 
   
    //TODO(tomi): for now is not needed to flip the image because obj files store text_coords in y-reverse order 
    uint32_t* flip_img = (uint32_t*)malloc(image->w*image->h*image->format->BytesPerPixel);
    int dy = image->h-1;
    for(int y = 0; y < image->h; y++)
    {
        for(int x = 0; x < image->w; x++)
        {
            flip_img[(image->w*y)+x] = ((uint32_t*)image->pixels)[(image->w*dy)+x];
        }
        dy--;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (char*)flip_img);
    glGenerateMipmap(GL_TEXTURE_2D);
    free(flip_img);
    SDL_FreeSurface(image);
    printf("TEXTURE: %s Loaded!\n", path); 

    return new_texture;
}
