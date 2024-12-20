#include "m_camera.h"

void Camera::setScreenRatio(float _screenRatio){
    ratio = _screenRatio;
}

void Camera::setPosition(Vector3f _position){
    position = _position;
}

void Camera::setTarget(Vector3f _target){
    
}

void Camera::setFOV(Vector2f _fov){
    fov = _fov;
}

void Camera::setNear(float _near){
    nearFar = Vector2f(_near, nearFar.y);
}

void Camera::setFar(float _far){
    nearFar = Vector2f(nearFar.x, _far);
}

void Camera::setSpeed(float _speed){
    speed = _speed;
}



void Camera::lookAt(Vector3f, Vector3f){

}

void Camera::onKeyboard(unsigned char _key){
    switch(_key){
        case GLUT_KEY_PAGE_UP:
            {
                position += (front * speed);
            }
        break;

        case GLUT_KEY_PAGE_DOWN:
            {
                position -= (front * speed);
            }
        break;

        case GLUT_KEY_LEFT:
            {
                Vector3f left = up.cross(front);
                left.normalize();
                position += (left * speed);
            }
        break;

        case GLUT_KEY_RIGHT:
            {
                Vector3f right = front.cross(up);
                right.normalize();
                position += (right * speed);
            }
        break;

        case GLUT_KEY_UP:
            {
                position.y += speed;
            }
        break;

        case GLUT_KEY_DOWN:
            {
                position.y -= speed;
            }
        break;
    }
}


Matrix4f Camera::getMatrix(){
    PerspectiveProjectionMatrix projection(fov, ratio, nearFar);

    front.normalize();

    Vector3f u = up.cross(front);
    u.normalize();

    //UP could be V, but it is probably not perpendicular to Front/Target, so we cross Front/Target with U to get V
    Vector3f v = front.cross(u);
    v.normalize();

    ViewMatrix view(position, u, v, front);

    return projection; // * view
}