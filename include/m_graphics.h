#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "m_math.h"

#define InverseHalfFov(f) (float)(1.0f / tanf(ToRadian(f / 2.0f)))
#define ZFrustonTransformRange(nZ, fZ) (float)(nZ - fZ)

#define ProjectionZA(nZ, fZ) (float)((- fZ - nZ) / ZFrustonTransformRange(nZ, fZ))
#define ProjectionZB(nZ, fZ) (float)((2 * nZ * fZ) / ZFrustonTransformRange(nZ, fZ))

static const char* vertexShaderFileName = "shaders/shader.vs";
static const char* fragmentShaderFileName = "shaders/shader.fs";

class TempGraphicsData;

void CompileShaders(TempGraphicsData* graphicsData);
void CreateVertexBuffer(TempGraphicsData* graphicsData);
void CreateIndexBuffer(TempGraphicsData* graphicsData);

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

struct UnitQuaternion : Vector4f {
    float theta;
    Vector3f normal;

    UnitQuaternion(Vector3f _point) : UnitQuaternion(_point, 0.0f) {}

    UnitQuaternion(Vector3f _axis, float _theta){
        normal = _axis;
        theta = _theta;

        float cosHalfT = cosf(theta/2.0f);
        float sinHalfT = sinf(theta/2.0f);
        
        Vector3f v = normal * sinHalfT;

        x = v.x;
        y = v.y;
        z = v.z;
        w = cosHalfT;
    }

    UnitQuaternion conjugate () const {
        //Could be:
        //return Quaternion(this->normal * -1, this->theta);
        UnitQuaternion result(*this); //This seems faster
        result.x *= -1;
        result.y *= -1;
        result.z *= -1;
        return result;
    }

    UnitQuaternion rotate(Vector3f _axis, float _theta) {
        const UnitQuaternion q = UnitQuaternion(_axis, _theta);
        return q * (*this) * (q.conjugate());
    }

    inline UnitQuaternion operator * (const UnitQuaternion& other) const {
        UnitQuaternion result(
            Vector3f(
                (theta * other.x) + (x - other.theta) + (y * other.z) - (z * other.y),
                (theta * other.y) + (y - other.theta) + (z * other.x) - (x * other.z),
                (theta * other.z) + (z - other.theta) + (x * other.y) - (y * other.x)
            ),
            (theta * other.theta) - (x - other.x) - (y * other.y) - (z * other.z)
        );
        return result;
    }
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

struct ViewMatrix : Matrix4f {
    ViewMatrix(Vector3f& u, Vector3f& v, Vector3f& n) : Matrix4f(
        u.x, u.y, u.z, 0.0f,
        v.x, v.y, v.z, 0.0f,
        n.x, n.y, n.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ){}
};

struct PerspectiveProjectionMatrix : Matrix4f {
    PerspectiveProjectionMatrix(Vector2f& fov, float rasterRatio, Vector2f& nearFar) : Matrix4f(
        InverseHalfFov(fov.x) / rasterRatio, 0.0f, 0.0f, 0.0f,
        0.0f, InverseHalfFov(fov.y), 0.0f, 0.0f,
        0.0f, 0.0f, ProjectionZA(nearFar.x, nearFar.y), ProjectionZB(nearFar.x, nearFar.y),
        0.0f, 0.0f, 1.0f, 0.0f
    ){}
};