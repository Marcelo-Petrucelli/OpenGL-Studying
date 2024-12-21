#ifndef TRANSFORM_H
#define TRANSFORM_H

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

struct ScaleMatrix : Matrix4f {
    ScaleMatrix(Vector3f& scale) : Matrix4f(
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ) {}
};

struct RotationXMatrix : Matrix4f {
    RotationXMatrix(float angleInDegrees = 0.0f) : Matrix4f(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosf(ToRadian(angleInDegrees)), -sinf(ToRadian(angleInDegrees)), 0.0f,
        0.0f, sinf(ToRadian(angleInDegrees)), cosf(ToRadian(angleInDegrees)), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ) {}
};

struct RotationYMatrix : Matrix4f {
    RotationYMatrix(float angleInDegrees = 0.0f) : Matrix4f(
        cosf(ToRadian(angleInDegrees)), 0.0f, -sinf(ToRadian(angleInDegrees)), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(ToRadian(angleInDegrees)), 0.0f, cosf(ToRadian(angleInDegrees)), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ) {}
};

struct RotationZMatrix : Matrix4f {
    RotationZMatrix(float angleInDegrees = 0.0f) : Matrix4f(
        cosf(ToRadian(angleInDegrees)),-sinf(ToRadian(angleInDegrees)), 0.0f, 0.0f,
        sinf(ToRadian(angleInDegrees)), cosf(ToRadian(angleInDegrees)), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ) {}
};

struct TranslationMatrix : Matrix4f {
    TranslationMatrix(Vector3f& translation) : Matrix4f(
        1.0f, 0.0f, 0.0f, translation.x,
        0.0f, 1.0f, 0.0f, translation.y,
        0.0f, 0.0f, 1.0f, translation.z,
        0.0f, 0.0f, 0.0f, 1.0f
    ) {}
};

#endif