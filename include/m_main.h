#pragma once

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "m_camera.h"
#include "m_transform.h"
#include "m_graphics.h"

//Temporary Classe to hold GraphicsData
class TempGraphicsData {
    public:
        TempGraphicsData(){}
        TempGraphicsData(GLFWwindow* _window, Camera* _camera, Transform* _transform) {
            window = _window;
            camera = _camera;
            transform = _transform;
        }

        GLFWwindow* window;
        Camera* camera;
        Transform* transform;

        GLuint vertexBuffer, indexBuffer;
        GLint gUniformTransformMatrixID;
};