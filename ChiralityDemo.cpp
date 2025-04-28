#include <iostream>
#include "inc/Scene.h"

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //gCamera.Scale(-yoffset * 5.0);
}

int main(int argc, char **argv) {
    Scene test_scene("test");
    if (!test_scene.Initialize())
    {
        return -1;
    }
    test_scene.BeginRender();
    return 0;
}
