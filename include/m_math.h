#pragma once

#define _USE_MATH_DEFINES
#include <cstdint>
#include <math.h>

#include "m_utils.h"

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

//https://en.wikipedia.org/wiki/Fast_inverse_square_root
static float Q_rsqrt(float number) {
    union {
        float    f;
        uint32_t i;
    } conv = { .f = number };

    conv.i  = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
}

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

    void normalize () {
        float invMod = Q_rsqrt(x*x + y*y);
        x = x*invMod;
        y = y*invMod;
    }

    inline Vector2f operator + (const Vector2f& other) const {
        Vector2f result(*this);  // Copy.
        return result += other;
    }

    inline Vector2f operator += (const Vector2f& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline Vector2f operator - (const Vector2f& other) const {
        Vector2f result(*this);  // Copy.
        return result -= other;
    }

    inline Vector2f operator -= (const Vector2f& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline Vector2f operator * (const float scalar) const {
        Vector2f result(*this);  // Copy.
        return result *= scalar;
    }

    inline Vector2f operator *= (const float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
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

    Vector3f cross (const Vector3f other) {
        return Vector3f((y*other.z) - (z*other.y), (z*other.x) - (x*other.z), (x*other.y) - (y*other.x));
    }

    void normalize () {
        float invMod = Q_rsqrt(x*x + y*y + z*z);
        x = x*invMod;
        y = y*invMod;
        z = z*invMod;
    }

    inline Vector3f& operator + (const Vector3f& other) const {
        Vector3f result(*this);  // Copy.
        return result += other;
    }

    inline Vector3f& operator += (const Vector3f& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    inline Vector3f& operator - (const Vector3f& other) const {
        Vector3f result(*this);  // Copy.
        return result -= other;
    }

    inline Vector3f& operator -= (const Vector3f& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    inline Vector3f& operator * (const float scalar) const {
        Vector3f result(*this);  // Copy.
        return result *= scalar;
    }

    inline Vector3f& operator *= (const float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
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

    void normalize () {
        float invMod = Q_rsqrt(x*x + y*y + z*z + w*w);
        x = x*invMod;
        y = y*invMod;
        z = z*invMod;
        w = w*invMod;
    }

    inline Vector4f operator + (const Vector4f& other) const {
        Vector4f result(*this);  // Copy.
        return result += other;
    }

    inline Vector4f operator += (const Vector4f& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    inline Vector4f operator - (const Vector4f& other) const {
        Vector4f result(*this);  // Copy.
        return result -= other;
    }

    inline Vector4f operator -= (const Vector4f& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    inline Vector4f operator * (const float scalar) const {
        Vector4f result(*this);  // Copy.
        return result *= scalar;
    }

    inline Vector4f operator *= (const float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
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