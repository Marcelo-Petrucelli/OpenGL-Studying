#include "m_main.h"

TempGraphicsData graphicsData;

static void RenderSceneCB(){
    glClear(GL_COLOR_BUFFER_BIT);

    Matrix4f cameraMatrix = (*(graphicsData.camera)).getMatrix();

    (*(graphicsData.transform)).setPosition(Vector3f(0.0f, 0.0f, 3.0f));
    (*(graphicsData.transform)).rotateBy(Vector3f(0.0f, 1.0f, 0.0f));

    Matrix4f objectTransformMatrix = (*(graphicsData.transform)).getMatrix();
    
    Matrix4f WVP = cameraMatrix * objectTransformMatrix;

    glUniformMatrix4fv(graphicsData.gUniformTransformMatrixID, 1, GL_TRUE, &WVP.m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, graphicsData.vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graphicsData.indexBuffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutPostRedisplay();
    glutSwapBuffers();
}

static void KeyboardCB(unsigned char key, int mouse_x, int mouse_y){
    (*(graphicsData.camera)).onKeyboard(key);
}

static void SpecialKeyboardCB(int key, int mouse_x, int mouse_y){
    (*(graphicsData.camera)).onKeyboard(key);
}

int main(int argc, char **argv) {
    srand(getpid());

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

    float width = 1500.0f;
    float height = 900.0f;
    glutInitWindowSize(width, height);

    int x = (2560/2)-(width/2);
    int y = (1080/2)-(height/2);
    glutInitWindowPosition(x, y);

    int winID = glutCreateWindow("OpenGL Learning");

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Blue = 0.0f, Green = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    Camera cam = Camera(
        width/height,
        Vector3f(0.0f, 0.0f, -1.0f),
        Vector2f(90.0f, 90.0f),
        Vector2f(1.0f, 10.0f),
        1.0f
    );
    Transform trans = Transform();
    graphicsData = TempGraphicsData(&cam, &trans);

    CreateVertexBuffer(&graphicsData);
    CreateIndexBuffer(&graphicsData);

    CompileShaders(&graphicsData);

    glutDisplayFunc(RenderSceneCB);
    glutKeyboardFunc(KeyboardCB);
    glutSpecialFunc(SpecialKeyboardCB);

    glutMainLoop();

    /*GLenum err = glewInit();
    if (GLEW_OK != err) {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    getchar();*/

    return 0;
}