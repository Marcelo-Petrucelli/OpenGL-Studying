#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <dev_graphics.h>

GLuint vertexBuffer, indexBuffer;
GLint gTransformLocation;

const char* vertexShaderFileName = "shaders/shader.vs";
const char* fragmentShaderFileName = "shaders/shader.fs"; 

static void RenderSceneCB(){
    glClear(GL_COLOR_BUFFER_BIT);

    static float scale = 0.75f;
    static float delta = 0.001f;

    scale += delta;
    if((scale >= 0.8f) || (scale <= 0.5f)){
        delta *= -1.0f;
    }

    Matrix4f translation(
        scale, 0.0f, 0.0f, 0.0f,
        0.0f, scale, 0.0f, 0.0f,
        0.0f, 0.0f, scale, 0.0,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    glUniformMatrix4fv(gTransformLocation, 1, GL_TRUE, &translation.m[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void *)(3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, 18 * 3, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutPostRedisplay();
    glutSwapBuffers();
}

static void CreateVertexBuffer(){
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
    //glCullFace(GL_FRONT);

    Vertex vertices[19];

    vertices[0] = Vertex(Vector3f(0.0f, 0.0f)); //Center

    vertices[1] = Vertex(Vector3f(-1.00f, 1.0f));
    vertices[2] = Vertex(Vector3f(-0.75f, 1.0f));
    vertices[3] = Vertex(Vector3f(-0.50f, 1.0f));
    vertices[4] = Vertex(Vector3f(-0.25f, 1.0f));
    vertices[5] = Vertex(Vector3f( 0.00f, 1.0f));
    vertices[6] = Vertex(Vector3f( 0.25f, 1.0f));
    vertices[7] = Vertex(Vector3f( 0.50f, 1.0f));
    vertices[8] = Vertex(Vector3f( 0.75f, 1.0f));
    vertices[9] = Vertex(Vector3f( 1.00f, 1.0f));

    vertices[10] = Vertex(Vector3f(-1.00f, -1.0f));
    vertices[11] = Vertex(Vector3f(-0.75f, -1.0f));
    vertices[12] = Vertex(Vector3f(-0.50f, -1.0f));
    vertices[13] = Vertex(Vector3f(-0.25f, -1.0f));
    vertices[14] = Vertex(Vector3f( 0.00f, -1.0f));
    vertices[15] = Vertex(Vector3f( 0.25f, -1.0f));
    vertices[16] = Vertex(Vector3f( 0.50f, -1.0f));
    vertices[17] = Vertex(Vector3f( 0.75f, -1.0f));
    vertices[18] = Vertex(Vector3f( 1.00f, -1.0f));

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void CreateIndexBuffer(){
    unsigned int indices[] = {
        //Top Triangles
        0,2,1,
        0,3,2,
        0,4,3,
        0,5,4,
        0,6,5,
        0,7,6,
        0,8,7,
        0,9,8,

        //Bottom Triangle
        0,10,11,
        0,11,12,
        0,12,13,
        0,13,14,
        0,14,15,
        0,15,16,
        0,16,17,
        0,17,18,

        //Side Triangles
        0,1,10,
        0,18,9
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

    gTransformLocation = glGetUniformLocation(shaderProgramHandle, "gTransform");
    if(gTransformLocation == -1){
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

    int width = 1200;
    int height = 800;
    glutInitWindowSize(width, height);

    int x = (2560/2)-(1200/2);
    int y = (1080/2)-(800/2);
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