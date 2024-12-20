#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dev_graphics.h>

float aspectRatio;
GLuint vertexBuffer, indexBuffer;
GLint gTransformMatrix;

const char* vertexShaderFileName = "shaders/shader.vs";
const char* fragmentShaderFileName = "shaders/shader.fs"; 

static void RenderSceneCB(){
    glClear(GL_COLOR_BUFFER_BIT);

    /*********************** TEMP ***********************/
    static float variableScale = 0.5f;
    static float scaleDelta = 0.001f;

    variableScale += scaleDelta;
    if((variableScale >= 0.6f) || (variableScale <= 0.3f)){
        scaleDelta *= -1.0f;
    }

    static float variableRotation = 0.0f;
    static float rotationDelta = 0.5f;

    variableRotation += rotationDelta;
    if(variableRotation >= 360.0f){
        variableRotation = 0.0f;
    }

    static float variableTranslation = 0.0f;
    static float translationDelta = 0.005f;

    variableTranslation += translationDelta;
    if((variableTranslation >= 1.5f) || (variableTranslation <= -1.5f)){
        translationDelta *= -1.0f;
    }
    /****************************************************/

    ScaleMatrix scaleMatrix; //(variableScale)
    RotationXMatrix rotateXMatrix(-35);
    RotationYMatrix rotateYMatrix(variableRotation); //(variableRotation)
    RotationZMatrix rotateZMatrix;
    TranslationMatrix translateMatrix(variableTranslation, 0.0f, 3.0f); //(variableTranslation)

    Matrix4f transform = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    ProjectionMatrix projection(90.0f, aspectRatio);

    Matrix4f finalMatrix = projection * transform;

    glUniformMatrix4fv(gTransformMatrix, 1, GL_TRUE, &finalMatrix.m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

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

static void CreateVertexBuffer(){
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW); //Because default Blender is CW
    glCullFace(GL_BACK);

    Vertex vertices[8];

    vertices[0] = Vertex(Vector3f( 0.5f,  0.5f,  0.5f));
    vertices[1] = Vertex(Vector3f( 0.5f,  0.5f, -0.5f));
    vertices[2] = Vertex(Vector3f( 0.5f, -0.5f,  0.5f));
    vertices[3] = Vertex(Vector3f( 0.5f, -0.5f, -0.5f));
    vertices[4] = Vertex(Vector3f(-0.5f,  0.5f,  0.5f));
    vertices[5] = Vertex(Vector3f(-0.5f,  0.5f, -0.5f));
    vertices[6] = Vertex(Vector3f(-0.5f, -0.5f,  0.5f));
    vertices[7] = Vertex(Vector3f(-0.5f, -0.5f, -0.5f));

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer(){
    unsigned int indices[] = {
        //Top Triangles
        0,5,4,
        5,3,7,
        1,2,3,
        6,3,2,
        4,7,6,
        0,6,2,
        0,1,5,
        5,1,3,
        1,0,2,
        6,7,3,
        4,5,7,
        0,4,6,
    };

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

    gTransformMatrix = glGetUniformLocation(shaderProgramHandle, "gTransform");
    if(gTransformMatrix == -1){
        fprintf(stderr, "Error while fetching uniform location of %s\n", "gTransform");
        getchar();
        exit(1);
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
    srand(getpid());

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

    float width = 1500.0f;
    float height = 900.0f;
    aspectRatio = width/height;
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

    CreateVertexBuffer();
    CreateIndexBuffer();

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