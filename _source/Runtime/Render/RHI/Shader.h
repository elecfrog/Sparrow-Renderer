#pragma once


#include "Core/Log/LogSystem.h"
#include "OpenGL.h"

namespace Sparrow
{
    struct ShaderProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
        std::string tescSource;
        std::string teseSource;
    };

    class Shader
    {
        using fspath = std::filesystem::path;

    private:
        unsigned int ID = 0;

        std::filesystem::path m_filePath;

        // caching for uniforms
        mutable std::unordered_map<std::string, int> m_locationCache;

    public:
        mutable std::filesystem::path vertPath;
        mutable std::filesystem::path fragPath;
        mutable std::filesystem::path geomPath;
        mutable std::filesystem::path tescPath;
        mutable std::filesystem::path tesePath;

        enum ShaderType
        {
            NONE = 0,
            VERTEX = GL_VERTEX_SHADER,
            Tesc = GL_TESS_CONTROL_SHADER,
            Tese = GL_TESS_EVALUATION_SHADER,
            FRAGMENT = GL_FRAGMENT_SHADER,
            Geometry = GL_GEOMETRY_SHADER,
        };

        // Constructor generates the Shader Class on the fly
        // ------------------------------------------------------------------------
        Shader() = default;

        // Read Single File
        Shader(const std::filesystem::path& filepath);

        // Read Multiple Files

        /*
         * Common Shader Function
         * */
        Shader(const fspath& vertexPath, const fspath& fragmentPath)
            : vertPath(vertexPath), fragPath(fragmentPath)
        {
            // Get Shader Strings
            std::string vertexCode = ParseShader(VERTEX, vertexPath);
            std::string fragmentCode = ParseShader(FRAGMENT, fragmentPath);

            CreateShaderProgram(vertexCode, fragmentCode);
        }

        /*
         * Shader with Tessellation types
         * */
        Shader(fspath vertPath, fspath fragPath, fspath tescPath, fspath tesePath);

        // Read Multiple Files
        Shader(const fspath& vertexPath, const fspath& fragmentPath,
               const fspath& geometryPath)
            : vertPath(vertexPath), fragPath(fragmentPath), geomPath(geometryPath)
        {
            // Get Shader Strings
            std::string vertexCode = ParseShader(VERTEX, vertexPath);
            std::string fragmentCode = ParseShader(FRAGMENT, fragmentPath);

            std::string geometryCode = ParseShader(Geometry, geometryPath);

            CreateShaderProgram(vertexCode, fragmentCode, geometryCode);
        }

        inline unsigned int getShaderId() const { return ID; }


        std::string ParseShader(ShaderType shaderType, const fspath& file_path)
        {
            // 1. retrieve the vertex/fragment source code from filePath
            std::string shaderCode;
            std::ifstream curr_file;
            // ensure ifstream objects can throw exceptions:
            curr_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                // open files
                curr_file.open(file_path);
                std::stringstream file_stream;
                // read file's buffer contents into streams
                file_stream << curr_file.rdbuf();
                // close file handlers
                curr_file.close();
                // convert stream into string
                shaderCode = file_stream.str();
            }
            catch (std::ifstream::failure& e)
            {
                LOG_ERROR(Sparrow::LogModule::Asset, "Shader::{} FILE_NOT_SUCCESSFULLY_READ: {}", (UInt)shaderType, e.what());
            }

            return shaderCode;
        }

        void CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode)
        {
            unsigned int vertex = ReadAndCompileShader(VERTEX, vertexCode);
            unsigned int fragment = ReadAndCompileShader(FRAGMENT, fragmentCode);
            this->ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            CheckShaderProgram(ID, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void
        CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode, const std::string& tescCode,
                            const std::string& teseCode)
        {
            unsigned int vertShaderId = ReadAndCompileShader(VERTEX, vertexCode);
            unsigned int fragShaderId = ReadAndCompileShader(FRAGMENT, fragmentCode);
            unsigned int tescShaderId = ReadAndCompileShader(Tesc, tescCode);
            unsigned int teseShaderId = ReadAndCompileShader(Tese, teseCode);

            this->ID = glCreateProgram();
            GLCall(glAttachShader(ID, vertShaderId));
            GLCall(glAttachShader(ID, fragShaderId));
            GLCall(glAttachShader(ID, tescShaderId));
            GLCall(glAttachShader(ID, teseShaderId));

            GLCall(glLinkProgram(ID));
            CheckShaderProgram(ID, "PROGRAM");

            glDeleteShader(vertShaderId);
            glDeleteShader(fragShaderId);
            glDeleteShader(tescShaderId);
            glDeleteShader(teseShaderId);
        }

        void CreateShaderProgram(const std::string& vertexCode, const std::string& fragmentCode,
                                 const std::string& geometryCode)
        {
            unsigned int vertex = ReadAndCompileShader(VERTEX, vertexCode);
            unsigned int fragment = ReadAndCompileShader(FRAGMENT, fragmentCode);
            unsigned int geometry = ReadAndCompileShader(Geometry, geometryCode);

            this->ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glAttachShader(ID, geometry);
            glLinkProgram(ID);
            CheckShaderProgram(ID, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
            glDeleteShader(geometry);
        }

        unsigned int ReadAndCompileShader(ShaderType shaderType, const std::string& source);

        void CheckShaderCompileErrors(unsigned int shaderId, ShaderType shaderType);

        ~Shader();

        // Activates the Shader Program
        Shader& Bind();

        void Unbind() const;


        /***********************************************
        * Utility functions to help to set uniform values
        ************************************************/
        Shader& SetUniform1i(const std::string& name, int value);

        Shader& SetUniform1u(const std::string& name, uint32_t value);

        Shader& SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

        Shader& SetUniform4f(const std::string& name, glm::vec4 _vec);

        Shader& SetUniform3f(const std::string& name, Vector3f _vec);

        Shader& SetUniform3f(const std::string& name, glm::vec4 _vec);

        Shader& SetUniform1f(const std::string& name, float _vec);

        Shader& SetUniform3f(const std::string& name, float _x, float _y, float _z);

        Shader& SetUniformMat4f(const std::string& name, glm::mat4& matrix);
        Shader& SetUniformMat4f(const std::string& name, Matrix4x4& matrix);

        Shader& SetUniformMat3f(const std::string& name, glm::mat3& matrix);

        Shader& SetUniformVec1i(const std::string& name, std::vector<int>& vector);

    private:
        /***********************************************
        * Parse Shader Text File from the Hard Disk to Vertex Shader Strings and Fragment Shader Strings
        ************************************************/
        ShaderProgramSource ParseShader(const fspath& filePath);

        /***********************************************
        * Compile Different Kinds of Shader Strings
        ************************************************/
        unsigned int CompileShader(unsigned int type, const std::string& source);

        /***********************************************
        * Create Shader Program
        ************************************************/
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

        int GetUniformLocation(const std::string& name) const;

        // utility function for checking shaderId compilation/linking errors.
        // ------------------------------------------------------------------------
        void CheckShaderProgram(unsigned int shaderId, std::string type);
    };

    static SharedPtr<Shader> ReloadShader(const std::shared_ptr<Shader>& shader)
    {
        return std::make_shared<Shader>(shader->vertPath, shader->fragPath);
    }
}
