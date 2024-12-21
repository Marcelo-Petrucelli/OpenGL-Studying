#pragma once

#include "m_graphics.h"

class Transform {
    public:
        Transform(Vector3f _position = Vector3f(0.0f, 0.0f, 0.0f)){
            position = _position;
        }
        
        void setPosition(Vector3f);
        void setRotation(Vector3f);
        void setScale(Vector3f);

        void translateBy(Vector3f);
        void rotateBy(Vector3f);
        void scaleBy(Vector3f);

        Matrix4f getMatrix();
    
    private:
        Vector3f position;
        Vector3f rotation = Vector3f(0.0f, 0.0f, 0.0f);
        Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f);
};