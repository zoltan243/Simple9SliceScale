#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <glm/glm.hpp>

struct GLFWwindow;

class MyWindow
{
public:
    MyWindow();
    ~MyWindow();
    void init(int width, int height);
    bool shouldClose();
    glm::vec2 createImGuiWindow(unsigned int textureID);
    void update();
    void clear();

private:
    GLFWwindow* m_window;
};

#endif // !MY_WINDOW_H
