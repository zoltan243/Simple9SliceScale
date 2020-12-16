// 9slice shader source: https://www.gamedev.net/forums/topic/696879-glsl-9-slicing/

#include "my_window.h"
#include "simple_9_slice.h"

#include <exception>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

int main()
{
    MyWindow myWindow;
    SlicedTexture texture;

    try
    {
        myWindow.init(WIDTH, HEIGHT);
        texture.init(WIDTH, HEIGHT);
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
        return EXIT_FAILURE;
    }

	while (!myWindow.shouldClose())
	{
        myWindow.clear();
        glm::vec2 imguiWindowSize = myWindow.createImGuiWindow(texture.getFrameTexture());

        texture.draw(imguiWindowSize.x, imguiWindowSize.y);

        myWindow.update();
	}

	return EXIT_SUCCESS;
}
