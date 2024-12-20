#include <dev_math.h>

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