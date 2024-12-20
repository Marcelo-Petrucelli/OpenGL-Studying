#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dev_math.h>

GLuint VBO;
GLint gScaleLocation;
const char* vertexShaderFileName = "shaders/shader.vs";
const char* fragmentShaderFileName = "shaders/shader.fs"; 

static void RenderSceneCB(){
    glClear(GL_COLOR_BUFFER_BIT);

    static float scale = 0.0f;
    static float delta = 0.005f;

    scale += delta;
    if((scale >= 1.0f) || (scale <= -1.0f)){
        delta *= -1.0f;
    }
    glUniform1f(gScaleLocation, scale);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);

    //glutPostRedisplay();
    glutSwapBuffers();
}

static void CreateVertexBuffer(){
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    //glCullFace(GL_FRONT);

    Vector3f vertices[3];
    vertices[0] = Vector3f(1.0f, -1.0f, 0.0f);
    vertices[1] = Vector3f(0.0f, 1.0f, 0.0f);
    vertices[2] = Vector3f(-1.0f, -1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void AddShader(GLuint shaderProgramHandle, const char* shaderCode, GLenum shaderType){
    const GLchar *glShaderText[1];
    GLint length[1];
    GLint success;

    GLuint shaderHandler = glCreateShader(shaderType);

    if(shaderHandler == 0){
        fprintf(stderr, "Error while creating shader type %d!\n", shaderType);
        getchar();
        exit(1);
    }

    glShaderText[0] = shaderCode;
    length[0] = strlen(shaderCode);

    glShaderSource(shaderHandler, 1, glShaderText, length);
    glCompileShader(shaderHandler);

    glGetShaderiv(shaderHandler, GL_COMPILE_STATUS, &success);

    if(success == 0){
        GLchar infoLog[1024] = { 0 };
        glGetShaderInfoLog(shaderHandler, sizeof(infoLog), NULL, infoLog);

        fprintf(stderr, "Error while compiling shader type %d: '%s'\n", shaderType, infoLog);
        getchar();
        exit(1);
    }

    glAttachShader(shaderProgramHandle, shaderHandler);
}

static void CompileShaders(){
    std::string vertexShader, fragmentShader;
    GLuint shaderProgramHandle = glCreateProgram();
    GLint success;

    if(shaderProgramHandle == 0){
        fprintf(stderr, "Error while creating shader program!\n");
        getchar();
        exit(1);
    }
    
    if(!ReadFile(vertexShaderFileName, vertexShader)){
        fprintf(stderr, "Unable to load file %s!\n", vertexShaderFileName);
        getchar();
        exit(1);
    }
    if(!ReadFile(fragmentShaderFileName, fragmentShader)){
        fprintf(stderr, "Unable to load file %s!\n", fragmentShaderFileName);
        getchar();
        exit(1);
    }

    AddShader(shaderProgramHandle, vertexShader.c_str(), GL_VERTEX_SHADER);
    AddShader(shaderProgramHandle, fragmentShader.c_str(), GL_FRAGMENT_SHADER);

    glLinkProgram(shaderProgramHandle);
    glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &success);
    if(success == 0){
        GLchar infoLog[1024] = { 0 };
        glGetProgramInfoLog(shaderProgramHandle, sizeof(infoLog), NULL, infoLog);

        fprintf(stderr, "Error while linking shader program: '%s'\n", infoLog);
        getchar();
        exit(1);
    }

    gScaleLocation = glGetUniformLocation(shaderProgramHandle, "gScale");
    if(gScaleLocation == -1){
        fprintf(stderr, "Error while fetching uniform location of %s\n", "gScale");
    }

    glValidateProgram(shaderProgramHandle);
    glGetProgramiv(shaderProgramHandle, GL_VALIDATE_STATUS, &success);
    if(success == 0){
        GLchar infoLog[1024] = { 0 };
        glGetProgramInfoLog(shaderProgramHandle, sizeof(infoLog), NULL, infoLog);

        fprintf(stderr, "Error while validating shader program: '%s'\n", infoLog);
        getchar();
        exit(1);
    }

    glUseProgram(shaderProgramHandle);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

    int width = 1920;
    int height = 1080;
    glutInitWindowSize(width, height);

    int x = 0;
    int y = 0;
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

    CreateVertexBuffer();
    CompileShaders();

    glutDisplayFunc(RenderSceneCB);

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