#define _USE_MATH_DEFINES
#include <math.h>
#include <dev_utils.h>

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

struct Vector2f {
    float x = 0.0f;
    float y = 0.0f;

    Vector2f(){};

    Vector2f(float _x, float _y){
        x = _x;
        y = _y;
    }

    Vector2f(float _all) {
        x = y = _all;
    }
};

struct Vector3f {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3f(){};

    Vector3f(float _x, float _y, float _z = 0.0f){
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f(float _all) {
        x = y = z = _all;
    }
};

struct Vector4f {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    Vector4f(){};

    Vector4f(float _x, float _y, float _z, float _w){
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vector4f(float _all) {
        x = y = z = w = _all;
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

    inline Matrix4f operator * (const Matrix4f& right) const {
        Matrix4f ret;

        ret.m[0][0] = (m[0][0] * right.m[0][0]) + (m[0][1] * right.m[1][0]) + (m[0][2] * right.m[2][0]) + (m[0][3] * right.m[3][0]);
        ret.m[0][1] = (m[0][0] * right.m[0][1]) + (m[0][1] * right.m[1][1]) + (m[0][2] * right.m[2][1]) + (m[0][3] * right.m[3][1]);
        ret.m[0][2] = (m[0][0] * right.m[0][2]) + (m[0][1] * right.m[1][2]) + (m[0][2] * right.m[2][2]) + (m[0][3] * right.m[3][2]);
        ret.m[0][3] = (m[0][0] * right.m[0][3]) + (m[0][1] * right.m[1][3]) + (m[0][2] * right.m[2][3]) + (m[0][3] * right.m[3][3]);

        ret.m[1][0] = (m[1][0] * right.m[0][0]) + (m[1][1] * right.m[1][0]) + (m[1][2] * right.m[2][0]) + (m[1][3] * right.m[3][0]);
        ret.m[1][1] = (m[1][0] * right.m[0][1]) + (m[1][1] * right.m[1][1]) + (m[1][2] * right.m[2][1]) + (m[1][3] * right.m[3][1]); 
        ret.m[1][2] = (m[1][0] * right.m[0][2]) + (m[1][1] * right.m[1][2]) + (m[1][2] * right.m[2][2]) + (m[1][3] * right.m[3][2]); 
        ret.m[1][3] = (m[1][0] * right.m[0][3]) + (m[1][1] * right.m[1][3]) + (m[1][2] * right.m[2][3]) + (m[1][3] * right.m[3][3]);

        ret.m[2][0] = (m[2][0] * right.m[0][0]) + (m[2][1] * right.m[1][0]) + (m[2][2] * right.m[2][0]) + (m[2][3] * right.m[3][0]); 
        ret.m[2][1] = (m[2][0] * right.m[0][1]) + (m[2][1] * right.m[1][1]) + (m[2][2] * right.m[2][1]) + (m[2][3] * right.m[3][1]); 
        ret.m[2][2] = (m[2][0] * right.m[0][2]) + (m[2][1] * right.m[1][2]) + (m[2][2] * right.m[2][2]) + (m[2][3] * right.m[3][2]); 
        ret.m[2][3] = (m[2][0] * right.m[0][3]) + (m[2][1] * right.m[1][3]) + (m[2][2] * right.m[2][3]) + (m[2][3] * right.m[3][3]);

        ret.m[3][0] = (m[3][0] * right.m[0][0]) + (m[3][1] * right.m[1][0]) + (m[3][2] * right.m[2][0]) + (m[3][3] * right.m[3][0]); 
        ret.m[3][1] = (m[3][0] * right.m[0][1]) + (m[3][1] * right.m[1][1]) + (m[3][2] * right.m[2][1]) + (m[3][3] * right.m[3][1]); 
        ret.m[3][2] = (m[3][0] * right.m[0][2]) + (m[3][1] * right.m[1][2]) + (m[3][2] * right.m[2][2]) + (m[3][3] * right.m[3][2]); 
        ret.m[3][3] = (m[3][0] * right.m[0][3]) + (m[3][1] * right.m[1][3]) + (m[3][2] * right.m[2][3]) + (m[3][3] * right.m[3][3]);

        return ret;
    }
};