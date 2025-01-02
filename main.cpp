#include "m_main.h"

TempGraphicsData graphicsData;

static void RenderSceneCB(){
    glClear(GL_COLOR_BUFFER_BIT);

    (*(graphicsData.camera)).onRender();
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

static void MouseMoveCB(int mouse_x, int mouse_y){
    (*(graphicsData.camera)).onMouse(mouse_x, mouse_y);
}

static float InitGlutWindow(const char* windowName, Vector2f size = Vector2f(-1.0f, -1.0f), bool fullScreen = false){
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

    int deviceScreenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int deviceScreenHeight = glutGet(GLUT_SCREEN_HEIGHT);

    if(size.x == -1.0f && size.y == -1.0f){
        size.x = deviceScreenWidth; //Forces x to be 0 below
        size.y = deviceScreenHeight; //Forces y to be 0 below
    }

    glutInitWindowSize(size.x, size.y);

    int x = (deviceScreenWidth/2)-(size.x/2);
    int y = (deviceScreenHeight/2)-(size.y/2);
    glutInitWindowPosition(x, y);

    int winID = glutCreateWindow("OpenGL Learning");

    if(fullScreen){
        char gameModeString[64];
        snprintf(gameModeString, sizeof(gameModeString), "%dx%d@32", deviceScreenWidth, deviceScreenHeight);
        glutGameModeString(gameModeString);
        glutEnterGameMode();
    }

    return size.x/size.y;
}

static void InitGlutCallbacks(){
    glutDisplayFunc(RenderSceneCB);
    glutKeyboardFunc(KeyboardCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(MouseMoveCB);
    glutMotionFunc(MouseMoveCB);
}

int main(int argc, char **argv) {
    srand(getpid());

    glutInit(&argc, argv);
    float aspectRatio = InitGlutWindow("OpenGL Learning", Vector2f(1500, 900));
    InitGlutCallbacks();

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        return 1;
    }

    GLclampf Red = 0.0f, Blue = 0.0f, Green = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    Camera cam = Camera(
        aspectRatio,
        Vector3f(0.0f, 0.0f, -1.0f),
        Vector2f(90.0f, 90.0f),
        Vector2f(1.0f, 10.0f),
        1.0f
    );
    Transform trans = Transform();
    graphicsData = TempGraphicsData(&cam, &trans);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW); //Because default Blender is CW
    glCullFace(GL_BACK);
    
    CreateVertexBuffer(&graphicsData);
    CreateIndexBuffer(&graphicsData);

    CompileShaders(&graphicsData);

    glutMainLoop();
    return 0;
}