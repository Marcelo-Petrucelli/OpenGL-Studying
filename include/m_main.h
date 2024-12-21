#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "m_camera.h"
#include "m_transform.h"
#include "m_graphics.h"

//Temporary Classe to hold GraphicsData
class TempGraphicsData {
    public:
        TempGraphicsData(){}
        TempGraphicsData(Camera* _camera, Transform* _transform){
            camera = _camera;
            transform = _transform;
        }

        Camera* camera;
        Transform* transform;

        GLuint vertexBuffer, indexBuffer;
        GLint gUniformTransformMatrixID;
};