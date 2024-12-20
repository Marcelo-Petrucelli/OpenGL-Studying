#include "m_transform.h"

void Transform::setPosition(Vector3f _position){
    position = _position;
};

void Transform::setRotation(Vector3f _rotation){
    rotation = _rotation;
};

void Transform::setScale(Vector3f _scale){
    scale = _scale;
};


void Transform::translateBy(Vector3f _translateBy){
    position += _translateBy;
};

void Transform::rotateBy(Vector3f _rotateBy){
    rotation += _rotateBy;
};

void Transform::scaleBy(Vector3f _scaleBy){
    scale += _scaleBy;
};


Matrix4f Transform::getMatrix() {
    ScaleMatrix scaleMatrix(scale);

    RotationXMatrix rotateXMatrix(rotation.x);
    RotationYMatrix rotateYMatrix(rotation.y);
    RotationZMatrix rotateZMatrix(rotation.z);

    TranslationMatrix translateMatrix(position);

    return translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
}