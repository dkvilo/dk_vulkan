/**
 *  $File: main.c
 *  $By: David Kviloria (dkvilo) & SKYSTAR GAMES Interactive david@skystargames.com
 *  $Created: 2025-03-21 16:31:43
 *  $Modified: 2025-03-27 03:46:47
 */
#define DK_VULKAN_IMPLEMENTATION
#include "dk_vulkan.h"

int main()
{
  DK_vkApplication app = { 0 };

  int32_t windowWidth  = 800;
  int32_t windowHeight = 600;

  DK_vkInitApp( &app, windowWidth, windowHeight, "MoltenVK Renderer" );
  DK_vkInitTextureSystem( &app );

  uint32_t texture0SamplerId = DK_vkAddTexture( &app, "res/textures/uv.jpg" );
  uint32_t texture1SamplerId = DK_vkAddTexture( &app, "res/textures/Vulkan_logo.png" );

  DK_vkFont font = DK_vkLoadFont( &app, "res/fonts/Alegreya-Regular.ttf", 120.0f );

  double current = glfwGetTime();
  double prev    = current;
  double delta   = 0.0;

  float scaleFactorX, scaleFactorY;
  DK_vkGetWindowScale( &app, &scaleFactorX, &scaleFactorY );

  while ( !glfwWindowShouldClose( app.window ) )
  {
    double mouseX, mouseY;
    glfwGetCursorPos( app.window, &mouseX, &mouseY );

    mouseX *= scaleFactorX;
    mouseY *= scaleFactorX;

    float aspectRatio = app.screenWidth / app.screenHeight;

    current = glfwGetTime();
    delta   = current - prev;

    glfwPollEvents();
    DK_vkUpdateUniformBuffer( &app );

    DK_vkBeginBatch( &app );

    int32_t col = 250, row = 250;
    int32_t cell_size = 64;
    for ( int32_t y = 0; y < row; y++ )
    {
      for ( int32_t x = 0; x < col; x++ )
      {
        DK_vkVec2  position = { x * cell_size, y * cell_size };
        DK_vkColor color    = { 1.0f, (float)( x + y ) / 20.0f, 1.0f, 1.0f };

        DK_vkDrawRectangle( &app, position, (DK_vkVec2){ cell_size, cell_size }, color );
      }
    }

    {
      DK_vkVec2  p1   = { app.screenWidth * 0.5f, 0.0f };
      DK_vkVec2  p2   = { app.screenWidth, app.screenHeight };
      DK_vkVec2  p3   = { 0.0f, app.screenHeight };
      DK_vkColor tint = { 1.0, 1.0, 0.0, 1.0 };
      DK_vkDrawTriangle( &app, p1, p2, p3, tint );
    }

    {
      DK_vkColor tint      = { 0.9f, 0.6f, 0.3f, 0.8f };
      DK_vkSize  size      = { 100.0f, 100.0f };
      DK_vkVec2  position  = { ( app.screenWidth - size[0] ) * 0.5, 100.0f };
      float      roundness = 20.0f;
      int32_t    segments  = 128;
      DK_vkDrawRoundedRectangle( &app, position, size, roundness, tint, segments );
    }

    {
      DK_vkVec2 position = { 200.0f, 200.0f };
      DK_vkSize size     = { 400.0f, 400.0f };

      DK_vkColor tint_bl = { 1.0f, 0.0f, 0.0f, 1.0f };
      DK_vkColor tint_br = { 0.0f, 1.0f, 0.0f, 1.0f };
      DK_vkColor tint_tl = { 0.0f, 0.0f, 1.0f, 1.0f };
      DK_vkColor tint_tr = { 0.0f, 0.0f, 0.0f, 1.0f };
      DK_vkDrawGradientRectangle( &app, position, size, tint_bl, tint_br, tint_tr, tint_tl );
    }

    {
      float scale = 1.0f;

      DK_vkTexture texture = app.textures[texture0SamplerId];

      DK_vkColor tint     = { 1.0f, 1.0f, 1.0f, 1.0f };
      DK_vkSize  size     = { texture.width * scale, texture.height * scale };
      DK_vkVec2  position = { ( app.screenWidth - ( size[0] + 20.0f ) ),
                              ( app.screenHeight - ( size[1] + 20.0f ) ) };

      DK_vkDrawTexture( &app, position, texture0SamplerId, scale, tint );
    }

    {
      float        scale   = 1.0f;
      DK_vkTexture texture = app.textures[texture1SamplerId];

      DK_vkColor tint     = { 1.0f, 1.0f, 1.0f, 1.0f };
      DK_vkSize  size     = { texture.width * scale, texture.height * scale };
      DK_vkVec2  position = { ( ( 20.0f ) ), ( app.screenHeight - ( size[1] + 20.0f ) ) };

      DK_vkDrawTexture( &app, position, texture1SamplerId, scale, tint );
    }

    {
      char buffer[64];
      if ( delta > 0.0001 )
      {
        sprintf( buffer, "ms: %.2f, fps: %.2f", delta * 1000.0, 1.0 / delta );
      }
      else
      {
        sprintf( buffer, "ms: 0.00, fps 0.00" );
      }

      float      fontSize    = 40.0f * scaleFactorX;
      DK_vkVec2  position    = { 20, 10 };
      float      padding     = 20.0f;
      float      bgRoundness = 20.0f;
      int32_t    bgSegments  = 128;
      DK_vkColor bgTint      = { 0.0f, 0.0f, 0.0f, 0.8f };
      DK_vkSize  bgSize      = {
          DK_vkMeasureTextWidth( &font, buffer, fontSize ) + padding,
          ( DK_vkMeasureTextHeight( &font, buffer, fontSize ) * 0.5 ) + padding,
      };

      DK_vkVec2 bgPos = { position[0] - (padding * 0.5f), position[1] };
      DK_vkDrawRoundedRectangle( &app, bgPos, bgSize, bgRoundness, bgTint, bgSegments );

      DK_vkVec2  textPos        = { position[0] + 2, position[1] + 2 };
      DK_vkColor textTint       = { 0.0f, 1.0f, 0.0f, 1.0f };
      DK_vkColor textShadowTint = { 0.0f, 0.0f, 0.0f, 1.0f };

      DK_vkDrawText( &app, &font, buffer, position, fontSize, textShadowTint ); // @shadow
      DK_vkDrawText( &app, &font, buffer, textPos, fontSize, textTint );
    }

    DK_vkColor tint     = { 1.0f, 0.0f, 0.0f, 1.0f };
    DK_vkVec2  position = { mouseX, mouseY };
    {
      DK_vkDrawCircle( &app, position, 10.0f, tint, 32 );
    }

    float fontSize = 60.0f * scaleFactorX;
    DK_vkDrawText( &app, &font, "Hello World", position, fontSize, tint );
    float width  = DK_vkMeasureTextWidth( &font, "Hello World", fontSize );
    float height = DK_vkMeasureTextHeight( &font, "Hello World", fontSize );

    DK_vkEndBatch( &app );

    prev = current;
  }

  return 0;
}
