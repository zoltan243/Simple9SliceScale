#ifndef SIMPLE_9_SLICE_H
#define SIMPLE_9_SLICE_H

class SlicedTexture
{
public:
    SlicedTexture();
    ~SlicedTexture();
    void init(int width, int height);
    unsigned int getFrameTexture();
    void draw(float imguiWindowWidth, float imguiWindowHeight);

private:
    void loadTexture();
    void createFrameBuffer();
    void bind();
    void unbind();
    unsigned int createShader(const char* filename, unsigned int shaderType);
    void createProgram();
    void createDataBuffers();

private:
    float m_sliceSize; // ~ 10% of the frame width

    unsigned int m_texture;
    int m_texWidth, m_texHeight;

    unsigned int m_frameBuffer;
    unsigned int m_frameTexture;
    int m_frameWidth, m_frameHeight; // these are the same as GLFWwindow width and height

    unsigned int m_program;
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_EBO;
};

#endif // !SIMPLE_9_SLICE_H
