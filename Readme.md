# DK_VULKAN Renderer

This is 2D batch renderer implemented on top of Vulkan 1.2 (via MoltenVK) that runs on Mac

Supported feature

- TrueType font rendering
- Primitive Shapes (Rectangle, Line, Circle, Triangle)
- Textures and Texture Regions

# Full Screen Triangle Example

```c
#define DK_VULKAN_IMPLEMENTATION
#include "dk_vulkan.h"

int main()
{
  DK_vkApplication app = { 0 };
  DK_vkInitApp( &app, 800, 600, "MoltenVK Renderer" );
  while ( !glfwWindowShouldClose( app.window ) )
  {
    glfwPollEvents();
    DK_vkUpdateUniformBuffer( &app );

    DK_vkBeginBatch( &app );

      DK_vkVec2  p1   = { app.screenWidth * 0.5f, 0.0f };
      DK_vkVec2  p2   = { app.screenWidth, app.screenHeight };
      DK_vkVec2  p3   = { 0.0f, app.screenHeight };
      DK_vkColor tint = { 1.0, 1.0, 0.0, 1.0 };
      DK_vkDrawTriangle( &app, p1, p2, p3, tint );

    DK_vkEndBatch( &app );
  }

  return 0;
}

```

# Building

It is expected to have installed glfw3 and vulkan 1.2 sdk, if you are on arm64, you can use libraries that are included in libs/ folder, but you will still need to install glfw3 and glsl utils. for more details check Makefile.

# Sample Program

![Screenshot](/res/screenshot.png)
