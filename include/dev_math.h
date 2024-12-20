#include <dev_utils.h>

struct Vector3f {
    float x;
    float y;
    float z;

    Vector3f(){};

    Vector3f(float _x, float _y, float _z){
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f(float _all) {
        x = y = z = _all;
    }
};

struct Matrix4f {
    float m[4][4];

    Matrix4f(){};

    Matrix4f(
        float a00, float a01, float a02, float a03,
        float a10, float a11, float a12, float a13,
        float a20, float a21, float a22, float a23,
        float a30, float a31, float a32, float a33
    ){
        m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
        m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
        m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
        m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
    };
};