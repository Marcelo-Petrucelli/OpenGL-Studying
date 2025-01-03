#include "m_main.h"

TempGraphicsData graphicsData;

static void RenderSceneCB(){
    glClear(GL_COLOR_BUFFER_BIT);

    graphicsData.camera->onRender();
    Matrix4f cameraMatrix;
    graphicsData.camera->getMatrix(&cameraMatrix);

    graphicsData.transform->setPosition(Vector3f(0.0f, 0.0f, 3.0f));
    graphicsData.transform->rotateBy(Vector3f(0.0f, 1.0f, 0.0f));

    Matrix4f worldTransformMatrix;
    graphicsData.transform->getMatrix(&worldTransformMatrix);
    
    Matrix4f WVP = cameraMatrix * worldTransformMatrix;

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

    glfwSwapBuffers(graphicsData.window);
    glfwPollEvents(); /* Poll for and process events */
}

static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    graphicsData.camera->setScreenRatio((float)width/(float)height);
}

static void KeyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods){
    graphicsData.camera->onKeyboard(key);
}

static void MouseMoveCB(GLFWwindow* window, double mouse_x, double mouse_y){
    graphicsData.camera->onMouse(mouse_x, mouse_y);
}

static GLFWwindow* InitGLFWWindow(const char* windowName, Vector2f size = Vector2f(-1.0f, -1.0f), bool fullScreen = false){
    GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();

    const GLFWvidmode * mode = glfwGetVideoMode(mainMonitor);
    int deviceScreenWidth = mode->width;
    int deviceScreenHeight = mode->height;

    if(size.x == -1.0f && size.y == -1.0f){
        size.x = deviceScreenWidth; //Forces x to be 0 below
        size.y = deviceScreenHeight; //Forces y to be 0 below
    }

    int x = (deviceScreenWidth/2) - (size.x/2);
    int y = (deviceScreenHeight/2) - (size.y/2);

    if(fullScreen){
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        glfwWindowHint(GLFW_POSITION_X, 0);
        glfwWindowHint(GLFW_POSITION_Y, 0);
    } else {
        glfwWindowHint(GLFW_POSITION_X, x);
        glfwWindowHint(GLFW_POSITION_Y, y);
    }

    GLFWwindow* window = glfwCreateWindow(size.x, size.y, windowName, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    //glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetWindowAttrib(window, GLFW_DOUBLEBUFFER, GLFW_TRUE); //mode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glfwSetWindowAttrib(window, GLFW_STEREO, GLFW_TRUE);

    /*GLFWimage images[2];
    images[0] = load_icon("my_icon.png");
    images[1] = load_icon("my_icon_small.png");
    
    glfwSetWindowIcon(window, 2, images);*/

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return window;
}

static void InitGLFWCallbacks(){
    glfwSetKeyCallback(graphicsData.window, KeyboardCB);
    glfwSetCursorPosCallback(graphicsData.window, MouseMoveCB);
    glfwSetFramebufferSizeCallback(graphicsData.window, FrameBufferSizeCallback);

    do {
        glClear(GL_COLOR_BUFFER_BIT);
        RenderSceneCB();
    } while( glfwGetKey(graphicsData.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(graphicsData.window) == 0 );
}

int main(int argc, char **argv) {
    srand(getpid());

    if (!glfwInit()){
        fprintf(stderr, "GLFWInit() Failed!\n");
        return -1;
    }

    GLFWwindow* window = InitGLFWWindow("OpenGL Learning", Vector2f(1500, 900));
    if (!window){
        fprintf(stderr, "InitGLFWWindow() Failed!\n");
        return -1;
    }

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(res));
        glfwTerminate();
        return 1;
    }

    GLclampf Red = 0.0f, Blue = 0.0f, Green = 0.0f, Alpha = 0.0f;
    glClearColor(Red, Green, Blue, Alpha);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW); //Because default Blender is CW
    glCullFace(GL_BACK);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    Camera cam = Camera(
        (float)width / (float)height,
        Vector3f(0.0f, 0.0f, -1.0f),
        Vector2f(90.0f, 90.0f),
        Vector2f(1.0f, 10.0f),
        1.0f
    );
    Transform trans = Transform();
    graphicsData = TempGraphicsData(window, &cam, &trans);

    CreateVertexBuffer(&graphicsData);
    CreateIndexBuffer(&graphicsData);

    CompileShaders(&graphicsData);

    InitGLFWCallbacks();

    glfwDestroyWindow(graphicsData.window);
    glfwTerminate();
    return 0;
}