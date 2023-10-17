#include "Shader.h"

Shader::Shader(const std::filesystem::path &filepath)
        : m_filePath(filepath) {
    ShaderProgramSource source = ParseShader(m_filePath);
    ID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(ID));
}

Shader &Shader::Bind() {
    GLCall(glUseProgram(ID))
    return *this;
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

/***********************************************
* Utility functions to help to set uniform values
************************************************/
Shader &Shader::SetUniform1i(const std::string &name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value))
    return *this;
}

Shader &Shader::SetUniform1u(const std::string &name, uint32_t value) {
    GLCall(glUniform1i(GetUniformLocation(name), value))
    return *this;
}

Shader &Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3))
    return *this;
}

Shader &Shader::SetUniform4f(const std::string &name, glm::vec4 _vec) {
    GLCall(glUniform4f(GetUniformLocation(name), _vec.x, _vec.y, _vec.z, _vec.w))
    return *this;
}

Shader &Shader::SetUniform3f(const std::string &name, glm::vec3 _vec) {
    GLCall(glUniform3f(GetUniformLocation(name), _vec.x, _vec.y, _vec.z))
    return *this;
}

Shader &Shader::SetUniform3f(const std::string &name, glm::vec4 _vec) {
    GLCall(glUniform3f(GetUniformLocation(name), _vec.x, _vec.y, _vec.z))
    return *this;
}

Shader &Shader::SetUniform1f(const std::string &name, float _vec) {
    GLCall(glUniform1f(GetUniformLocation(name), _vec))
    return *this;
}

Shader &Shader::SetUniform3f(const std::string &name, float _x, float _y, float _z) {
    GLCall(glUniform3f(GetUniformLocation(name), _x, _y, _z))
    return *this;
}

Shader &Shader::SetUniformMat4f(const std::string &name, glm::mat4 &matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]))
    return *this;
}

Shader &Shader::SetUniformMat3f(const std::string &name, glm::mat3 &matrix) {
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]))
    return *this;
}

Shader &Shader::SetUniformVec1i(const std::string &name, std::vector<int> &vector) {
    GLCall(glUniform1iv(GetUniformLocation(name), GLsizei(vector.size()), &vector[0]))
    return *this;
}

/***********************************************
* Parse Shader Text File from the Hard Disk to Vertex Shader Strings and Fragment Shader Strings
************************************************/
ShaderProgramSource Shader::ParseShader(const std::filesystem::path &filePath) {
    enum class ShaderType {
        none = -1, vertex = 0, fragment = 1
    };

    std::ifstream stream(filePath);
    if (!stream)
        exit(EXIT_FAILURE);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::none;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::vertex;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::fragment;
        } else {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    return {ss[0].str(), ss[1].str()};
}

/***********************************************
* Compile Different Kinds of Shader Strings
************************************************/
unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
    unsigned int shader;
    GLCall(shader = glCreateShader(type));

    const char *src = source.c_str();
    GLCall(glShaderSource(shader, 1, &src, nullptr));
    GLCall(glCompileShader(shader));

    int result;
    GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length));
        char *message = static_cast<char *>(alloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(shader, length, nullptr, message));

        std::cerr << "Error: Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
        std::cerr << message << std::endl;

        GLCall(glDeleteShader(shader));
        return 0;
    }

    return shader;
}

/***********************************************
* Create Shader Program
************************************************/
unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program;
    GLCall(program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string &name) const {
    if (const auto cache = m_locationCache.find(name); cache != m_locationCache.end())
        return cache->second;

    int location;
    GLCall(location = glGetUniformLocation(ID, name.c_str()));
    m_locationCache[name] = location;

    if (location == -1) {
        SPW_WARN("Shader: {0}, {1} \n \t Uniform {2} doesn't used!", vertPath.string(), fragPath.string(), name);
    }

    return location;
}

Shader::Shader(fspath vertPath, fspath fragPath, fspath tescPath, fspath tesePath)
        : vertPath(std::move(vertPath)), fragPath(std::move(fragPath)), tescPath(std::move(tescPath)),
          tesePath(std::move(tesePath)) {

    // Get Shader Strings
    std::string vertCode = ParseShader(ShaderType::VERTEX, this->vertPath);
    std::string fragCode = ParseShader(ShaderType::FRAGMENT, this->fragPath);
    std::string tescCode = ParseShader(ShaderType::Tesc, this->tescPath);
    std::string teseCode = ParseShader(ShaderType::Tese, this->tesePath);

    CreateShaderProgram(vertCode, fragCode, tescCode, teseCode);
}

void Shader::CheckShaderCompileErrors(unsigned int shaderId, Shader::ShaderType shaderType) {
    int success;
    char infoLog[1024];
    if (shaderType != ShaderType::NONE) {
        GLCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success))
        if (!success) {
            glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
            SPW_ERROR("SHADER_COMPILATION_ERROR of type: {} \n {} \n ", shaderType, infoLog);
        }
    }
}

void Shader::CheckShaderProgram(unsigned int shaderId, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
            SPW_ERROR("SHADER_COMPILATION_ERROR of type: {} \n infoLog \n {}", type, infoLog);
        }
    } else {
        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderId, 1024, nullptr, infoLog);
            SPW_ERROR("SHADER_COMPILATION_ERROR of type: {} \n infoLog \n {}", type, infoLog);
        }
    }
}

unsigned int Shader::ReadAndCompileShader(Shader::ShaderType shaderType, const std::string &source) {
    unsigned int shaderId = glCreateShader(shaderType);

    const char *src = source.c_str();
    GLCall(glShaderSource(shaderId, 1, &src, nullptr))
    GLCall(glCompileShader(shaderId))
    CheckShaderCompileErrors(shaderId, shaderType);

    return shaderId;
}
