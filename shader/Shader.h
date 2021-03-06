 #pragma once
#include <string>
#include <ios>
#include <fstream>
#include <string>

#define GL_GLEXT_PROTOTYPES
#include "glfw/glfw3.h"

#include "CompilationResult.h"

class Shader {
private:
    std::string path;
    std::string code;
    std::string logMessage;
    GLenum shaderType;
    GLuint shaderId;

    CompilationResult checkResults();

public:
    Shader(std::string &path, GLenum shaderType);
    CompilationResult compile();
    GLuint getShaderId();
};