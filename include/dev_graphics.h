#include <dev_math.h>

#define InverseHalfFov(f) (float)(1.0f / tanf(ToRadian(f / 2.0f)))
#define ZFrustonTransformRange(nZ, fZ) (float)(nZ - fZ)
#define ProjectionA(nZ, fZ) (float)((- fZ - nZ) / ZFrustonTransformRange(nZ, fZ))
#define ProjectionB(nZ, fZ) (float)((2 * nZ * fZ) / ZFrustonTransformRange(nZ, fZ))

struct ProjectionMatrix : Matrix4f {
    ProjectionMatrix(float fov, float rasterRatio, float nearClip = 1.0f, float farClip = 10.0f) : Matrix4f(
        InverseHalfFov(fov) / rasterRatio, 0.0f, 0.0f, 0.0f,
        0.0f, InverseHalfFov(fov), 0.0f, 0.0f,
        0.0f, 0.0f, ProjectionA(nearClip, farClip), ProjectionB(nearClip, farClip),
        0.0f, 0.0f, 1.0f, 0.0f
    ){}
};

struct ScaleMatrix : Matrix4f {
    ScaleMatrix(float scaleAll = 1.0f) : ScaleMatrix(scaleAll, scaleAll, scaleAll){}

    ScaleMatrix(float scaleX, float scaleY, float scaleZ) : Matrix4f(
        scaleX, 0.0f, 0.0f, 0.0f,
        0.0f, scaleY, 0.0f, 0.0f,
        0.0f, 0.0f, scaleZ, 0.0f,
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
    TranslationMatrix(float offsetX = 0.0f, float offsetY = 0.0f, float offsetZ = 0.0f) : Matrix4f(
        1.0f, 0.0f, 0.0f, offsetX,
        0.0f, 1.0f, 0.0f, offsetY,
        0.0f, 0.0f, 1.0f, offsetZ,
        0.0f, 0.0f, 0.0f, 1.0f
    ) {}
};

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