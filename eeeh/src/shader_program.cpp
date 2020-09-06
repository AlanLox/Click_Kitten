#include <shader_program.h>

#include <fstream>   // ifstream
#include <sstream>   // stringstream
#include <stdexcept> // runtime_error
#include <vector>    // vector

using namespace std;

static GLuint CompileShader(const string& path, GLenum shaderType) {
    GLuint id = 0;
    try {
        // Получаем идентификатор
        id = glCreateShader(shaderType);
        if (id == 0) {
            stringstream ess;
            ess << "Invalid shader type for \"" << path << "\"!";
            throw runtime_error(ess.str().c_str());
        }

        // Загружаем шейдер
        string shader;
        {
            ifstream fs(path);
            stringstream ss;
            ss << fs.rdbuf();
            shader = ss.str();
            if (fs.fail()) {
                stringstream ess;
                ess << "Failed to load shader \"" << path << "\"!";
                throw runtime_error(ess.str().c_str());
            }
        }

        // Компилируем шейдер
        auto shaderPtr = shader.c_str();
        glShaderSource(id, 1, &shaderPtr, nullptr);
        glCompileShader(id);

        // Проверяем наличие ошибок
        GLint success = GL_FALSE;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            vector<GLchar> log(length);
            glGetShaderInfoLog(id, length, nullptr, log.data());
            stringstream ess;
            ess << "Failed to compile shader \"" << path << "\"!" << endl;
            ess << log.data();
            throw runtime_error(ess.str().c_str());
        }

        return id;
    }
    catch (...) {
        if (id != 0) {
            glDeleteShader(id);
        }
        throw;
    }
}

ShaderProgram::ShaderProgram(initializer_list<pair<string, GLenum>> shaders) : id(glCreateProgram()) {
    if (id == 0)
        throw runtime_error("Failed to create shader program!");

    vector<GLuint> shaderIds;
    shaderIds.reserve(shaders.size());
    try {
        for (auto& shader : shaders) {
            const auto shaderId = CompileShader(shader.first, shader.second);
            glAttachShader(id, shaderId);
            shaderIds.push_back(shaderId);
        }
        glLinkProgram(id);

        GLint success = GL_FALSE;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
            vector<GLchar> log(length);
            glGetProgramInfoLog(id, length, nullptr, log.data());
            stringstream ess;
            ess << "Failed to link program!" << endl;
            ess << log.data();
            throw runtime_error(ess.str().c_str());
        }

        for (auto shaderId : shaderIds)
            glDeleteShader(shaderId);
    }
    catch (...) {
        for (auto shaderId : shaderIds)
            glDeleteShader(shaderId);
        glDeleteProgram(id);
        throw;
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteShader(id);
}

//добавил юз
void ShaderProgram::Use() { glUseProgram(this->id); }