#include "m_main.h"
#include "m_graphics.h"

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

void CompileShaders(TempGraphicsData* graphicsData){
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

    GLint transformMatrixID = glGetUniformLocation(shaderProgramHandle, "gTransform");
    if(transformMatrixID == -1){
        fprintf(stderr, "Error while fetching uniform location of %s\n", "gTransform");
        getchar();
        exit(1);
    }
    (*graphicsData).gUniformTransformMatrixID = transformMatrixID;

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

//Should be moved to another class with object info?
void CreateVertexBuffer(TempGraphicsData* graphicsData){
    Vertex vertices[8];

    vertices[0] = Vertex(Vector3f( 0.5f,  0.5f,  0.5f));
    vertices[1] = Vertex(Vector3f( 0.5f,  0.5f, -0.5f));
    vertices[2] = Vertex(Vector3f( 0.5f, -0.5f,  0.5f));
    vertices[3] = Vertex(Vector3f( 0.5f, -0.5f, -0.5f));
    vertices[4] = Vertex(Vector3f(-0.5f,  0.5f,  0.5f));
    vertices[5] = Vertex(Vector3f(-0.5f,  0.5f, -0.5f));
    vertices[6] = Vertex(Vector3f(-0.5f, -0.5f,  0.5f));
    vertices[7] = Vertex(Vector3f(-0.5f, -0.5f, -0.5f));

    GLuint* vertexBuffer = &((*graphicsData).vertexBuffer);
    glGenBuffers(1, vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void CreateIndexBuffer(TempGraphicsData* graphicsData){
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

    GLuint* indexBuffer = &((*graphicsData).indexBuffer);
    glGenBuffers(1, indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}