#pragma once

#include "m_graphics.h"

struct DefaultUpVector : Vector3f {
    DefaultUpVector() : Vector3f(0.0f, 1.0f, 0.0f) {}
};

struct DefaultRightVector : Vector3f {
    DefaultRightVector() : Vector3f(1.0f, 0.0f, 0.0f) {}
};

struct DefaultFrontVector : Vector3f {
    DefaultFrontVector() : Vector3f(0.0f, 0.0f, 1.0f) {}
};

#define UP DefaultUpVector()
#define RIGHT DefaultRightVector()
#define FRONT DefaultFrontVector()

class Camera {
    public:
        Camera(float _screenRatio, Vector3f _position = Vector3f(), Vector2f _fov = Vector2f(90.0f, 90.0f), Vector2f _nearFar = Vector2f(1.0f, 10.0f), float _speed = 1.0f){
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