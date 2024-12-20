#ifndef CAMERA_H
#define CAMERA_H

#include "m_graphics.h"

#define InverseHalfFov(f) (float)(1.0f / tanf(ToRadian(f / 2.0f)))
#define ZFrustonTransformRange(nZ, fZ) (float)(nZ - fZ)

#define ProjectionA(nZ, fZ) (float)((- fZ - nZ) / ZFrustonTransformRange(nZ, fZ))
#define ProjectionB(nZ, fZ) (float)((2 * nZ * fZ) / ZFrustonTransformRange(nZ, fZ))

struct DefaultUpVector : Vector3f {
    DefaultUpVector() : Vector3f(1.0f, 0.0f, 0.0f) {}
};

struct DefaultRightVector : Vector3f {
    DefaultRightVector() : Vector3f(0.0f, 1.0f, 0.0f) {}
};

struct DefaultFrontVector : Vector3f {
    DefaultFrontVector() : Vector3f(0.0f, 0.0f, 1.0f) {}
};

#define UP DefaultUpVector()
#define RIGHT DefaultRightVector()
#define FRONT DefaultFrontVector()

class Camera {
    public:
        Camera(float _screenRatio, Vector3f _position = Vector3f(), Vector2f _fov = 90.0f, Vector2f _nearFar = Vector2f(1.0f, 10.0f), float _speed = 1.0f){
            ratio = _screenRatio;
            position = _position;
            fov = _fov;
            nearFar = _nearFar;
            speed = _speed;
        }

        void setScreenRatio(float);
        void setPosition(Vector3f);
        void setTarget(Vector3f);
        void setFOV(Vector2f);
        void setNear(float);
        void setFar(float);
        void setSpeed(float);

        void lookAt(Vector3f, Vector3f);
        void onKeyboard(unsigned char);

        virtual Matrix4f getMatrix();
    
    private:
        Vector3f position;
        Vector3f front = FRONT;
        Vector3f up = UP;
        float speed = 1.0f;

        Vector2f fov;
        float ratio;
        Vector2f nearFar;
};

struct ViewMatrix : Matrix4f {
    ViewMatrix(Vector3f pos, Vector3f u, Vector3f v, Vector3f n) : Matrix4f(
        u.x, u.y, u.z, -pos.x,
        v.x, v.y, v.z, -pos.y,
        n.x, n.y, n.z, -pos.z,
        0.0f, 0.0f, 0.0f, 1.0f
    ){}
};

struct PerspectiveProjectionMatrix : Matrix4f {
    PerspectiveProjectionMatrix(Vector2f fov, float rasterRatio, Vector2f nearFar) : Matrix4f(
        InverseHalfFov(fov.x) / rasterRatio, 0.0f, 0.0f, 0.0f,
        0.0f, InverseHalfFov(fov.y), 0.0f, 0.0f,
        0.0f, 0.0f, ProjectionA(nearFar.x, nearFar.y), ProjectionB(nearFar.x, nearFar.y),
        0.0f, 0.0f, 1.0f, 0.0f
    ){}
};

#endif