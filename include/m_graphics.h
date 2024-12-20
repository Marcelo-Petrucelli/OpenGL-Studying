#ifndef DEV_GRAPHICS_H
#define DEV_GRAPHICS_H

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "m_math.h"

static const char* vertexShaderFileName = "shaders/shader.vs";
static const char* fragmentShaderFileName = "shaders/shader.fs"; 

struct Vertex {
    Vector3f pos;
    Vector4f color;
    Vector2f texCoord;
    Vector3f normal;

    Vertex(){}

    Vertex(Vector3f _pos, Vector4f _color){
        pos = _pos;
        color = _color;
    }

    Vertex(Vector3f _pos, Vector3f _color){
        pos = _pos;
        color = Vector4f(_color.x, _color.y, _color.z, 1.0);
    }

    //Temporary
    Vertex(Vector3f _pos){
        pos = _pos;
        color = Vector4f(
            (float)rand() / (float)RAND_MAX,
            (float)rand() / (float)RAND_MAX,
            (float)rand() / (float)RAND_MAX,
            1.0
        );
    }
};

#endif