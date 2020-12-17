#include "simple_9_slice.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <string>
#include <array>

#ifdef NDEBUG
#define RESOURCE_PATH RELEASE_RESOURCE_PATH
#else
#define RESOURCE_PATH DEBUG_RESOURCE_PATH
#endif

#define INDICES_SIZE 6

SlicedTexture::SlicedTexture()
{
}

SlicedTexture::~SlicedTexture()
{
    glDeleteProgram(m_program);
    glDeleteTextures(1, &m_texture);
    glDeleteTextures(1, &m_frameTexture);
    glDeleteFramebuffers(1, &m_frameBuffer);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void SlicedTexture::init(int width, int height)
{
    m_frameWidth = width;
    m_frameHeight = height;
    m_sliceSize = (width / 10);

    createProgram();

    loadTexture();

    createFrameBuffer();

    createDataBuffers();
}

unsigned int SlicedTexture::getFrameTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_frameTexture);
    return m_frameTexture;
}

void SlicedTexture::draw(float imguiWindowWidth, float imguiWindowHeight)
{
    bind();

    unsigned int borderLoc = glGetUniformLocation(m_program, "u_border");
    glUniform2f(borderLoc, m_sliceSize / m_texWidth, m_sliceSize / m_texHeight);

    unsigned int dimensionLoc = glGetUniformLocation(m_program, "u_dimensions");
    glUniform2f(dimensionLoc, m_sliceSize / imguiWindowWidth, m_sliceSize / imguiWindowHeight);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, INDICES_SIZE, GL_UNSIGNED_INT, nullptr);

    unbind();
}

void SlicedTexture::bind()
{
    glUseProgram(m_program);
    glBindVertexArray(m_VAO);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
}

void SlicedTexture::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void SlicedTexture::loadTexture()
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int nrChannels;
    unsigned char* textureData = stbi_load(RESOURCE_PATH"/frame.png", &m_texWidth, &m_texHeight, &nrChannels, STBI_rgb_alpha);
    if (textureData)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texWidth, m_texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    else
        throw std::runtime_error("Cannot load image!");

    stbi_image_free(textureData);
}

void SlicedTexture::createFrameBuffer()
{
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glGenTextures(1, &m_frameTexture);
    glBindTexture(GL_TEXTURE_2D, m_frameTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_frameWidth, m_frameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_frameTexture, 0);
}

unsigned int SlicedTexture::createShader(const char* filename, unsigned int shaderType)
{
    std::ifstream file(filename);
    if (!file.good())
        throw std::runtime_error("Cannot open shader file!");

    std::stringstream shaderCode;
    shaderCode << file.rdbuf();
    file.close();

    std::string src = shaderCode.str();
    const char* rawSrc = src.c_str();

    unsigned int shader = glCreateShader(shaderType);
    if (shader == 0)
        throw std::runtime_error("Cannot create shader!");

    glShaderSource(shader, 1, &rawSrc, nullptr);
    glCompileShader(shader);

    int isSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccess);
    if (!isSuccess)
    {
        int msgLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgLen);
        char* errorMsg = (char*)alloca(msgLen * sizeof(char));
        glGetShaderInfoLog(shader, msgLen, &msgLen, errorMsg);
        glDeleteShader(shader);

        std::string error("Compilation error : ");
        error += std::string(errorMsg);
        throw std::runtime_error(error.c_str());
    }

    return shader;
}

void SlicedTexture::createProgram()
{
    unsigned int vertexShader = createShader(RESOURCE_PATH"/shader.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = createShader(RESOURCE_PATH"/shader.frag", GL_FRAGMENT_SHADER);
    
    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);
    glValidateProgram(m_program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!m_program)
        throw std::runtime_error("Cannot create program!");

    glUseProgram(m_program);
}

void SlicedTexture::createDataBuffers()
{    
    std::array<float, 16> vertices =
    {
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
    };
    std::array<unsigned int, INDICES_SIZE> indices = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}
