/**
 *  $File: dk_vulkan.h
 *  $By: David Kviloria (dkvilo) & SKYSTAR GAMES Interactive david@skystargames.com
 *  $Created: 2025-03-22 04:26:31
 *  $Modified: 2025-03-27 03:46:45
 */
#ifndef __dk_vulkan
#define __dk_vulkan

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define DK_VK_USE_GLFW

#ifdef DK_VK_USE_GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#ifdef DK_VULKAN_INLINE
#define DK_VULKAN_FUNC inline
#else
#define DK_VULKAN_FUNC static inline
#endif

  typedef float DK_vkVec2[2];
  typedef float DK_vkVec3[3];
  typedef float DK_vkVec4[4];

  typedef DK_vkVec4 DK_vkMat4[4];
  typedef DK_vkVec4 DK_vkColor;
  typedef DK_vkVec2 DK_vkSize;

  typedef struct
  {
    DK_vkMat4 model;
    DK_vkMat4 view;
    DK_vkMat4 proj;
  } DK_vkUniformBufferObject;

#define DK_VULKAN_MAX_FRAMES_IN_FLIGHT 2
#define DK_VK_DEFAULT_FONT_LINE_SPACEING 1.2
#define DK_VULKAN_DEFAULT_FONT_ATLAS_SIZE 1024

#define MAX_BATCH_VERTICES 500000
#define MAX_BATCH_INDICES 550000
#define DK_VK_MAX_TEXTURES 10
#define DK_VK_FONT_ATLAS_PADDING 1

  typedef struct DK_Vertex
  {
    DK_vkVec2  pos;
    DK_vkColor color;
    DK_vkVec2  texCoord;
    /* Note (david) when 0 is treated as no textured output */
    int32_t samplerId;
  } DK_Vertex;

  typedef struct
  {
    VkImage        image;
    VkDeviceMemory memory;
    VkImageView    view;
    VkSampler      sampler;
    int32_t        width;
    int32_t        height;
    int32_t        channels;
    bool           isActive;
    uint32_t       samplerId;
  } DK_vkTexture;

  typedef struct
  {
    VkBuffer       vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    DK_Vertex     *vertexBufferMapped;

    VkBuffer       indexBuffer;
    VkDeviceMemory indexBufferMemory;
    uint32_t      *indexBufferMapped;

    uint32_t vertexCount;
    uint32_t indexCount;

    VkCommandBuffer commandBuffer;
    bool            hasBegun;

    VkPipeline       pipeline;
    VkPipelineLayout pipelineLayout;

    DK_vkTexture *currentTexture;

  } DK_vkRenderer;

  typedef struct
  {
    float left;
    float right;
    float bottom;
    float top;
    float near;
    float far;
    float zoom;
  } DK_Camera;

  typedef struct
  {
    GLFWwindow *window;
    int32_t     screenWidth;
    int32_t     screenHeight;

    VkInstance               instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR             surface;

    VkPhysicalDevice physicalDevice;
    VkDevice         device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    VkImage       *swapChainImages;
    VkFormat       swapChainImageFormat;
    VkExtent2D     swapChainExtent;
    VkImageView   *swapChainImageViews;
    VkFramebuffer *swapChainFramebuffers;

    VkRenderPass     renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline       graphicsPipeline;

    VkCommandPool    commandPool;
    VkCommandBuffer *commandBuffers;

    VkSemaphore *imageAvailableSemaphores;
    VkSemaphore *renderFinishedSemaphores;
    VkFence     *inFlightFences;

    uint32_t imageCount;
    uint32_t currentFrame;
    bool     framebufferResized;

    VkBuffer       vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer       uniformBuffer;
    VkDeviceMemory uniformBufferMemory;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool      descriptorPool;
    VkDescriptorSet       descriptorSet;

    DK_vkRenderer batchRenderer;

    DK_vkTexture *textures;
    uint32_t      textureCount;
    uint32_t      maxTextures;
    uint32_t      activeTextureCount;

    DK_vkTexture *currentTexture;

    DK_Camera camera;
  } DK_vkApplication;

  typedef struct
  {
    unsigned char *bitmap;
    int32_t        width;
    int32_t        height;
    int32_t        xadvance;
    int32_t        xoff;
    int32_t        yoff;
  } DK_vkGlyphInfo;

  typedef struct
  {
    VkImage           image;
    VkDeviceMemory    memory;
    VkImageView       view;
    VkSampler         sampler;
    int32_t           width;
    int32_t           height;
    int32_t           channels;
    bool              isActive;
    uint32_t          samplerId;
    stbtt_packedchar *char_data;
    int32_t           base_size;
    int32_t           ascent;
    int32_t           descent;
    int32_t           line_gap;
  } DK_vkFont;

  typedef struct
  {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool     graphicsFamilyFound;
    bool     presentFamilyFound;
  } DK_vkQueueFamilyIndices;

  typedef struct
  {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR      *formats;
    uint32_t                 formatCount;
    VkPresentModeKHR        *presentModes;
    uint32_t                 presentModeCount;
  } DK_vkSwapChainSupportDetails;

  DK_VULKAN_FUNC void DK_vkInitApp( DK_vkApplication *app, int32_t width, int32_t height, const char *title );
  DK_VULKAN_FUNC void DK_vkCleanup( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkCreateInstanceEx( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkSetupDebugMessenger( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkCreateSurface( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkPickPhysicalDevice( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkCreateLogicalDevice( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkCreateSwapChain( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkGetWindowScale( DK_vkApplication *app, float *scaleX, float *scaleY );

  DK_VULKAN_FUNC void           DK_vkCreateImageViews( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkCreateRenderPass( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkCreateGraphicsPipeline( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkCreateFramebuffers( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkCreateCommandPool( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkCreateCommandBuffers( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkCreateSyncObjects( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkRecreateSwapChain( DK_vkApplication *app );
  DK_VULKAN_FUNC void           DK_vkCleanupSwapChain( DK_vkApplication *app );
  DK_VULKAN_FUNC VkShaderModule DK_vkCreateShaderModule( DK_vkApplication    *app,
                                                         const unsigned char *code,
                                                         size_t               codeSize );
  DK_VULKAN_FUNC void           DK_vkInitializeTextureDescriptor( DK_vkApplication *app );
  DK_VULKAN_FUNC void
  DK_vkSafeUnmapMemory( DK_vkApplication *app, VkDeviceMemory *memory, void **mappedData );

  DK_VULKAN_FUNC void     DK_vkCreateVertexBuffer( DK_vkApplication *app );
  DK_VULKAN_FUNC void     DK_vkCreateUniformBuffer( DK_vkApplication *app );
  DK_VULKAN_FUNC void     DK_vkCreateDescriptorSetLayoutEx( DK_vkApplication *app );
  DK_VULKAN_FUNC void     DK_vkCreateDescriptorPoolEx( DK_vkApplication *app );
  DK_VULKAN_FUNC void     DK_vkCreateDescriptorSetEx( DK_vkApplication *app );
  DK_VULKAN_FUNC void     DK_vkUpdateUniformBuffer( DK_vkApplication *app );
  DK_VULKAN_FUNC uint32_t DK_vkFindMemoryType( DK_vkApplication     *app,
                                               uint32_t              typeFilter,
                                               VkMemoryPropertyFlags properties );
  DK_VULKAN_FUNC void     DK_vkCreateBuffer( DK_vkApplication     *app,
                                             VkDeviceSize          size,
                                             VkBufferUsageFlags    usage,
                                             VkMemoryPropertyFlags properties,
                                             VkBuffer             *buffer,
                                             VkDeviceMemory       *bufferMemory );
  DK_VULKAN_FUNC void
  DK_vkCopyBuffer( DK_vkApplication *app, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size );

  DK_VULKAN_FUNC void DK_vkCreateBatchRenderer( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkDestroyBatchRenderer( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkBeginBatch( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkEndBatch( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkFlushBatch( DK_vkApplication *app );

  DK_VULKAN_FUNC void         DK_vkUpdateDescriptorSetLayout( DK_vkApplication *app );
  DK_VULKAN_FUNC DK_vkTexture DK_vkLoadTexture( DK_vkApplication *app, const char *filename );
  DK_VULKAN_FUNC void         DK_vkDestroyTexture( DK_vkApplication *app, DK_vkTexture *texture );
  DK_VULKAN_FUNC void         DK_vkCreateTextureImage( DK_vkApplication *app,
                                                       const char       *filename,
                                                       VkImage          *image,
                                                       VkDeviceMemory   *imageMemory,
                                                       int32_t          *width,
                                                       int32_t          *height,
                                                       int32_t          *channels );
  DK_VULKAN_FUNC void         DK_vkCreateDummyTexture( DK_vkApplication *app );

  DK_VULKAN_FUNC void
  DK_vkCreateTextureImageView( DK_vkApplication *app, VkImage image, VkImageView *imageView );
  DK_VULKAN_FUNC void            DK_vkCreateTextureSampler( DK_vkApplication *app, VkSampler *sampler );
  DK_VULKAN_FUNC void            DK_vkTransitionImageLayout( DK_vkApplication *app,
                                                             VkImage           image,
                                                             VkFormat          format,
                                                             VkImageLayout     oldLayout,
                                                             VkImageLayout     newLayout );
  DK_VULKAN_FUNC void            DK_vkCopyBufferToImage( DK_vkApplication *app,
                                                         VkBuffer          buffer,
                                                         VkImage           image,
                                                         uint32_t          width,
                                                         uint32_t          height );
  DK_VULKAN_FUNC VkCommandBuffer DK_vkBeginSingleTimeCommands( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkEndSingleTimeCommands( DK_vkApplication *app, VkCommandBuffer commandBuffer );

  DK_VULKAN_FUNC void     DK_vkInitTextureSystem( DK_vkApplication *app );
  DK_VULKAN_FUNC uint32_t DK_vkAddTexture( DK_vkApplication *app, const char *filename );
  DK_VULKAN_FUNC void     DK_vkSetTexture( DK_vkApplication *app, uint32_t textureId );
  DK_VULKAN_FUNC void     DK_vkUpdateDescriptorSetWithTexture( DK_vkApplication *app, DK_vkTexture *texture );
  DK_VULKAN_FUNC void     DK_vkCreateDescriptorPoolWithTextures( DK_vkApplication *app );

  DK_VULKAN_FUNC void
  DK_vkDrawTriangle( DK_vkApplication *app, DK_vkVec2 p1, DK_vkVec2 p2, DK_vkVec3 p3, DK_vkColor tint );

  DK_VULKAN_FUNC void
  DK_vkDrawRectangle( DK_vkApplication *app, DK_vkVec2 position, DK_vkSize size, DK_vkColor tint );

  DK_VULKAN_FUNC void DK_vkDrawQuad( DK_vkApplication *app,
                                     DK_vkVec2         p1,
                                     DK_vkVec2         p2,
                                     DK_vkVec2         p3,
                                     DK_vkVec2         p4,
                                     DK_vkColor        tint );

  DK_VULKAN_FUNC void DK_vkDrawGradientRectangle( DK_vkApplication *app,
                                                  DK_vkVec2         position,
                                                  DK_vkSize         size,
                                                  DK_vkColor        tint_bl,
                                                  DK_vkColor        tint_br,
                                                  DK_vkColor        tint_tr,
                                                  DK_vkColor        tint_tl );

  DK_VULKAN_FUNC void
  DK_vkDrawLine( DK_vkApplication *app, DK_vkVec2 p1, DK_vkVec2 p2, float thickness, DK_vkColor tint );

  DK_VULKAN_FUNC void DK_vkDrawRoundedRectangle( DK_vkApplication *app,
                                                 DK_vkVec2         position,
                                                 DK_vkSize         size,
                                                 float             radius,
                                                 DK_vkColor        tint,
                                                 int32_t           segments );

  DK_VULKAN_FUNC void DK_vkDrawCircle( DK_vkApplication *app,
                                       DK_vkVec2         position,
                                       float             radius,
                                       DK_vkColor        tint,
                                       int32_t           segments );

  DK_VULKAN_FUNC void DK_vkDrawTexturedQuad( DK_vkApplication *app,
                                             DK_vkVec2         position,
                                             DK_vkSize         size,
                                             DK_vkVec2         uv1,
                                             DK_vkVec2         uv2,
                                             DK_vkColor        tint,
                                             int32_t           samplerId );

  DK_VULKAN_FUNC void DK_vkDrawTexture( DK_vkApplication *app,
                                        DK_vkVec2         position,
                                        uint32_t          textureId,
                                        float             scale,
                                        DK_vkColor        tint );

  DK_VULKAN_FUNC void DK_vkDrawTextureRegion( DK_vkApplication *app,
                                              DK_vkVec2         position,
                                              DK_vkSize         size,
                                              uint32_t          textureId,
                                              DK_vkVec2         region_position,
                                              DK_vkSize         region_size,
                                              DK_vkColor        tinit );

  DK_VULKAN_FUNC void DK_vkFlushBatchWithTexture( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkCleanupTextureSystem( DK_vkApplication *app );

  DK_VULKAN_FUNC void
  DK_vkOrtho( float left, float right, float bottom, float top, float near, float far, DK_vkMat4 matrix );

  DK_VULKAN_FUNC void DK_vkIdentityMatrix( DK_vkMat4 matrix );
  DK_VULKAN_FUNC void DK_vkOrthoCameraInit( DK_vkApplication *app, float width, float height );

  DK_VULKAN_FUNC void DK_vkOrthoCameraUpdate( DK_vkApplication *app, float width, float height );
  DK_VULKAN_FUNC void DK_vkUpdateUniformBufferWithOrtho( DK_vkApplication *app );
  DK_VULKAN_FUNC void DK_vkZoomCamera( DK_vkApplication *app, float zoomFactor );
  DK_VULKAN_FUNC void DK_vkFramebufferResizeCallback( GLFWwindow *window, int32_t width, int32_t height );
  DK_VULKAN_FUNC void DK_vkScrollCallback( GLFWwindow *window, double xoffset, double yoffset );

  DK_VULKAN_FUNC VKAPI_ATTR VkBool32 VKAPI_CALL
  DK_vkDebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT             messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                      void                                       *pUserData );

  DK_VULKAN_FUNC VkResult
  DK_vkCreateDebugUtilsMessengerEXT( VkInstance                                instance,
                                     const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                     const VkAllocationCallbacks              *pAllocator,
                                     VkDebugUtilsMessengerEXT                 *pDebugMessenger );

  DK_VULKAN_FUNC void DK_vkDestroyDebugUtilsMessengerEXT( VkInstance                   instance,
                                                          VkDebugUtilsMessengerEXT     debugMessenger,
                                                          const VkAllocationCallbacks *pAllocator );

  DK_VULKAN_FUNC unsigned char *DK_vkReadFile( const char *filename, size_t *size );

  DK_VULKAN_FUNC DK_vkFont DK_vkLoadFont( DK_vkApplication *app, const char *filename, int32_t baseSize );
  DK_VULKAN_FUNC void      DK_vkUnloadFont( DK_vkApplication *app, DK_vkFont *font );

  DK_VULKAN_FUNC void DK_vkDrawText( DK_vkApplication *app,
                                     DK_vkFont        *font,
                                     const char       *text,
                                     DK_vkVec2         position,
                                     float             fontSize,
                                     DK_vkColor        tint );

  DK_VULKAN_FUNC float DK_vkMeasureTextWidth( DK_vkFont *font, const char *text, float fontSize );
  DK_VULKAN_FUNC float DK_vkMeasureTextHeight( DK_vkFont *font, const char *text, float fontSize );

#define DK_VK_IMPLEMENTATION
#ifdef DK_VK_IMPLEMENTATION

  const char   *validationLayers[]   = { "VK_LAYER_KHRONOS_validation" };
  const int32_t validationLayerCount = 1;

  const char *deviceExtensions[] = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      "VK_KHR_portability_subset",
  };

  const int32_t deviceExtensionCount = 2;

#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

  unsigned char *vertShaderCode = NULL;
  unsigned char *fragShaderCode = NULL;

  size_t vertShaderCodeSize;
  size_t fragShaderCodeSize;

  const DK_Vertex vertices[] = {
      { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
      { { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
      { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
  };

  DK_VULKAN_FUNC unsigned char *DK_vkReadFile( const char *filename, size_t *size )
  {
    int32_t fd = open( filename, O_RDONLY );
    if ( fd == -1 )
    {
      fprintf( stderr, "Failed to open file: %s\n", filename );
      exit( 1 );
    }

    struct stat st;
    if ( fstat( fd, &st ) == -1 )
    {
      fprintf( stderr, "Failed to get file size: %s\n", filename );
      close( fd );
      exit( 1 );
    }

    unsigned char *buffer    = (unsigned char *)malloc( st.st_size );
    ssize_t        bytesRead = read( fd, buffer, st.st_size );

    if ( bytesRead != st.st_size )
    {
      fprintf( stderr, "Failed to read file: %s\n", filename );
      free( buffer );
      close( fd );
      exit( 1 );
    }

    close( fd );
    *size = st.st_size;
    return buffer;
  }

  DK_VULKAN_FUNC uint32_t DK_vkFindMemoryType( DK_vkApplication     *app,
                                               uint32_t              typeFilter,
                                               VkMemoryPropertyFlags properties )
  {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties( app->physicalDevice, &memProperties );

    for ( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ )
    {
      if ( ( typeFilter & ( 1 << i ) ) &&
           ( memProperties.memoryTypes[i].propertyFlags & properties ) == properties )
      {
        return i;
      }
    }

    fprintf( stderr, "Failed to find suitable memory type\n" );
    exit( 1 );
  }

  DK_VULKAN_FUNC void DK_vkCreateBuffer( DK_vkApplication     *app,
                                         VkDeviceSize          size,
                                         VkBufferUsageFlags    usage,
                                         VkMemoryPropertyFlags properties,
                                         VkBuffer             *buffer,
                                         VkDeviceMemory       *bufferMemory )
  {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size               = size;
    bufferInfo.usage              = usage;
    bufferInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;

    if ( vkCreateBuffer( app->device, &bufferInfo, NULL, buffer ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create buffer\n" );
      exit( 1 );
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements( app->device, *buffer, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize       = memRequirements.size;
    allocInfo.memoryTypeIndex      = DK_vkFindMemoryType( app, memRequirements.memoryTypeBits, properties );

    if ( vkAllocateMemory( app->device, &allocInfo, NULL, bufferMemory ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to allocate buffer memory\n" );
      exit( 1 );
    }

    vkBindBufferMemory( app->device, *buffer, *bufferMemory, 0 );
  }

  DK_VULKAN_FUNC void
  DK_vkCopyBuffer( DK_vkApplication *app, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size )
  {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool                 = app->commandPool;
    allocInfo.commandBufferCount          = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers( app->device, &allocInfo, &commandBuffer );

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer( commandBuffer, &beginInfo );

    VkBufferCopy copyRegion = { 0 };
    copyRegion.srcOffset    = 0;
    copyRegion.dstOffset    = 0;
    copyRegion.size         = size;
    vkCmdCopyBuffer( commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion );

    vkEndCommandBuffer( commandBuffer );

    VkSubmitInfo submitInfo       = {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit( app->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( app->graphicsQueue );

    vkFreeCommandBuffers( app->device, app->commandPool, 1, &commandBuffer );
  }

  DK_VULKAN_FUNC void DK_vkCreateVertexBuffer( DK_vkApplication *app )
  {
    VkDeviceSize bufferSize = sizeof( vertices );

    VkBuffer       stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    DK_vkCreateBuffer( app,
                       bufferSize,
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &stagingBuffer,
                       &stagingBufferMemory );

    void *data;
    vkMapMemory( app->device, stagingBufferMemory, 0, bufferSize, 0, &data );
    memcpy( data, vertices, bufferSize );
    vkUnmapMemory( app->device, stagingBufferMemory );

    DK_vkCreateBuffer( app,
                       bufferSize,
                       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                       &app->vertexBuffer,
                       &app->vertexBufferMemory );

    DK_vkCopyBuffer( app, stagingBuffer, app->vertexBuffer, bufferSize );

    vkDestroyBuffer( app->device, stagingBuffer, NULL );
    vkFreeMemory( app->device, stagingBufferMemory, NULL );
  }

  DK_VULKAN_FUNC void DK_vkCreateUniformBuffer( DK_vkApplication *app )
  {
    VkDeviceSize bufferSize = sizeof( DK_vkUniformBufferObject );
    DK_vkCreateBuffer( app,
                       bufferSize,
                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &app->uniformBuffer,
                       &app->uniformBufferMemory );
  }

  DK_VULKAN_FUNC void DK_vkCreateDescriptorSetLayoutEx( DK_vkApplication *app )
  {
    VkDescriptorSetLayoutBinding bindings[2] = { 0 };

    bindings[0].binding            = 0;
    bindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount    = 1;
    bindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
    bindings[0].pImmutableSamplers = NULL;

    bindings[1].binding            = 1;
    bindings[1].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].descriptorCount    = DK_VK_MAX_TEXTURES;
    bindings[1].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[1].pImmutableSamplers = NULL;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount                    = 2;
    layoutInfo.pBindings                       = bindings;

    if ( vkCreateDescriptorSetLayout( app->device, &layoutInfo, NULL, &app->descriptorSetLayout ) !=
         VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create descriptor set layout\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateDescriptorPoolEx( DK_vkApplication *app )
  {
    VkDescriptorPoolSize poolSizes[2] = {};

    poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;

    poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = DK_VK_MAX_TEXTURES;

    VkDescriptorPoolCreateInfo poolInfo = {};
    poolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount              = 2;
    poolInfo.pPoolSizes                 = poolSizes;
    poolInfo.maxSets                    = 1;

    if ( vkCreateDescriptorPool( app->device, &poolInfo, NULL, &app->descriptorPool ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create descriptor pool\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkUpdateDescriptorSetWithTextures( DK_vkApplication *app )
  {
    VkDescriptorBufferInfo bufferInfo = { 0 };
    bufferInfo.buffer                 = app->uniformBuffer;
    bufferInfo.offset                 = 0;
    bufferInfo.range                  = sizeof( DK_vkUniformBufferObject );

    VkDescriptorImageInfo imageInfos[DK_VK_MAX_TEXTURES];
    for ( uint32_t i = 0; i < DK_VK_MAX_TEXTURES; i++ )
    {
      uint32_t texIndex         = i < app->textureCount ? i : 0; // use texture 0 as fallback
      imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfos[i].imageView   = app->textures[texIndex].view;
      imageInfos[i].sampler     = app->textures[texIndex].sampler;
    }

    VkWriteDescriptorSet descriptorWrites[2] = {};

    descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet          = app->descriptorSet;
    descriptorWrites[0].dstBinding      = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo     = &bufferInfo;

    descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet          = app->descriptorSet;
    descriptorWrites[1].dstBinding      = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = DK_VK_MAX_TEXTURES;
    descriptorWrites[1].pImageInfo      = imageInfos;

    vkUpdateDescriptorSets( app->device, 2, descriptorWrites, 0, NULL );
  }

  DK_VULKAN_FUNC void DK_vkCreateDescriptorSetEx( DK_vkApplication *app )
  {
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool              = app->descriptorPool;
    allocInfo.descriptorSetCount          = 1;
    allocInfo.pSetLayouts                 = &app->descriptorSetLayout;

    if ( vkAllocateDescriptorSets( app->device, &allocInfo, &app->descriptorSet ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to allocate descriptor set\n" );
      exit( 1 );
    }

    VkDescriptorBufferInfo bufferInfo = { 0 };
    bufferInfo.buffer                 = app->uniformBuffer;
    bufferInfo.offset                 = 0;
    bufferInfo.range                  = sizeof( DK_vkUniformBufferObject );

    VkWriteDescriptorSet descriptorWrites[1] = {};

    descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet          = app->descriptorSet;
    descriptorWrites[0].dstBinding      = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo     = &bufferInfo;

    vkUpdateDescriptorSets( app->device, 1, descriptorWrites, 0, NULL );
  }

  DK_VULKAN_FUNC void DK_vkUpdateUniformBuffer( DK_vkApplication *app )
  {
    DK_vkUniformBufferObject ubo  = { 0 };
    float                    rotX = 0.0f;

    float cosVal = cos( rotX );
    float sinVal = sin( rotX );

    ubo.model[0][0] = cosVal;
    ubo.model[0][1] = -sinVal;
    ubo.model[1][0] = sinVal;
    ubo.model[1][1] = cosVal;
    ubo.model[2][2] = 1.0f;
    ubo.model[3][3] = 1.0f;

    DK_vkIdentityMatrix( ubo.view );
    DK_vkOrtho( app->camera.left,
                app->camera.right,
                app->camera.bottom,
                app->camera.top,
                app->camera.near,
                app->camera.far,
                ubo.proj );

    void *data;
    vkMapMemory( app->device, app->uniformBufferMemory, 0, sizeof( ubo ), 0, &data );
    memcpy( data, &ubo, sizeof( ubo ) );
    vkUnmapMemory( app->device, app->uniformBufferMemory );
  }

  DK_VULKAN_FUNC void DK_vkFramebufferResizeCallback( GLFWwindow *window, int32_t width, int32_t height )
  {
    DK_vkApplication *app = (DK_vkApplication *)glfwGetWindowUserPointer( window );
    if ( width == 0 || height == 0 )
    {
      return;
    }

    app->framebufferResized = true;
    app->screenWidth        = width;
    app->screenHeight       = height;

    DK_vkOrthoCameraUpdate( app, (float)width, (float)height );
  }

  DK_VULKAN_FUNC void DK_vkScrollCallback( GLFWwindow *window, double xoffset, double yoffset )
  {
    DK_vkApplication *app = (DK_vkApplication *)glfwGetWindowUserPointer( window );

    if ( yoffset > 0 )
    {
      DK_vkZoomCamera( app, 0.9f );
    }
    else if ( yoffset < 0 )
    {
      DK_vkZoomCamera( app, 1.1f );
    }
  }

  DK_VULKAN_FUNC VKAPI_ATTR VkBool32 VKAPI_CALL
  DK_vkDebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT             messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                      void                                       *pUserData )
  {

    fprintf( stderr, "Validation layer: %s\n", pCallbackData->pMessage );
    return VK_FALSE;
  }

  DK_VULKAN_FUNC VkResult
  DK_vkCreateDebugUtilsMessengerEXT( VkInstance                                instance,
                                     const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                     const VkAllocationCallbacks              *pAllocator,
                                     VkDebugUtilsMessengerEXT                 *pDebugMessenger )
  {
    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance,
                                                                   "vkCreateDebugUtilsMessengerEXT" );
    if ( func != NULL )
    {
      return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
    }
    else
    {
      return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
  }

  DK_VULKAN_FUNC void DK_vkDestroyDebugUtilsMessengerEXT( VkInstance                   instance,
                                                          VkDebugUtilsMessengerEXT     debugMessenger,
                                                          const VkAllocationCallbacks *pAllocator )
  {
    PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance,
                                                                    "vkDestroyDebugUtilsMessengerEXT" );
    if ( func != NULL )
    {
      func( instance, debugMessenger, pAllocator );
    }
  }

  DK_VULKAN_FUNC void DK_vkInitializeTextureDescriptor( DK_vkApplication *app )
  {
    if ( app->textureCount == 0 )
    {
      if ( app->textures == NULL )
      {
        app->maxTextures  = DK_VK_MAX_TEXTURES;
        app->textureCount = 0;
        app->textures     = (DK_vkTexture *)malloc( sizeof( DK_vkTexture ) * app->maxTextures );
      }

      DK_vkCreateDummyTexture( app );
    }

    DK_vkTexture          *texture    = app->currentTexture ? app->currentTexture : &app->textures[0];
    VkDescriptorImageInfo *imageInfos = malloc( app->maxTextures * sizeof( VkDescriptorImageInfo ) );
    for ( uint32_t i = 0; i < app->maxTextures; i++ )
    {
      if ( i < app->textureCount )
      {
        imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfos[i].imageView   = app->textures[i].view;
        imageInfos[i].sampler     = app->textures[i].sampler;
      }
      else
      {
        imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfos[i].imageView   = app->textures[0].view;
        imageInfos[i].sampler     = app->textures[0].sampler;
      }
    }

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType                = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet               = app->descriptorSet;
    descriptorWrite.dstBinding           = 1;
    descriptorWrite.dstArrayElement      = 0;
    descriptorWrite.descriptorType       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount      = app->maxTextures;
    descriptorWrite.pImageInfo           = imageInfos;

    vkUpdateDescriptorSets( app->device, 1, &descriptorWrite, 0, NULL );

    free( imageInfos );
  }

  DK_VULKAN_FUNC void DK_vkGetWindowScale( DK_vkApplication *app, float *scaleX, float *scaleY )
  {
    int windowWidth, windowHeight;
    int framebufferWidth, framebufferHeight;

    glfwGetWindowSize( app->window, &windowWidth, &windowHeight );
    glfwGetFramebufferSize( app->window, &framebufferWidth, &framebufferHeight );

    *scaleX = (float)framebufferWidth / windowWidth;
    *scaleY = (float)framebufferHeight / windowHeight;
  }

  DK_VULKAN_FUNC void DK_vkInitApp( DK_vkApplication *app, int32_t width, int32_t height, const char *title )
  {
    vertShaderCode = DK_vkReadFile( "vert.spv", &vertShaderCodeSize );
    fragShaderCode = DK_vkReadFile( "frag.spv", &fragShaderCodeSize );

    glfwInit();

    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );
    glfwWindowHint( GLFW_SCALE_TO_MONITOR, GLFW_TRUE );

    app->window = glfwCreateWindow( width, height, title, NULL, NULL );
    glfwSetWindowUserPointer( app->window, app );

    app->screenWidth  = width;
    app->screenHeight = height;

    int32_t framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize( app->window, &framebufferWidth, &framebufferHeight );
    app->screenWidth  = framebufferWidth;
    app->screenHeight = framebufferHeight;

    glfwSetFramebufferSizeCallback( app->window, DK_vkFramebufferResizeCallback );
#if 0
    glfwSetScrollCallback( app->window, DK_vkScrollCallback );
#endif
    DK_vkOrthoCameraInit( app, (float)width, (float)height );

    DK_vkCreateInstanceEx( app );
    DK_vkSetupDebugMessenger( app );
    DK_vkCreateSurface( app );
    DK_vkPickPhysicalDevice( app );
    DK_vkCreateLogicalDevice( app );

    DK_vkCreateDescriptorSetLayoutEx( app );

    DK_vkCreateSwapChain( app );
    DK_vkCreateImageViews( app );
    DK_vkCreateRenderPass( app );
    DK_vkCreateGraphicsPipeline( app );
    DK_vkCreateFramebuffers( app );
    DK_vkCreateCommandPool( app );

    DK_vkCreateVertexBuffer( app );
    DK_vkCreateUniformBuffer( app );

    DK_vkCreateDescriptorPoolEx( app );
    DK_vkCreateDescriptorSetEx( app );

    DK_vkInitTextureSystem( app );
    DK_vkInitializeTextureDescriptor( app );

    DK_vkCreateCommandBuffers( app );
    DK_vkCreateSyncObjects( app );

    DK_vkCreateBatchRenderer( app );

    app->currentFrame       = 0;
    app->framebufferResized = true;

    printf( "Info: Initialization step ok\n" );
  }

  DK_VULKAN_FUNC void DK_vkCleanup( DK_vkApplication *app )
  {

    vkDeviceWaitIdle( app->device );
    DK_vkCleanupSwapChain( app );

    DK_vkDestroyBatchRenderer( app );

    vkDestroyBuffer( app->device, app->uniformBuffer, NULL );
    vkFreeMemory( app->device, app->uniformBufferMemory, NULL );

    vkDestroyDescriptorPool( app->device, app->descriptorPool, NULL );
    vkDestroyDescriptorSetLayout( app->device, app->descriptorSetLayout, NULL );

    vkDestroyBuffer( app->device, app->vertexBuffer, NULL );
    vkFreeMemory( app->device, app->vertexBufferMemory, NULL );

    for ( int32_t i = 0; i < DK_VULKAN_MAX_FRAMES_IN_FLIGHT; i++ )
    {
      vkDestroySemaphore( app->device, app->renderFinishedSemaphores[i], NULL );
      vkDestroySemaphore( app->device, app->imageAvailableSemaphores[i], NULL );
      vkDestroyFence( app->device, app->inFlightFences[i], NULL );
    }
    free( app->renderFinishedSemaphores );
    free( app->imageAvailableSemaphores );
    free( app->inFlightFences );

    vkDestroyCommandPool( app->device, app->commandPool, NULL );
    vkDestroyDevice( app->device, NULL );
    if ( enableValidationLayers )
    {
      DK_vkDestroyDebugUtilsMessengerEXT( app->instance, app->debugMessenger, NULL );
    }

    vkDestroySurfaceKHR( app->instance, app->surface, NULL );
    vkDestroyInstance( app->instance, NULL );

    free( vertShaderCode );
    free( fragShaderCode );

    glfwDestroyWindow( app->window );
    glfwTerminate();
  }

  DK_VULKAN_FUNC void DK_vkCreateInstanceEx( DK_vkApplication *app )
  {
    VkApplicationInfo appInfo  = {};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName   = "No App";
    appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.pEngineName        = "No Engine";
    appInfo.engineVersion      = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.apiVersion         = VK_API_VERSION_1_0;

    uint32_t     glfwExtensionCount = 0;
    const char **glfwExtensions     = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

    const char **extensions;
    uint32_t     extensionCount = glfwExtensionCount;

    if ( enableValidationLayers )
    {
      extensionCount += 2;
      extensions = (const char **)malloc( extensionCount * sizeof( char * ) );
      for ( uint32_t i = 0; i < glfwExtensionCount; i++ )
      {
        extensions[i] = glfwExtensions[i];
      }

      extensions[glfwExtensionCount]     = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
      extensions[glfwExtensionCount + 1] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
    }
    else
    {
      extensionCount += 1;
      extensions = (const char **)malloc( extensionCount * sizeof( char * ) );
      for ( uint32_t i = 0; i < glfwExtensionCount; i++ )
      {
        extensions[i] = glfwExtensions[i];
      }

      extensions[glfwExtensionCount] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
    }

    VkInstanceCreateInfo createInfo    = {};
    createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo        = &appInfo;
    createInfo.enabledExtensionCount   = extensionCount;
    createInfo.ppEnabledExtensionNames = extensions;

    if ( enableValidationLayers )
    {
      createInfo.enabledLayerCount   = validationLayerCount;
      createInfo.ppEnabledLayerNames = validationLayers;

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
      debugCreateInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      debugCreateInfo.pfnUserCallback = DK_vkDebugCallback;

      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }
    else
    {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext             = NULL;
    }

    if ( vkCreateInstance( &createInfo, NULL, &app->instance ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create Vulkan instance\n" );
      exit( 1 );
    }

    if ( enableValidationLayers )
    {
      free( extensions );
    }
  }

  DK_VULKAN_FUNC void DK_vkSetupDebugMessenger( DK_vkApplication *app )
  {
    if ( !enableValidationLayers )
      return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity                    = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = DK_vkDebugCallback;

    if ( DK_vkCreateDebugUtilsMessengerEXT( app->instance, &createInfo, NULL, &app->debugMessenger ) !=
         VK_SUCCESS )
    {
      fprintf( stderr, "Failed to set up debug messenger\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateSurface( DK_vkApplication *app )
  {
    if ( glfwCreateWindowSurface( app->instance, app->window, NULL, &app->surface ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create window surface\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC bool DK_vkIsDeviceSuitable( VkPhysicalDevice device, VkSurfaceKHR surface )
  {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures   deviceFeatures;
    vkGetPhysicalDeviceProperties( device, &deviceProperties );
    vkGetPhysicalDeviceFeatures( device, &deviceFeatures );

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, NULL );

    VkQueueFamilyProperties *queueFamilies =
        (VkQueueFamilyProperties *)malloc( queueFamilyCount * sizeof( VkQueueFamilyProperties ) );
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies );

    bool graphicsFamily = false;
    bool presentFamily  = false;

    for ( uint32_t i = 0; i < queueFamilyCount; i++ )
    {
      if ( queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
      {
        graphicsFamily = true;
      }

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR( device, i, surface, &presentSupport );

      if ( presentSupport )
      {
        presentFamily = true;
      }

      if ( graphicsFamily && presentFamily )
      {
        break;
      }
    }

    free( queueFamilies );

    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties( device, NULL, &extensionCount, NULL );

    VkExtensionProperties *availableExtensions =
        (VkExtensionProperties *)malloc( extensionCount * sizeof( VkExtensionProperties ) );
    vkEnumerateDeviceExtensionProperties( device, NULL, &extensionCount, availableExtensions );

    bool extensionsSupported = true;
    for ( int32_t i = 0; i < deviceExtensionCount; i++ )
    {
      bool found = false;

      for ( uint32_t j = 0; j < extensionCount; j++ )
      {
        if ( strcmp( deviceExtensions[i], availableExtensions[j].extensionName ) == 0 )
        {
          found = true;
          break;
        }
      }

      if ( !found )
      {
        extensionsSupported = false;
        break;
      }
    }

    free( availableExtensions );

    bool swapChainAdequate = false;
    if ( extensionsSupported )
    {
      uint32_t formatCount;
      uint32_t presentModeCount;

      vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &formatCount, NULL );
      vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &presentModeCount, NULL );

      swapChainAdequate = formatCount > 0 && presentModeCount > 0;
    }

    return graphicsFamily && presentFamily && extensionsSupported && swapChainAdequate;
  }

  DK_VULKAN_FUNC void DK_vkPickPhysicalDevice( DK_vkApplication *app )
  {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices( app->instance, &deviceCount, NULL );

    if ( deviceCount == 0 )
    {
      fprintf( stderr, "Failed to find GPUs with Vulkan support\n" );
      exit( 1 );
    }

    VkPhysicalDevice *devices = (VkPhysicalDevice *)malloc( deviceCount * sizeof( VkPhysicalDevice ) );
    vkEnumeratePhysicalDevices( app->instance, &deviceCount, devices );

    for ( uint32_t i = 0; i < deviceCount; i++ )
    {
      if ( DK_vkIsDeviceSuitable( devices[i], app->surface ) )
      {
        app->physicalDevice = devices[i];
        break;
      }
    }

    free( devices );

    if ( app->physicalDevice == VK_NULL_HANDLE )
    {
      fprintf( stderr, "Failed to find a suitable GPU\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC DK_vkQueueFamilyIndices DK_vkFindQueueFamilies( VkPhysicalDevice device,
                                                                 VkSurfaceKHR     surface )
  {
    DK_vkQueueFamilyIndices indices = { 0 };

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, NULL );

    VkQueueFamilyProperties *queueFamilies =
        (VkQueueFamilyProperties *)malloc( queueFamilyCount * sizeof( VkQueueFamilyProperties ) );
    vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies );

    for ( uint32_t i = 0; i < queueFamilyCount; i++ )
    {
      if ( queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
      {
        indices.graphicsFamily      = i;
        indices.graphicsFamilyFound = true;
      }

      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR( device, i, surface, &presentSupport );

      if ( presentSupport )
      {
        indices.presentFamily      = i;
        indices.presentFamilyFound = true;
      }

      if ( indices.graphicsFamilyFound && indices.presentFamilyFound )
      {
        break;
      }
    }

    free( queueFamilies );

    return indices;
  }

  DK_VULKAN_FUNC void DK_vkCreateLogicalDevice( DK_vkApplication *app )
  {
    DK_vkQueueFamilyIndices indices = DK_vkFindQueueFamilies( app->physicalDevice, app->surface );

    uint32_t uniqueQueueFamilies[2];
    uint32_t uniqueQueueFamilyCount = 0;

    uniqueQueueFamilies[uniqueQueueFamilyCount++] = indices.graphicsFamily;
    if ( indices.graphicsFamily != indices.presentFamily )
    {
      uniqueQueueFamilies[uniqueQueueFamilyCount++] = indices.presentFamily;
    }

    VkDeviceQueueCreateInfo *queueCreateInfos =
        (VkDeviceQueueCreateInfo *)malloc( uniqueQueueFamilyCount * sizeof( VkDeviceQueueCreateInfo ) );

    float queuePriority = 1.0f;
    for ( uint32_t i = 0; i < uniqueQueueFamilyCount; i++ )
    {
      VkDeviceQueueCreateInfo queueCreateInfo = {};
      queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queueCreateInfo.queueFamilyIndex        = uniqueQueueFamilies[i];
      queueCreateInfo.queueCount              = 1;
      queueCreateInfo.pQueuePriorities        = &queuePriority;

      queueCreateInfos[i] = queueCreateInfo;
    }

    VkPhysicalDeviceFeatures deviceFeatures = { 0 };

    VkDeviceCreateInfo createInfo      = {};
    createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount    = uniqueQueueFamilyCount;
    createInfo.pQueueCreateInfos       = queueCreateInfos;
    createInfo.pEnabledFeatures        = &deviceFeatures;
    createInfo.enabledExtensionCount   = deviceExtensionCount;
    createInfo.ppEnabledExtensionNames = deviceExtensions;

    if ( enableValidationLayers )
    {
      createInfo.enabledLayerCount   = validationLayerCount;
      createInfo.ppEnabledLayerNames = validationLayers;
    }
    else
    {
      createInfo.enabledLayerCount = 0;
    }

    if ( vkCreateDevice( app->physicalDevice, &createInfo, NULL, &app->device ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create logical device\n" );
      exit( 1 );
    }

    vkGetDeviceQueue( app->device, indices.graphicsFamily, 0, &app->graphicsQueue );
    vkGetDeviceQueue( app->device, indices.presentFamily, 0, &app->presentQueue );

    free( queueCreateInfos );
  }

  DK_VULKAN_FUNC DK_vkSwapChainSupportDetails DK_vkQuerySwapChainSupport( VkPhysicalDevice device,
                                                                          VkSurfaceKHR     surface )
  {
    DK_vkSwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( device, surface, &details.capabilities );
    vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &details.formatCount, NULL );

    if ( details.formatCount != 0 )
    {
      details.formats = (VkSurfaceFormatKHR *)malloc( details.formatCount * sizeof( VkSurfaceFormatKHR ) );
      vkGetPhysicalDeviceSurfaceFormatsKHR( device, surface, &details.formatCount, details.formats );
    }

    vkGetPhysicalDeviceSurfacePresentModesKHR( device, surface, &details.presentModeCount, NULL );

    if ( details.presentModeCount != 0 )
    {
      details.presentModes =
          (VkPresentModeKHR *)malloc( details.presentModeCount * sizeof( VkPresentModeKHR ) );
      vkGetPhysicalDeviceSurfacePresentModesKHR( device,
                                                 surface,
                                                 &details.presentModeCount,
                                                 details.presentModes );
    }

    return details;
  }

  DK_VULKAN_FUNC VkSurfaceFormatKHR DK_vkChooseSwapSurfaceFormat( const VkSurfaceFormatKHR *availableFormats,
                                                                  uint32_t                  formatCount )
  {

    for ( uint32_t i = 0; i < formatCount; i++ )
    {
      if ( availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
           availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
      {
        return availableFormats[i];
      }
    }

    return availableFormats[0];
  }

  DK_VULKAN_FUNC VkPresentModeKHR DK_vkChooseSwapPresentMode( const VkPresentModeKHR *availablePresentModes,
                                                              uint32_t                presentModeCount )
  {
    for ( uint32_t i = 0; i < presentModeCount; i++ )
    {
      if ( availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR )
      {
        return VK_PRESENT_MODE_MAILBOX_KHR;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  DK_VULKAN_FUNC VkExtent2D DK_vkChooseSwapExtent( const VkSurfaceCapabilitiesKHR *capabilities,
                                                   GLFWwindow                     *window )
  {
    if ( capabilities->currentExtent.width != UINT32_MAX )
    {
      return capabilities->currentExtent;
    }
    else
    {
      int32_t width, height;
      glfwGetFramebufferSize( window, &width, &height );
      VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };
      actualExtent.width      = ( actualExtent.width > capabilities->maxImageExtent.width )
                                    ? capabilities->maxImageExtent.width
                                    : actualExtent.width;

      actualExtent.width = ( actualExtent.width < capabilities->minImageExtent.width )
                               ? capabilities->minImageExtent.width
                               : actualExtent.width;

      actualExtent.height = ( actualExtent.height > capabilities->maxImageExtent.height )
                                ? capabilities->maxImageExtent.height
                                : actualExtent.height;

      actualExtent.height = ( actualExtent.height < capabilities->minImageExtent.height )
                                ? capabilities->minImageExtent.height
                                : actualExtent.height;

      return actualExtent;
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateSwapChain( DK_vkApplication *app )
  {
    DK_vkSwapChainSupportDetails swapChainSupport =
        DK_vkQuerySwapChainSupport( app->physicalDevice, app->surface );

    VkSurfaceFormatKHR surfaceFormat =
        DK_vkChooseSwapSurfaceFormat( swapChainSupport.formats, swapChainSupport.formatCount );
    VkPresentModeKHR presentMode =
        DK_vkChooseSwapPresentMode( swapChainSupport.presentModes, swapChainSupport.presentModeCount );
    VkExtent2D extent = DK_vkChooseSwapExtent( &swapChainSupport.capabilities, app->window );

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if ( swapChainSupport.capabilities.maxImageCount > 0 &&
         imageCount > swapChainSupport.capabilities.maxImageCount )
    {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface                  = app->surface;
    createInfo.minImageCount            = imageCount;
    createInfo.imageFormat              = surfaceFormat.format;
    createInfo.imageColorSpace          = surfaceFormat.colorSpace;
    createInfo.imageExtent              = extent;
    createInfo.imageArrayLayers         = 1;
    createInfo.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    DK_vkQueueFamilyIndices indices = DK_vkFindQueueFamilies( app->physicalDevice, app->surface );
    uint32_t                queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

    if ( indices.graphicsFamily != indices.presentFamily )
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices   = queueFamilyIndices;
    }
    else
    {
      createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
      createInfo.queueFamilyIndexCount = 0;
      createInfo.pQueueFamilyIndices   = NULL;
    }

    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode    = presentMode;
    createInfo.clipped        = VK_TRUE;
    createInfo.oldSwapchain   = VK_NULL_HANDLE;

    if ( vkCreateSwapchainKHR( app->device, &createInfo, NULL, &app->swapChain ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create swap chain\n" );
      exit( 1 );
    }

    vkGetSwapchainImagesKHR( app->device, app->swapChain, &imageCount, NULL );
    app->swapChainImages = (VkImage *)malloc( imageCount * sizeof( VkImage ) );
    vkGetSwapchainImagesKHR( app->device, app->swapChain, &imageCount, app->swapChainImages );

    app->swapChainImageFormat = surfaceFormat.format;
    app->swapChainExtent      = extent;
    app->imageCount           = imageCount;

    free( swapChainSupport.formats );
    free( swapChainSupport.presentModes );
  }

  DK_VULKAN_FUNC void DK_vkCreateImageViews( DK_vkApplication *app )
  {
    app->swapChainImageViews = (VkImageView *)malloc( app->imageCount * sizeof( VkImageView ) );

    for ( uint32_t i = 0; i < app->imageCount; i++ )
    {
      VkImageViewCreateInfo createInfo = {};
      createInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image                 = app->swapChainImages[i];
      createInfo.viewType              = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format                = app->swapChainImageFormat;

      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

      createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel   = 0;
      createInfo.subresourceRange.levelCount     = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount     = 1;

      if ( vkCreateImageView( app->device, &createInfo, NULL, &app->swapChainImageViews[i] ) != VK_SUCCESS )
      {
        fprintf( stderr, "Failed to create image views\n" );
        exit( 1 );
      }
    }
  }

  DK_VULKAN_FUNC VkShaderModule DK_vkCreateShaderModule( DK_vkApplication    *app,
                                                         const unsigned char *code,
                                                         size_t               codeSize )
  {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize                 = codeSize;
    createInfo.pCode                    = (const uint32_t *)code;

    VkShaderModule shaderModule;
    if ( vkCreateShaderModule( app->device, &createInfo, NULL, &shaderModule ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create shader module\n" );
      exit( 1 );
    }

    return shaderModule;
  }

  DK_VULKAN_FUNC void DK_vkCreateRenderPass( DK_vkApplication *app )
  {
    VkAttachmentDescription colorAttachment = { 0 };
    colorAttachment.format                  = app->swapChainImageFormat;
    colorAttachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = { 0 };
    colorAttachmentRef.attachment            = 0;
    colorAttachmentRef.layout                = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = { 0 };
    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments    = &colorAttachmentRef;

    VkSubpassDependency dependency = { 0 };
    dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass          = 0;
    dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask       = 0;
    dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount        = 1;
    renderPassInfo.pAttachments           = &colorAttachment;
    renderPassInfo.subpassCount           = 1;
    renderPassInfo.pSubpasses             = &subpass;
    renderPassInfo.dependencyCount        = 1;
    renderPassInfo.pDependencies          = &dependency;

    if ( vkCreateRenderPass( app->device, &renderPassInfo, NULL, &app->renderPass ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create render pass\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateGraphicsPipeline( DK_vkApplication *app )
  {
    VkShaderModule vertShaderModule = DK_vkCreateShaderModule( app, vertShaderCode, vertShaderCodeSize );
    VkShaderModule fragShaderModule = DK_vkCreateShaderModule( app, fragShaderCode, fragShaderCodeSize );

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage                           = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module                          = vertShaderModule;
    vertShaderStageInfo.pName                           = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage                           = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module                          = fragShaderModule;
    fragShaderStageInfo.pName                           = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkVertexInputBindingDescription bindingDescription = { 0 };
    bindingDescription.binding                         = 0;
    bindingDescription.stride                          = sizeof( DK_Vertex );
    bindingDescription.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attributeDescriptions[4] = { 0 };

    attributeDescriptions[0].binding  = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset   = offsetof( DK_Vertex, pos );

    attributeDescriptions[1].binding  = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format   = VK_FORMAT_R32G32B32A32_SFLOAT;
    attributeDescriptions[1].offset   = offsetof( DK_Vertex, color );

    attributeDescriptions[2].binding  = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset   = offsetof( DK_Vertex, texCoord );

    attributeDescriptions[3].binding  = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format   = VK_FORMAT_R32_SINT;
    attributeDescriptions[3].offset   = offsetof( DK_Vertex, samplerId );

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType                         = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions    = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = 4;
    vertexInputInfo.pVertexAttributeDescriptions    = attributeDescriptions;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = { 0 };
    viewport.x          = 0.0f;
    viewport.y          = 0.0f;
    viewport.width      = (float)app->swapChainExtent.width;
    viewport.height     = (float)app->swapChainExtent.height;
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    VkRect2D scissor = { 0 };
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent   = app->swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount                     = 1;
    viewportState.pViewports                        = &viewport;
    viewportState.scissorCount                      = 1;
    viewportState.pScissors                         = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable        = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode             = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth               = 1.0f;
    rasterizer.cullMode                = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace               = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable         = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = { 0 };
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable         = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable  = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable   = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments    = &colorBlendAttachment;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount             = 1;
    pipelineLayoutInfo.pSetLayouts                = &app->descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount     = 0;
    pipelineLayoutInfo.pPushConstantRanges        = NULL;

    if ( vkCreatePipelineLayout( app->device, &pipelineLayoutInfo, NULL, &app->pipelineLayout ) !=
         VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create pipeline layout\n" );
      exit( 1 );
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount                   = 2;
    pipelineInfo.pStages                      = shaderStages;
    pipelineInfo.pVertexInputState            = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState          = &inputAssembly;
    pipelineInfo.pViewportState               = &viewportState;
    pipelineInfo.pRasterizationState          = &rasterizer;
    pipelineInfo.pMultisampleState            = &multisampling;
    pipelineInfo.pDepthStencilState           = NULL;
    pipelineInfo.pColorBlendState             = &colorBlending;
    pipelineInfo.pDynamicState                = NULL;
    pipelineInfo.layout                       = app->pipelineLayout;
    pipelineInfo.renderPass                   = app->renderPass;
    pipelineInfo.subpass                      = 0;
    pipelineInfo.basePipelineHandle           = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex            = -1;

    if ( vkCreateGraphicsPipelines( app->device,
                                    VK_NULL_HANDLE,
                                    1,
                                    &pipelineInfo,
                                    NULL,
                                    &app->graphicsPipeline ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create graphics pipeline\n" );
      exit( 1 );
    }

    vkDestroyShaderModule( app->device, fragShaderModule, NULL );
    vkDestroyShaderModule( app->device, vertShaderModule, NULL );
  }

  DK_VULKAN_FUNC void DK_vkCreateFramebuffers( DK_vkApplication *app )
  {
    app->swapChainFramebuffers = (VkFramebuffer *)malloc( app->imageCount * sizeof( VkFramebuffer ) );

    for ( uint32_t i = 0; i < app->imageCount; i++ )
    {
      VkImageView attachments[] = { app->swapChainImageViews[i] };

      VkFramebufferCreateInfo framebufferInfo = {};
      framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass              = app->renderPass;
      framebufferInfo.attachmentCount         = 1;
      framebufferInfo.pAttachments            = attachments;
      framebufferInfo.width                   = app->swapChainExtent.width;
      framebufferInfo.height                  = app->swapChainExtent.height;
      framebufferInfo.layers                  = 1;

      if ( vkCreateFramebuffer( app->device, &framebufferInfo, NULL, &app->swapChainFramebuffers[i] ) !=
           VK_SUCCESS )
      {
        fprintf( stderr, "Failed to create framebuffer\n" );
        exit( 1 );
      }
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateCommandPool( DK_vkApplication *app )
  {
    DK_vkQueueFamilyIndices queueFamilyIndices = DK_vkFindQueueFamilies( app->physicalDevice, app->surface );

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex        = queueFamilyIndices.graphicsFamily;
    poolInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if ( vkCreateCommandPool( app->device, &poolInfo, NULL, &app->commandPool ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create command pool\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateCommandBuffers( DK_vkApplication *app )
  {
    app->commandBuffers = (VkCommandBuffer *)malloc( app->imageCount * sizeof( VkCommandBuffer ) );

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool                 = app->commandPool;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount          = app->imageCount;

    if ( vkAllocateCommandBuffers( app->device, &allocInfo, app->commandBuffers ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to allocate command buffers\n" );
      exit( 1 );
    }

    for ( uint32_t i = 0; i < app->imageCount; i++ )
    {
      VkCommandBufferBeginInfo beginInfo = {};
      beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags                    = 0;
      beginInfo.pInheritanceInfo         = NULL;

      if ( vkBeginCommandBuffer( app->commandBuffers[i], &beginInfo ) != VK_SUCCESS )
      {
        fprintf( stderr, "Failed to begin recording command buffer\n" );
        exit( 1 );
      }

      VkRenderPassBeginInfo renderPassInfo = {};
      renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass            = app->renderPass;
      renderPassInfo.framebuffer           = app->swapChainFramebuffers[i];
      renderPassInfo.renderArea.offset.x   = 0;
      renderPassInfo.renderArea.offset.y   = 0;
      renderPassInfo.renderArea.extent     = app->swapChainExtent;

      VkClearValue clearColor        = { 0.0f, 0.0f, 0.0f, 1.0f };
      renderPassInfo.clearValueCount = 1;
      renderPassInfo.pClearValues    = &clearColor;

      vkCmdBeginRenderPass( app->commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );

      vkCmdBindPipeline( app->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, app->graphicsPipeline );

      VkBuffer     vertexBuffers[] = { app->vertexBuffer };
      VkDeviceSize offsets[]       = { 0 };
      vkCmdBindVertexBuffers( app->commandBuffers[i], 0, 1, vertexBuffers, offsets );

      vkCmdBindDescriptorSets( app->commandBuffers[i],
                               VK_PIPELINE_BIND_POINT_GRAPHICS,
                               app->pipelineLayout,
                               0,
                               1,
                               &app->descriptorSet,
                               0,
                               NULL );

      vkCmdDraw( app->commandBuffers[i], 3, 1, 0, 0 );
      vkCmdEndRenderPass( app->commandBuffers[i] );

      if ( vkEndCommandBuffer( app->commandBuffers[i] ) != VK_SUCCESS )
      {
        fprintf( stderr, "Failed to record command buffer\n" );
        exit( 1 );
      }
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateSyncObjects( DK_vkApplication *app )
  {
    app->imageAvailableSemaphores =
        (VkSemaphore *)malloc( DK_VULKAN_MAX_FRAMES_IN_FLIGHT * sizeof( VkSemaphore ) );
    app->renderFinishedSemaphores =
        (VkSemaphore *)malloc( DK_VULKAN_MAX_FRAMES_IN_FLIGHT * sizeof( VkSemaphore ) );
    app->inFlightFences = (VkFence *)malloc( DK_VULKAN_MAX_FRAMES_IN_FLIGHT * sizeof( VkFence ) );

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags             = VK_FENCE_CREATE_SIGNALED_BIT;

    for ( int32_t i = 0; i < DK_VULKAN_MAX_FRAMES_IN_FLIGHT; i++ )
    {
      if ( vkCreateSemaphore( app->device, &semaphoreInfo, NULL, &app->imageAvailableSemaphores[i] ) !=
               VK_SUCCESS ||
           vkCreateSemaphore( app->device, &semaphoreInfo, NULL, &app->renderFinishedSemaphores[i] ) !=
               VK_SUCCESS ||
           vkCreateFence( app->device, &fenceInfo, NULL, &app->inFlightFences[i] ) != VK_SUCCESS )
      {
        fprintf( stderr, "Failed to create synchronization objects\n" );
        exit( 1 );
      }
    }
  }

  DK_VULKAN_FUNC void DK_vkRecreateSwapChain( DK_vkApplication *app )
  {
    int32_t width = 0, height = 0;
    glfwGetFramebufferSize( app->window, &width, &height );
    while ( width == 0 || height == 0 )
    {
      glfwGetFramebufferSize( app->window, &width, &height );
      glfwWaitEvents();
    }

    vkDeviceWaitIdle( app->device );

    DK_vkCleanupSwapChain( app );
    DK_vkCreateSwapChain( app );
    DK_vkCreateImageViews( app );
    DK_vkCreateRenderPass( app );
    DK_vkCreateDescriptorSetLayoutEx( app );
    DK_vkCreateGraphicsPipeline( app );
    DK_vkCreateFramebuffers( app );

    vkDestroyDescriptorPool( app->device, app->descriptorPool, NULL );
    DK_vkCreateDescriptorPoolEx( app );
    DK_vkCreateDescriptorSetEx( app );

    DK_vkTexture *currentTexture = app->currentTexture;
    DK_vkInitializeTextureDescriptor( app );
    if ( currentTexture != NULL )
    {
      DK_vkUpdateDescriptorSetWithTexture( app, currentTexture );
    }

    DK_vkCreateCommandBuffers( app );

    app->batchRenderer.pipeline       = app->graphicsPipeline;
    app->batchRenderer.pipelineLayout = app->pipelineLayout;

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool                 = app->commandPool;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount          = 1;

    vkAllocateCommandBuffers( app->device, &allocInfo, &app->batchRenderer.commandBuffer );
    DK_vkOrthoCameraUpdate( app, width, height );

    app->batchRenderer.hasBegun    = false;
    app->batchRenderer.vertexCount = 0;
    app->batchRenderer.indexCount  = 0;
  }

  DK_VULKAN_FUNC void DK_vkCleanupSwapChain( DK_vkApplication *app )
  {
    for ( uint32_t i = 0; i < app->imageCount; i++ )
    {
      vkDestroyFramebuffer( app->device, app->swapChainFramebuffers[i], NULL );
    }

    vkFreeCommandBuffers( app->device, app->commandPool, app->imageCount, app->commandBuffers );
    vkDestroyPipeline( app->device, app->graphicsPipeline, NULL );
    vkDestroyPipelineLayout( app->device, app->pipelineLayout, NULL );
    vkDestroyRenderPass( app->device, app->renderPass, NULL );
    for ( uint32_t i = 0; i < app->imageCount; i++ )
    {
      vkDestroyImageView( app->device, app->swapChainImageViews[i], NULL );
    }

    vkDestroySwapchainKHR( app->device, app->swapChain, NULL );

    free( app->swapChainFramebuffers );
    free( app->swapChainImageViews );
    free( app->swapChainImages );
    free( app->commandBuffers );
  }

  DK_VULKAN_FUNC void DK_vkUpdateDescriptorSetLayout( DK_vkApplication *app )
  {
    VkDescriptorSetLayoutBinding bindings[2];

    bindings[0].binding            = 0;
    bindings[0].descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].descriptorCount    = 1;
    bindings[0].stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
    bindings[0].pImmutableSamplers = NULL;

    bindings[1].binding            = 1;
    bindings[1].descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[1].descriptorCount    = 1;
    bindings[1].stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
    bindings[1].pImmutableSamplers = NULL;

    VkDescriptorSetLayoutCreateInfo layoutInfo = {};
    layoutInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount                    = 2;
    layoutInfo.pBindings                       = bindings;

    if ( app->descriptorSetLayout != VK_NULL_HANDLE )
    {
      vkDestroyDescriptorSetLayout( app->device, app->descriptorSetLayout, NULL );
    }

    if ( vkCreateDescriptorSetLayout( app->device, &layoutInfo, NULL, &app->descriptorSetLayout ) !=
         VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create descriptor set layout\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC DK_vkTexture DK_vkLoadTexture( DK_vkApplication *app, const char *filename )
  {
    DK_vkTexture texture = { 0 };

    DK_vkCreateTextureImage( app,
                             filename,
                             &texture.image,
                             &texture.memory,
                             &texture.width,
                             &texture.height,
                             &texture.channels );
    DK_vkCreateTextureImageView( app, texture.image, &texture.view );
    DK_vkCreateTextureSampler( app, &texture.sampler );

    return texture;
  }

  DK_VULKAN_FUNC void DK_vkDestroyTexture( DK_vkApplication *app, DK_vkTexture *texture )
  {
    vkDeviceWaitIdle( app->device );

    vkDestroySampler( app->device, texture->sampler, NULL );
    vkDestroyImageView( app->device, texture->view, NULL );
    vkDestroyImage( app->device, texture->image, NULL );
    vkFreeMemory( app->device, texture->memory, NULL );
  }

  DK_VULKAN_FUNC void DK_vkCreateImage( DK_vkApplication     *app,
                                        uint32_t              width,
                                        uint32_t              height,
                                        VkFormat              format,
                                        VkImageTiling         tiling,
                                        VkImageUsageFlags     usage,
                                        VkMemoryPropertyFlags properties,
                                        VkImage              *image,
                                        VkDeviceMemory       *imageMemory )
  {
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType         = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width      = width;
    imageInfo.extent.height     = height;
    imageInfo.extent.depth      = 1;
    imageInfo.mipLevels         = 1;
    imageInfo.arrayLayers       = 1;
    imageInfo.format            = format;
    imageInfo.tiling            = tiling;
    imageInfo.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage             = usage;
    imageInfo.samples           = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode       = VK_SHARING_MODE_EXCLUSIVE;

    if ( vkCreateImage( app->device, &imageInfo, NULL, image ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create image\n" );
      exit( 1 );
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements( app->device, *image, &memRequirements );

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize       = memRequirements.size;
    allocInfo.memoryTypeIndex      = DK_vkFindMemoryType( app, memRequirements.memoryTypeBits, properties );

    if ( vkAllocateMemory( app->device, &allocInfo, NULL, imageMemory ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to allocate image memory\n" );
      exit( 1 );
    }

    vkBindImageMemory( app->device, *image, *imageMemory, 0 );
  }

  DK_VULKAN_FUNC void DK_vkCreateTextureImage( DK_vkApplication *app,
                                               const char       *filename,
                                               VkImage          *image,
                                               VkDeviceMemory   *imageMemory,
                                               int32_t          *width,
                                               int32_t          *height,
                                               int32_t          *channels )
  {

    int32_t      texWidth, texHeight, texChannels;
    stbi_uc     *pixels    = stbi_load( filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if ( !pixels )
    {
      fprintf( stderr, "Failed to load texture image: %s\n", filename );
      exit( 1 );
    }

    *width    = texWidth;
    *height   = texHeight;
    *channels = texChannels;

    VkBuffer       stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    DK_vkCreateBuffer( app,
                       imageSize,
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &stagingBuffer,
                       &stagingBufferMemory );

    void *data;
    vkMapMemory( app->device, stagingBufferMemory, 0, imageSize, 0, &data );
    memcpy( data, pixels, (size_t)imageSize );
    vkUnmapMemory( app->device, stagingBufferMemory );

    stbi_image_free( pixels );

    DK_vkCreateImage( app,
                      texWidth,
                      texHeight,
                      VK_FORMAT_R8G8B8A8_SRGB,
                      VK_IMAGE_TILING_OPTIMAL,
                      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                      image,
                      imageMemory );

    DK_vkTransitionImageLayout( app,
                                *image,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );

    DK_vkCopyBufferToImage( app, stagingBuffer, *image, (uint32_t)texWidth, (uint32_t)texHeight );

    DK_vkTransitionImageLayout( app,
                                *image,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

    vkDestroyBuffer( app->device, stagingBuffer, NULL );
    vkFreeMemory( app->device, stagingBufferMemory, NULL );
  }

  DK_VULKAN_FUNC void
  DK_vkCreateTextureImageView( DK_vkApplication *app, VkImage image, VkImageView *imageView )
  {
    VkImageViewCreateInfo viewInfo           = {};
    viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image                           = image;
    viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format                          = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel   = 0;
    viewInfo.subresourceRange.levelCount     = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount     = 1;

    if ( vkCreateImageView( app->device, &viewInfo, NULL, imageView ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create texture image view\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkCreateTextureSampler( DK_vkApplication *app, VkSampler *sampler )
  {
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType               = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;

    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy    = 1.0f;
    samplerInfo.borderColor      = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable           = VK_FALSE;

    samplerInfo.compareOp  = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod     = 0.0f;
    samplerInfo.maxLod     = 0.0f;

    if ( vkCreateSampler( app->device, &samplerInfo, NULL, sampler ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create texture sampler\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC VkCommandBuffer DK_vkBeginSingleTimeCommands( DK_vkApplication *app )
  {
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool                 = app->commandPool;
    allocInfo.commandBufferCount          = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers( app->device, &allocInfo, &commandBuffer );

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer( commandBuffer, &beginInfo );

    return commandBuffer;
  }

  DK_VULKAN_FUNC void DK_vkEndSingleTimeCommands( DK_vkApplication *app, VkCommandBuffer commandBuffer )
  {
    vkEndCommandBuffer( commandBuffer );

    VkSubmitInfo submitInfo       = {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit( app->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( app->graphicsQueue );

    vkFreeCommandBuffers( app->device, app->commandPool, 1, &commandBuffer );
  }

  DK_VULKAN_FUNC void DK_vkTransitionImageLayout( DK_vkApplication *app,
                                                  VkImage           image,
                                                  VkFormat          format,
                                                  VkImageLayout     oldLayout,
                                                  VkImageLayout     newLayout )
  {
    VkCommandBuffer commandBuffer = DK_vkBeginSingleTimeCommands( app );

    VkImageMemoryBarrier barrier            = {};
    barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout                       = oldLayout;
    barrier.newLayout                       = newLayout;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                           = image;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if ( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL )
    {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if ( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
              newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )
    {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
      fprintf( stderr, "Unsupported layout transition\n" );
      exit( 1 );
    }

    vkCmdPipelineBarrier( commandBuffer, sourceStage, destinationStage, 0, 0, NULL, 0, NULL, 1, &barrier );

    DK_vkEndSingleTimeCommands( app, commandBuffer );
  }

  DK_VULKAN_FUNC void DK_vkCopyBufferToImage( DK_vkApplication *app,
                                              VkBuffer          buffer,
                                              VkImage           image,
                                              uint32_t          width,
                                              uint32_t          height )
  {
    VkCommandBuffer commandBuffer = DK_vkBeginSingleTimeCommands( app );

    VkBufferImageCopy region               = { 0 };
    region.bufferOffset                    = 0;
    region.bufferRowLength                 = 0;
    region.bufferImageHeight               = 0;
    region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel       = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount     = 1;
    region.imageOffset                     = (VkOffset3D){ 0, 0, 0 };
    region.imageExtent                     = (VkExtent3D){ width, height, 1 };

    vkCmdCopyBufferToImage( commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );

    DK_vkEndSingleTimeCommands( app, commandBuffer );
  }

  // ========================================================================================
  // BATCH RENDERING IMPLEMENTATION
  // ========================================================================================

  DK_VULKAN_FUNC void DK_vkCreateBatchRenderer( DK_vkApplication *app )
  {
    DK_vkRenderer *renderer = &app->batchRenderer;

    renderer->vertexCount        = 0;
    renderer->indexCount         = 0;
    renderer->hasBegun           = false;
    renderer->vertexBuffer       = VK_NULL_HANDLE;
    renderer->vertexBufferMemory = VK_NULL_HANDLE;
    renderer->indexBuffer        = VK_NULL_HANDLE;
    renderer->indexBufferMemory  = VK_NULL_HANDLE;
    renderer->commandBuffer      = VK_NULL_HANDLE;
    renderer->vertexBufferMapped = NULL;
    renderer->indexBufferMapped  = NULL;
    renderer->currentTexture     = NULL;

    VkDeviceSize vertexBufferSize = sizeof( DK_Vertex ) * MAX_BATCH_VERTICES;

    DK_vkCreateBuffer( app,
                       vertexBufferSize,
                       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &renderer->vertexBuffer,
                       &renderer->vertexBufferMemory );

    vkMapMemory( app->device,
                 renderer->vertexBufferMemory,
                 0,
                 vertexBufferSize,
                 0,
                 (void **)&renderer->vertexBufferMapped );

    VkDeviceSize indexBufferSize = sizeof( uint32_t ) * MAX_BATCH_INDICES;

    DK_vkCreateBuffer( app,
                       indexBufferSize,
                       VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &renderer->indexBuffer,
                       &renderer->indexBufferMemory );

    vkMapMemory( app->device,
                 renderer->indexBufferMemory,
                 0,
                 indexBufferSize,
                 0,
                 (void **)&renderer->indexBufferMapped );

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool                 = app->commandPool;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount          = 1;

    vkAllocateCommandBuffers( app->device, &allocInfo, &renderer->commandBuffer );

    // NOTE (David) Batch renderer shares the existing pipeline for now, we need to create a specific pipeline for the batch renderer
    renderer->pipeline       = app->graphicsPipeline;
    renderer->pipelineLayout = app->pipelineLayout;
  }

  DK_VULKAN_FUNC void DK_vkDestroyBatchRenderer( DK_vkApplication *app )
  {
    DK_vkRenderer *renderer = &app->batchRenderer;

    if ( renderer->vertexBufferMapped != NULL )
    {
      vkUnmapMemory( app->device, renderer->vertexBufferMemory );
      renderer->vertexBufferMapped = NULL;
    }

    if ( renderer->indexBufferMapped != NULL )
    {
      vkUnmapMemory( app->device, renderer->indexBufferMemory );
      renderer->indexBufferMapped = NULL;
    }

    if ( renderer->vertexBuffer != VK_NULL_HANDLE )
    {
      vkDestroyBuffer( app->device, renderer->vertexBuffer, NULL );
      renderer->vertexBuffer = VK_NULL_HANDLE;
    }

    if ( renderer->vertexBufferMemory != VK_NULL_HANDLE )
    {
      vkFreeMemory( app->device, renderer->vertexBufferMemory, NULL );
      renderer->vertexBufferMemory = VK_NULL_HANDLE;
    }

    if ( renderer->indexBuffer != VK_NULL_HANDLE )
    {
      vkDestroyBuffer( app->device, renderer->indexBuffer, NULL );
      renderer->indexBuffer = VK_NULL_HANDLE;
    }

    if ( renderer->indexBufferMemory != VK_NULL_HANDLE )
    {
      vkFreeMemory( app->device, renderer->indexBufferMemory, NULL );
      renderer->indexBufferMemory = VK_NULL_HANDLE;
    }

    if ( renderer->commandBuffer != VK_NULL_HANDLE )
    {
      vkFreeCommandBuffers( app->device, app->commandPool, 1, &renderer->commandBuffer );
      renderer->commandBuffer = VK_NULL_HANDLE;
    }

    renderer->vertexCount    = 0;
    renderer->indexCount     = 0;
    renderer->hasBegun       = false;
    renderer->currentTexture = NULL;
  }

  DK_VULKAN_FUNC void DK_vkBeginBatch( DK_vkApplication *app )
  {
    DK_vkRenderer *renderer = &app->batchRenderer;

    if ( renderer->hasBegun )
    {
      fprintf( stderr, "Batch already in progress\n" );
      return;
    }

    vkQueueWaitIdle( app->graphicsQueue );
    vkResetCommandBuffer( renderer->commandBuffer, 0 );

    renderer->vertexCount = 0;
    renderer->indexCount  = 0;
    renderer->hasBegun    = true;

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if ( vkBeginCommandBuffer( renderer->commandBuffer, &beginInfo ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to begin command buffer for batch\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkEndBatch( DK_vkApplication *app )
  {
    DK_vkRenderer *renderer = &app->batchRenderer;
    if ( !renderer->hasBegun )
    {
      fprintf( stderr, "No batch in progress\n" );
      return;
    }

    if ( renderer->vertexCount > 0 )
    {
      DK_vkFlushBatch( app );
    }
    else
    {
      if ( renderer->hasBegun )
      {
        vkEndCommandBuffer( renderer->commandBuffer );
      }
    }

    renderer->hasBegun = false;
  }

  DK_VULKAN_FUNC void DK_vkSafeUnmapMemory( DK_vkApplication *app, VkDeviceMemory *memory, void **mappedData )
  {
    if ( app && app->device != VK_NULL_HANDLE && *memory != VK_NULL_HANDLE && *mappedData != NULL )
    {
      vkUnmapMemory( app->device, *memory );
      *mappedData = NULL;
    }
  }

  DK_VULKAN_FUNC uint32_t DK_vkAddVertex( DK_vkRenderer *renderer,
                                          float          x,
                                          float          y,
                                          float          r,
                                          float          g,
                                          float          b,
                                          float          a,
                                          float          u,
                                          float          v,
                                          int32_t        samplerId )
  {
    DK_Vertex *vertex = &renderer->vertexBufferMapped[renderer->vertexCount];

    vertex->pos[0]      = x;
    vertex->pos[1]      = y;
    vertex->color[0]    = r;
    vertex->color[1]    = g;
    vertex->color[2]    = b;
    vertex->color[3]    = a;
    vertex->texCoord[0] = u;
    vertex->texCoord[1] = v;
    vertex->samplerId   = samplerId;

    return renderer->vertexCount++;
  }

  DK_VULKAN_FUNC void DK_vkAddIndex( DK_vkRenderer *renderer, uint32_t index )
  {
    renderer->indexBufferMapped[renderer->indexCount++] = index;
  }

  DK_VULKAN_FUNC void
  DK_vkOrtho( float left, float right, float bottom, float top, float near, float far, DK_vkMat4 matrix )
  {
    memset( matrix, 0, sizeof( float ) * 16 );
    matrix[0][0] = 2.0f / ( right - left );
    matrix[1][1] = 2.0f / ( bottom - top );
    matrix[2][2] = -2.0f / ( far - near );
    matrix[3][0] = -( right + left ) / ( right - left );
    matrix[3][1] = -( bottom + top ) / ( bottom - top );
    matrix[3][2] = -( far + near ) / ( far - near );
    matrix[3][3] = 1.0f;
  }

  DK_VULKAN_FUNC void DK_vkIdentityMatrix( DK_vkMat4 matrix )
  {
    memset( matrix, 0, sizeof( float ) * 16 );
    matrix[0][0] = 1.0f;
    matrix[1][1] = 1.0f;
    matrix[2][2] = 1.0f;
    matrix[3][3] = 1.0f;
  }

  DK_VULKAN_FUNC void DK_vkOrthoCameraInit( DK_vkApplication *app, float width, float height )
  {
    float aspectRatio = width / height;
    float orthoWidth  = width;
    float orthoHeight = orthoWidth / aspectRatio;

    app->camera.zoom = 1.0f;
    app->camera.near = -1.0f;
    app->camera.far  = 1.0f;

    app->camera.left   = 0;
    app->camera.right  = orthoWidth;
    app->camera.top    = 0;
    app->camera.bottom = orthoHeight;
  }

  DK_VULKAN_FUNC void DK_vkOrthoCameraUpdate( DK_vkApplication *app, float width, float height )
  {
    float aspectRatio = width / height;
    float orthoWidth  = width;
    float orthoHeight = orthoWidth / aspectRatio;

    app->camera.left   = 0;
    app->camera.right  = orthoWidth;
    app->camera.top    = 0;
    app->camera.bottom = orthoHeight;
  }

  DK_VULKAN_FUNC void DK_vkZoomCamera( DK_vkApplication *app, float factor )
  {
    app->camera.zoom *= factor;
    if ( app->camera.zoom < 0.1f )
    {
      app->camera.zoom = 0.1f;
    }
    if ( app->camera.zoom > 10.0f )
    {
      app->camera.zoom = 10.0f;
    }

    int32_t width, height;
    glfwGetFramebufferSize( app->window, &width, &height );
    DK_vkOrthoCameraUpdate( app, (float)width, (float)height );
  }

  DK_VULKAN_FUNC void DK_vkCreateDummyTexture( DK_vkApplication *app )
  {
    if ( app->textureCount > 0 )
    {
      return;
    }

    if ( app->textures == NULL )
    {
      app->maxTextures  = DK_VK_MAX_TEXTURES;
      app->textureCount = 0;
      app->textures     = (DK_vkTexture *)malloc( sizeof( DK_vkTexture ) * app->maxTextures );
      if ( app->textures == NULL )
      {
        fprintf( stderr, "Failed to allocate memory for textures array\n" );
        exit( 1 );
      }
    }

    uint8_t      whitePixel[4] = { 255, 255, 255, 255 };
    VkDeviceSize imageSize     = 4; // 1x1 RGBA

    VkBuffer       stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    DK_vkCreateBuffer( app,
                       imageSize,
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &stagingBuffer,
                       &stagingBufferMemory );

    void *data;
    vkMapMemory( app->device, stagingBufferMemory, 0, imageSize, 0, &data );
    memcpy( data, whitePixel, imageSize );
    vkUnmapMemory( app->device, stagingBufferMemory );

    DK_vkTexture dummyTexture = { 0 };
    dummyTexture.width        = 1;
    dummyTexture.height       = 1;
    dummyTexture.channels     = 4;

    DK_vkCreateImage( app,
                      1,
                      1,
                      VK_FORMAT_R8G8B8A8_SRGB,
                      VK_IMAGE_TILING_OPTIMAL,
                      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                      &dummyTexture.image,
                      &dummyTexture.memory );

    DK_vkTransitionImageLayout( app,
                                dummyTexture.image,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );

    DK_vkCopyBufferToImage( app, stagingBuffer, dummyTexture.image, 1, 1 );

    DK_vkTransitionImageLayout( app,
                                dummyTexture.image,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

    DK_vkCreateTextureImageView( app, dummyTexture.image, &dummyTexture.view );

    DK_vkCreateTextureSampler( app, &dummyTexture.sampler );

    vkDestroyBuffer( app->device, stagingBuffer, NULL );
    vkFreeMemory( app->device, stagingBufferMemory, NULL );

    app->textures[0]                  = dummyTexture;
    app->textureCount                 = 1;
    app->currentTexture               = &app->textures[0];
    app->batchRenderer.currentTexture = app->currentTexture;

    DK_vkUpdateDescriptorSetWithTexture( app, &app->textures[0] );
  }

  DK_VULKAN_FUNC void DK_vkInitTextureSystem( DK_vkApplication *app )
  {
    app->maxTextures        = DK_VK_MAX_TEXTURES;
    app->textureCount       = 0;
    app->activeTextureCount = 0;

    if ( app->textures != NULL )
    {
      free( app->textures );
    }

    app->textures = (DK_vkTexture *)malloc( sizeof( DK_vkTexture ) * DK_VK_MAX_TEXTURES );
    if ( app->textures == NULL )
    {
      fprintf( stderr, "Failed to allocate memory for textures array\n" );
      exit( 1 );
    }

    app->currentTexture = NULL;
    DK_vkCreateDummyTexture( app );
  }

  DK_VULKAN_FUNC void DK_vkUpdateDescriptorSetsWithActiveTextures( DK_vkApplication *app )
  {
    if ( app->activeTextureCount == 0 )
    {
      return;
    }

    VkDescriptorImageInfo *imageInfos = malloc( app->activeTextureCount * sizeof( VkDescriptorImageInfo ) );

    uint32_t activeIndex = 0;
    for ( uint32_t i = 0; i < app->textureCount; i++ )
    {
      if ( app->textures[i].isActive )
      {
        imageInfos[activeIndex].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfos[activeIndex].imageView   = app->textures[i].view;
        imageInfos[activeIndex].sampler     = app->textures[i].sampler;
        activeIndex++;
      }
    }

    VkDescriptorBufferInfo bufferInfo = { 0 };
    bufferInfo.buffer                 = app->uniformBuffer;
    bufferInfo.offset                 = 0;
    bufferInfo.range                  = sizeof( DK_vkUniformBufferObject );

    VkWriteDescriptorSet descriptorWrites[2] = {};

    descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet          = app->descriptorSet;
    descriptorWrites[0].dstBinding      = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo     = &bufferInfo;

    descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet          = app->descriptorSet;
    descriptorWrites[1].dstBinding      = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = app->activeTextureCount;
    descriptorWrites[1].pImageInfo      = imageInfos;

    vkUpdateDescriptorSets( app->device, 2, descriptorWrites, 0, NULL );

    free( imageInfos );
  }

  DK_VULKAN_FUNC uint32_t DK_vkAddTexture( DK_vkApplication *app, const char *filename )
  {
    if ( app->textureCount >= app->maxTextures )
    {
      fprintf( stderr, "Maximum texture count reached\n" );
      return 0;
    }

    uint32_t textureId                 = app->textureCount++;
    app->textures[textureId]           = DK_vkLoadTexture( app, filename );
    app->textures[textureId].samplerId = textureId;

    DK_vkUpdateDescriptorSetWithTextures( app );

    return textureId;
  }

  DK_VULKAN_FUNC void DK_vkSetTextureActive( DK_vkApplication *app, uint32_t textureId, bool active )
  {
    if ( textureId < app->textureCount )
    {
      if ( active && !app->textures[textureId].isActive )
      {
        app->textures[textureId].isActive = true;
        app->activeTextureCount++;
      }
      else if ( !active && app->textures[textureId].isActive )
      {
        app->textures[textureId].isActive = false;
        if ( app->activeTextureCount > 0 )
        {
          app->activeTextureCount--;
        }
      }

      DK_vkUpdateDescriptorSetsWithActiveTextures( app );
    }
  }

  DK_VULKAN_FUNC void DK_vkSetTexture( DK_vkApplication *app, uint32_t textureId )
  {
    if ( textureId < app->textureCount )
    {
      app->currentTexture               = &app->textures[textureId];
      app->batchRenderer.currentTexture = app->currentTexture;

      DK_vkUpdateDescriptorSetWithTexture( app, app->currentTexture );
    }
  }

  DK_VULKAN_FUNC void DK_vkUpdateDescriptorSetWithTexture( DK_vkApplication *app, DK_vkTexture *texture )
  {
    if ( !texture )
    {
      return;
    }

    VkDescriptorBufferInfo bufferInfo = { 0 };
    bufferInfo.buffer                 = app->uniformBuffer;
    bufferInfo.offset                 = 0;
    bufferInfo.range                  = sizeof( DK_vkUniformBufferObject );

    VkDescriptorImageInfo imageInfo = { 0 };
    imageInfo.imageLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView             = texture->view;
    imageInfo.sampler               = texture->sampler;

    VkWriteDescriptorSet descriptorWrites[2] = {};

    descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet          = app->descriptorSet;
    descriptorWrites[0].dstBinding      = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo     = &bufferInfo;

    descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet          = app->descriptorSet;
    descriptorWrites[1].dstBinding      = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo      = &imageInfo;

    vkUpdateDescriptorSets( app->device, 2, descriptorWrites, 0, NULL );
  }

  DK_VULKAN_FUNC void DK_vkCreateDescriptorPoolWithTextures( DK_vkApplication *app )
  {
    VkDescriptorPoolSize poolSizes[2] = {};

    poolSizes[0].type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = 1;

    poolSizes[1].type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = app->maxTextures;

    VkDescriptorPoolCreateInfo poolInfo = { 0 };
    poolInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount              = 2;
    poolInfo.pPoolSizes                 = poolSizes;
    poolInfo.maxSets                    = 1;

    if ( vkCreateDescriptorPool( app->device, &poolInfo, NULL, &app->descriptorPool ) != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to create descriptor pool\n" );
      exit( 1 );
    }
  }

  DK_VULKAN_FUNC void DK_vkFlushBatch( DK_vkApplication *app )
  {
    DK_vkRenderer *renderer = &app->batchRenderer;

    if ( renderer->vertexCount == 0 )
    {
      return;
    }

    if ( app->framebufferResized )
    {
      if ( renderer->hasBegun )
      {
        vkEndCommandBuffer( renderer->commandBuffer );
        renderer->hasBegun = false;
      }

      vkDeviceWaitIdle( app->device );
      app->framebufferResized = false;
      DK_vkRecreateSwapChain( app );

      if ( renderer->vertexCount > 0 )
      {
        DK_vkBeginBatch( app );
      }
      return;
    }

    if ( renderer->currentTexture )
    {
      DK_vkUpdateDescriptorSetWithTexture( app, renderer->currentTexture );
    }
    else if ( app->textureCount > 0 )
    {
      DK_vkUpdateDescriptorSetWithTexture( app, &app->textures[0] );
    }
    else
    {
      // if no texture is available, create a dummy texture
      DK_vkCreateDummyTexture( app );
      DK_vkUpdateDescriptorSetWithTexture( app, &app->textures[0] );
    }

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR( app->device,
                                             app->swapChain,
                                             UINT64_MAX,
                                             app->imageAvailableSemaphores[app->currentFrame],
                                             VK_NULL_HANDLE,
                                             &imageIndex );

    if ( result == VK_ERROR_OUT_OF_DATE_KHR )
    {
      app->framebufferResized = true;
      return;
    }
    else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR )
    {
      fprintf( stderr, "Failed to acquire swap chain image\n" );
      return;
    }

    vkWaitForFences( app->device, 1, &app->inFlightFences[app->currentFrame], VK_TRUE, UINT64_MAX );
    vkResetFences( app->device, 1, &app->inFlightFences[app->currentFrame] );

    VkRenderPassBeginInfo renderPassInfo = { 0 };
    renderPassInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass            = app->renderPass;
    renderPassInfo.framebuffer           = app->swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset.x   = 0;
    renderPassInfo.renderArea.offset.y   = 0;
    renderPassInfo.renderArea.extent     = app->swapChainExtent;

    VkClearValue clearColor        = { 0.0f, 0.0f, 0.0f, 1.0f };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues    = &clearColor;

    vkCmdBeginRenderPass( renderer->commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );
    vkCmdBindPipeline( renderer->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->pipeline );

    vkCmdBindDescriptorSets( renderer->commandBuffer,
                             VK_PIPELINE_BIND_POINT_GRAPHICS,
                             renderer->pipelineLayout,
                             0,
                             1,
                             &app->descriptorSet,
                             0,
                             NULL );

    VkBuffer     vertexBuffers[] = { renderer->vertexBuffer };
    VkDeviceSize offsets[]       = { 0 };
    vkCmdBindVertexBuffers( renderer->commandBuffer, 0, 1, vertexBuffers, offsets );
    vkCmdBindIndexBuffer( renderer->commandBuffer, renderer->indexBuffer, 0, VK_INDEX_TYPE_UINT32 );

    vkCmdDrawIndexed( renderer->commandBuffer, renderer->indexCount, 1, 0, 0, 0 );
    vkCmdEndRenderPass( renderer->commandBuffer );
    vkEndCommandBuffer( renderer->commandBuffer );

    VkSubmitInfo submitInfo = {};
    submitInfo.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore          waitSemaphores[] = { app->imageAvailableSemaphores[app->currentFrame] };
    VkPipelineStageFlags waitStages[]     = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &renderer->commandBuffer;

    VkSemaphore signalSemaphores[]  = { app->renderFinishedSemaphores[app->currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    if ( vkQueueSubmit( app->graphicsQueue, 1, &submitInfo, app->inFlightFences[app->currentFrame] ) !=
         VK_SUCCESS )
    {
      fprintf( stderr, "Failed to submit batch draw command buffer\n" );
      exit( 1 );
    }

    VkPresentInfoKHR presentInfo   = {};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;

    VkSwapchainKHR swapChains[] = { app->swapChain };
    presentInfo.swapchainCount  = 1;
    presentInfo.pSwapchains     = swapChains;
    presentInfo.pImageIndices   = &imageIndex;

    result = vkQueuePresentKHR( app->presentQueue, &presentInfo );

    if ( result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || app->framebufferResized )
    {
      app->framebufferResized = false;
      DK_vkRecreateSwapChain( app );
    }
    else if ( result != VK_SUCCESS )
    {
      fprintf( stderr, "Failed to present swap chain image\n" );
    }

    app->currentFrame     = ( app->currentFrame + 1 ) % DK_VULKAN_MAX_FRAMES_IN_FLIGHT;
    renderer->vertexCount = 0;
    renderer->indexCount  = 0;
  }

  DK_VULKAN_FUNC void DK_vkCleanupTextureSystem( DK_vkApplication *app )
  {

    vkDeviceWaitIdle( app->device );
    DK_vkCleanupSwapChain( app );

    if ( app->batchRenderer.vertexBuffer != VK_NULL_HANDLE )
    {
      DK_vkDestroyBatchRenderer( app );
    }

    if ( app->textures != NULL )
    {
      DK_vkCleanupTextureSystem( app );
    }

    app->currentTexture               = NULL;
    app->batchRenderer.currentTexture = NULL;

    for ( uint32_t i = 0; i < app->textureCount; i++ )
    {
      if ( app->textures[i].sampler != VK_NULL_HANDLE )
      {
        vkDestroySampler( app->device, app->textures[i].sampler, NULL );
        app->textures[i].sampler = VK_NULL_HANDLE;
      }

      if ( app->textures[i].view != VK_NULL_HANDLE )
      {
        vkDestroyImageView( app->device, app->textures[i].view, NULL );
        app->textures[i].view = VK_NULL_HANDLE;
      }

      if ( app->textures[i].image != VK_NULL_HANDLE )
      {
        vkDestroyImage( app->device, app->textures[i].image, NULL );
        app->textures[i].image = VK_NULL_HANDLE;
      }

      if ( app->textures[i].memory != VK_NULL_HANDLE )
      {
        vkFreeMemory( app->device, app->textures[i].memory, NULL );
        app->textures[i].memory = VK_NULL_HANDLE;
      }
    }

    free( app->textures );
    app->textures     = NULL;
    app->textureCount = 0;
    app->maxTextures  = 0;
  }

  DK_VULKAN_FUNC void
  DK_vkDrawTriangle( DK_vkApplication *app, DK_vkVec2 p1, DK_vkVec2 p2, DK_vkVec2 p3, DK_vkColor tint )
  {
    DK_vkRenderer *renderer = &app->batchRenderer;
    if ( !renderer->hasBegun )
    {
      DK_vkBeginBatch( app );
    }

    if ( renderer->vertexCount + 3 > MAX_BATCH_VERTICES || renderer->indexCount + 3 > MAX_BATCH_INDICES )
    {
      DK_vkFlushBatch( app );
      DK_vkBeginBatch( app );
    }

    uint32_t baseIndex = renderer->vertexCount;

    DK_vkAddVertex( renderer, p1[0], p1[1], tint[0], tint[1], tint[2], tint[3], 0.0f, 0.0f, -1 );
    DK_vkAddVertex( renderer, p2[0], p2[1], tint[0], tint[1], tint[2], tint[3], 1.0f, 0.0f, -1 );
    DK_vkAddVertex( renderer, p3[0], p3[1], tint[0], tint[1], tint[2], tint[3], 0.5f, 1.0f, -1 );

    if ( renderer->indexCount + 3 <= MAX_BATCH_INDICES )
    {
      DK_vkAddIndex( renderer, baseIndex );
      DK_vkAddIndex( renderer, baseIndex + 1 );
      DK_vkAddIndex( renderer, baseIndex + 2 );
    }
    else
    {
      printf( "Warning: Index buffer overflow in DK_vkDrawTriangle!\n" );
    }
  }

  DK_VULKAN_FUNC void DK_vkDrawQuad( DK_vkApplication *app,
                                     DK_vkVec2         p1,
                                     DK_vkVec2         p2,
                                     DK_vkVec2         p3,
                                     DK_vkVec2         p4,
                                     DK_vkColor        tint )
  {

    DK_vkRenderer *renderer = &app->batchRenderer;
    if ( !renderer->hasBegun )
    {
      DK_vkBeginBatch( app );
    }

    if ( renderer->vertexCount + 4 > MAX_BATCH_VERTICES || renderer->indexCount + 6 > MAX_BATCH_INDICES )
    {
      DK_vkFlushBatch( app );
      DK_vkBeginBatch( app );
    }

    uint32_t baseIndex = renderer->vertexCount;
    DK_vkAddVertex( renderer, p1[0], p1[1], tint[0], tint[1], tint[2], tint[3], 0.0f, 0.0f, -1 );
    DK_vkAddVertex( renderer, p2[0], p2[1], tint[0], tint[1], tint[2], tint[3], 1.0f, 0.0f, -1 );
    DK_vkAddVertex( renderer, p3[0], p3[1], tint[0], tint[1], tint[2], tint[3], 1.0f, 1.0f, -1 );
    DK_vkAddVertex( renderer, p4[0], p4[1], tint[0], tint[1], tint[2], tint[3], 0.0f, 1.0f, -1 );

    if ( renderer->indexCount + 6 <= MAX_BATCH_INDICES )
    {
      DK_vkAddIndex( renderer, baseIndex );
      DK_vkAddIndex( renderer, baseIndex + 1 );
      DK_vkAddIndex( renderer, baseIndex + 2 );

      DK_vkAddIndex( renderer, baseIndex );
      DK_vkAddIndex( renderer, baseIndex + 2 );
      DK_vkAddIndex( renderer, baseIndex + 3 );
    }
    else
    {
      printf( "Warning: Index buffer overflow in DK_vkDrawQuad!\n" );
    }
  }

  DK_VULKAN_FUNC void
  DK_vkDrawRectangle( DK_vkApplication *app, DK_vkVec2 position, DK_vkSize size, DK_vkColor tint )
  {

    DK_vkVec2 p1 = { position[0], position[1] };
    DK_vkVec2 p2 = { position[0] + size[0], position[1] };
    DK_vkVec2 p3 = { position[0] + size[0], position[1] + size[1] };
    DK_vkVec2 p4 = { position[0], position[1] + size[1] };
    DK_vkDrawQuad( app, p1, p2, p3, p4, tint );
  }

  DK_VULKAN_FUNC void DK_vkDrawRoundedRectangle( DK_vkApplication *app,
                                                 DK_vkVec2         position,
                                                 DK_vkSize         size,
                                                 float             radius,
                                                 DK_vkColor        tint,
                                                 int32_t           segments )
  {
    if ( segments < 4 )
    {
      segments = 4;
    }

    float x = position[0];
    float y = position[1];

    float width  = size[0];
    float height = size[1];

    float max_radius = fminf( width, height ) * 0.5f;
    radius           = fminf( radius, max_radius );

    DK_vkDrawRectangle( app, (DK_vkVec2){ x + radius, y }, (DK_vkSize){ width - 2 * radius, height }, tint );

    // l and r rectangles
    DK_vkDrawRectangle( app, (DK_vkVec2){ x, y + radius }, (DK_vkSize){ radius, height - 2 * radius }, tint );
    DK_vkDrawRectangle( app,
                        (DK_vkVec2){ x + width - radius, y + radius },
                        (DK_vkSize){ radius, height - 2 * radius },
                        tint );

    // arch
    int32_t segmentsPerCorner = segments / 4;
    for ( int32_t i = 0; i < segmentsPerCorner; i++ )
    {
      float startAngle = M_PI + ( i * M_PI / 2 ) / segmentsPerCorner;
      float endAngle   = M_PI + ( ( i + 1 ) * M_PI / 2 ) / segmentsPerCorner;

      float startX = x + radius + radius * cosf( startAngle );
      float startY = y + radius + radius * sinf( startAngle );
      float endX   = x + radius + radius * cosf( endAngle );
      float endY   = y + radius + radius * sinf( endAngle );

      DK_vkVec2 p1 = { x + radius, y + radius };
      DK_vkVec2 p2 = { startX, startY };
      DK_vkVec2 p3 = { endX, endY };
      DK_vkDrawTriangle( app, p1, p2, p3, tint );
    }

    // tr corner
    for ( int32_t i = 0; i < segmentsPerCorner; i++ )
    {
      float startAngle = M_PI * 1.5f + ( i * M_PI / 2 ) / segmentsPerCorner;
      float endAngle   = M_PI * 1.5f + ( ( i + 1 ) * M_PI / 2 ) / segmentsPerCorner;

      float startX = x + width - radius + radius * cosf( startAngle );
      float startY = y + radius + radius * sinf( startAngle );
      float endX   = x + width - radius + radius * cosf( endAngle );
      float endY   = y + radius + radius * sinf( endAngle );

      DK_vkVec2 p1 = { x + width - radius, y + radius };
      DK_vkVec2 p2 = { startX, startY };
      DK_vkVec2 p3 = { endX, endY };

      DK_vkDrawTriangle( app, p1, p2, p3, tint );
    }

    // br corner
    for ( int32_t i = 0; i < segmentsPerCorner; i++ )
    {
      float startAngle = 0.0f + ( i * M_PI / 2 ) / segmentsPerCorner;
      float endAngle   = 0.0f + ( ( i + 1 ) * M_PI / 2 ) / segmentsPerCorner;

      float startX = x + width - radius + radius * cosf( startAngle );
      float startY = y + height - radius + radius * sinf( startAngle );
      float endX   = x + width - radius + radius * cosf( endAngle );
      float endY   = y + height - radius + radius * sinf( endAngle );

      DK_vkVec2 p1 = { x + width - radius, y + height - radius };
      DK_vkVec2 p2 = { startX, startY };
      DK_vkVec2 p3 = { endX, endY };

      DK_vkDrawTriangle( app, p1, p2, p3, tint );
    }

    // bl corner
    for ( int32_t i = 0; i < segmentsPerCorner; i++ )
    {
      float startAngle = M_PI / 2 + ( i * M_PI / 2 ) / segmentsPerCorner;
      float endAngle   = M_PI / 2 + ( ( i + 1 ) * M_PI / 2 ) / segmentsPerCorner;

      float startX = x + radius + radius * cosf( startAngle );
      float startY = y + height - radius + radius * sinf( startAngle );
      float endX   = x + radius + radius * cosf( endAngle );
      float endY   = y + height - radius + radius * sinf( endAngle );

      DK_vkVec2 p1 = { x + radius, y + height - radius };
      DK_vkVec2 p2 = { startX, startY };
      DK_vkVec2 p3 = { endX, endY };

      DK_vkDrawTriangle( app, p1, p2, p3, tint );
    }
  }

  DK_VULKAN_FUNC void DK_vkDrawGradientRectangle( DK_vkApplication *app,
                                                  DK_vkVec2         position,
                                                  DK_vkSize         size,
                                                  DK_vkColor        tint_bl,
                                                  DK_vkColor        tint_br,
                                                  DK_vkColor        tint_tr,
                                                  DK_vkColor        tint_tl )
  {

    DK_vkRenderer *renderer = &app->batchRenderer;

    if ( !renderer->hasBegun )
    {
      DK_vkBeginBatch( app );
    }

    if ( renderer->vertexCount + 4 > MAX_BATCH_VERTICES || renderer->indexCount + 6 > MAX_BATCH_INDICES )
    {
      DK_vkFlushBatch( app );
      DK_vkBeginBatch( app );
    }

    DK_vkVec2 p1 = { position[0], position[1] };
    DK_vkVec2 p2 = { position[0] + size[0], position[1] };
    DK_vkVec2 p3 = { position[0] + size[0], position[1] + size[1] };
    DK_vkVec2 p4 = { position[0], position[1] + size[1] };

    uint32_t baseIndex = renderer->vertexCount;

    // bl
    uint32_t   idx1 = renderer->vertexCount;
    DK_Vertex *v1   = &renderer->vertexBufferMapped[renderer->vertexCount++];
    v1->pos[0]      = p1[0];
    v1->pos[1]      = p1[1];
    memcpy( v1->color, tint_bl, sizeof( float ) * 4 );
    v1->texCoord[0] = 0.0f;
    v1->texCoord[1] = 0.0f;

    // br
    uint32_t   idx2 = renderer->vertexCount;
    DK_Vertex *v2   = &renderer->vertexBufferMapped[renderer->vertexCount++];
    v2->pos[0]      = p2[0];
    v2->pos[1]      = p2[1];
    memcpy( v2->color, tint_br, sizeof( float ) * 4 );
    v2->texCoord[0] = 1.0f;
    v2->texCoord[1] = 0.0f;

    // tr
    uint32_t   idx3 = renderer->vertexCount;
    DK_Vertex *v3   = &renderer->vertexBufferMapped[renderer->vertexCount++];
    v3->pos[0]      = p3[0];
    v3->pos[1]      = p3[1];
    memcpy( v1->color, tint_tr, sizeof( float ) * 4 );
    v3->texCoord[0] = 1.0f;
    v3->texCoord[1] = 1.0f;

    // tl
    uint32_t   idx4 = renderer->vertexCount;
    DK_Vertex *v4   = &renderer->vertexBufferMapped[renderer->vertexCount++];
    v4->pos[0]      = p4[0];
    v4->pos[1]      = p4[1];
    memcpy( v1->color, tint_tl, sizeof( float ) * 4 );
    v4->texCoord[0] = 0.0f;
    v4->texCoord[1] = 1.0f;

    if ( renderer->indexCount + 6 <= MAX_BATCH_INDICES )
    {
      DK_vkAddIndex( renderer, idx1 );
      DK_vkAddIndex( renderer, idx2 );
      DK_vkAddIndex( renderer, idx3 );

      DK_vkAddIndex( renderer, idx1 );
      DK_vkAddIndex( renderer, idx3 );
      DK_vkAddIndex( renderer, idx4 );
    }
    else
    {
      printf( "Warning: Index buffer overflow in DK_vkDrawGradientRectangle!\n" );
    }
  }

  DK_VULKAN_FUNC void DK_vkDrawCircle( DK_vkApplication *app,
                                       DK_vkVec2         position,
                                       float             radius,
                                       DK_vkColor        tint,
                                       int32_t           segments )
  {
    if ( segments < 3 )
    {
      segments = 3;
    }

    float vertices[segments * 2];

    for ( int32_t i = 0; i < segments; i++ )
    {
      float angle         = i * ( 2.0f * M_PI / segments );
      vertices[i * 2]     = position[0] + radius * cosf( angle );
      vertices[i * 2 + 1] = position[1] + radius * sinf( angle );
    }

    for ( int32_t i = 0; i < segments; i++ )
    {
      int32_t nextI = ( i + 1 ) % segments;

      DK_vkColor p1 = { position[0], position[1] };
      DK_vkColor p2 = { vertices[i * 2], vertices[i * 2 + 1] };
      DK_vkColor p3 = { vertices[nextI * 2], vertices[nextI * 2 + 1] };

      DK_vkDrawTriangle( app, p1, p2, p3, tint );
    }
  }

  DK_VULKAN_FUNC void
  DK_vkDrawLine( DK_vkApplication *app, DK_vkVec2 begin, DK_vkVec2 end, float thickness, DK_vkColor tint )
  {
    float dx     = end[0] - begin[0];
    float dy     = end[1] - begin[1];
    float length = sqrtf( dx * dx + dy * dy );

    if ( length > 0.0f )
    {
      dx /= length;
      dy /= length;
    }

    float px = -dy;
    float py = dx;

    float halfThickness = thickness * 0.5f;

    // tl
    float tlx = begin[0] + px * halfThickness;
    float tly = begin[1] + py * halfThickness;

    // tr
    float trx = end[0] + px * halfThickness;
    float try = end[1] + py * halfThickness;

    // br
    float brx = end[0] - px * halfThickness;
    float bry = end[1] - py * halfThickness;

    // bl
    float blx = begin[0] - px * halfThickness;
    float bly = begin[1] - py * halfThickness;

    DK_vkVec2 p1 = { tlx, tly };
    DK_vkVec2 p2 = { trx, try };
    DK_vkVec2 p3 = { brx, bry };
    DK_vkVec2 p4 = { blx, bly };

    DK_vkDrawQuad( app, p1, p2, p3, p4, tint );
  }

  DK_VULKAN_FUNC void DK_vkDrawTexturedQuad( DK_vkApplication *app,
                                             DK_vkVec2         position,
                                             DK_vkSize         size,
                                             DK_vkVec2         uv1,
                                             DK_vkVec2         uv2,
                                             DK_vkColor        tint,
                                             int32_t           samplerId )
  {
    DK_vkRenderer *renderer = &app->batchRenderer;
    if ( !renderer->hasBegun )
    {
      DK_vkBeginBatch( app );
    }

    if ( renderer->vertexCount + 4 > MAX_BATCH_VERTICES || renderer->indexCount + 6 > MAX_BATCH_INDICES )
    {
      DK_vkFlushBatch( app );
      DK_vkBeginBatch( app );
    }

    DK_vkVec2 p1 = { position[0], position[1] };
    DK_vkVec2 p2 = { position[0] + size[0], position[1] };
    DK_vkVec2 p3 = { position[0] + size[0], position[1] + size[1] };
    DK_vkVec2 p4 = { position[0], position[1] + size[1] };

    uint32_t baseIndex = renderer->vertexCount;
    DK_vkAddVertex( renderer,
                    p1[0],
                    p1[1],
                    tint[0],
                    tint[1],
                    tint[2],
                    tint[3],
                    uv1[0],
                    uv1[1],
                    samplerId ); // bl
    DK_vkAddVertex( renderer,
                    p2[0],
                    p2[1],
                    tint[0],
                    tint[1],
                    tint[2],
                    tint[3],
                    uv2[0],
                    uv1[1],
                    samplerId ); // br
    DK_vkAddVertex( renderer,
                    p3[0],
                    p3[1],
                    tint[0],
                    tint[1],
                    tint[2],
                    tint[3],
                    uv2[0],
                    uv2[1],
                    samplerId ); // tr
    DK_vkAddVertex( renderer,
                    p4[0],
                    p4[1],
                    tint[0],
                    tint[1],
                    tint[2],
                    tint[3],
                    uv1[0],
                    uv2[1],
                    samplerId ); // tl

    if ( renderer->indexCount + 6 <= MAX_BATCH_INDICES )
    {
      DK_vkAddIndex( renderer, baseIndex + 0 );
      DK_vkAddIndex( renderer, baseIndex + 1 );
      DK_vkAddIndex( renderer, baseIndex + 2 );

      DK_vkAddIndex( renderer, baseIndex + 0 );
      DK_vkAddIndex( renderer, baseIndex + 2 );
      DK_vkAddIndex( renderer, baseIndex + 3 );
    }
    else
    {
      printf( "Warning: Index buffer overflow in DK_vkDrawTexturedQuad!\n" );
    }
  }

  DK_VULKAN_FUNC void DK_vkDrawTexture( DK_vkApplication *app,
                                        DK_vkVec2         position,
                                        uint32_t          textureId,
                                        float             scale,
                                        DK_vkColor        tint )
  {
    if ( textureId >= app->textureCount )
    {
      fprintf( stderr, "Invalid texture ID\n" );
      return;
    }

    DK_vkSetTexture( app, textureId );
    DK_vkTexture *texture = &app->textures[textureId];
    float         width   = texture->width * scale;
    float         height  = texture->height * scale;

    DK_vkSize size = { width, height };
    DK_vkVec2 uv1  = { 0.0f, 0.0f };
    DK_vkVec2 uv2  = { 1.0f, 1.0f };

    DK_vkDrawTexturedQuad( app, position, size, uv1, uv2, tint, textureId );
  }

  DK_VULKAN_FUNC void DK_vkDrawTextureRegion( DK_vkApplication *app,
                                              DK_vkVec2         position,
                                              DK_vkSize         size,
                                              uint32_t          textureId,
                                              DK_vkVec2         region_position,
                                              DK_vkSize         region_size,
                                              DK_vkColor        tint )
  {
    if ( textureId >= app->textureCount )
    {
      fprintf( stderr, "Invalid texture ID\n" );
      return;
    }

    DK_vkSetTexture( app, textureId );
    DK_vkTexture *texture = &app->textures[textureId];

    DK_vkVec2 uv1 = { region_position[0] / texture->width, region_position[1] / texture->height };
    DK_vkVec2 uv2 = { uv1[0] + region_size[0] / texture->width, uv1[1] + region_size[1] / texture->height };

    DK_vkDrawTexturedQuad( app, position, size, uv1, uv2, tint, textureId );
  }

  DK_VULKAN_FUNC DK_vkFont DK_vkLoadFont( DK_vkApplication *app, const char *filename, int32_t baseSize )
  {
    DK_vkFont font = { 0 };

    size_t         size;
    unsigned char *fontBuffer = DK_vkReadFile( filename, &size );
    if ( !fontBuffer )
    {
      fprintf( stderr, "Failed to load font: %s\n", filename );
      return font;
    }

    stbtt_fontinfo fontInfo;
    if ( !stbtt_InitFont( &fontInfo, fontBuffer, 0 ) )
    {
      fprintf( stderr, "Failed to initialize font: %s\n", filename );
      free( fontBuffer );
      return font;
    }

    stbtt_GetFontVMetrics( &fontInfo, &font.ascent, &font.descent, &font.line_gap );
    font.base_size = baseSize;

    font.width  = DK_VULKAN_DEFAULT_FONT_ATLAS_SIZE;
    font.height = DK_VULKAN_DEFAULT_FONT_ATLAS_SIZE;

    font.char_data = (stbtt_packedchar *)calloc( 126, sizeof( stbtt_packedchar ) );
    unsigned char *pixels = (unsigned char *)calloc( font.width * font.height, sizeof( char ) );

    stbtt_pack_context packContext;
    stbtt_PackBegin( &packContext, pixels, font.width, font.height, font.width, 1, NULL );
    stbtt_PackFontRange( &packContext, fontBuffer, 0, baseSize, 0, 125, font.char_data );
    stbtt_PackEnd( &packContext );

    unsigned char *pixelsRGBA = (unsigned char *)malloc( font.width * font.height * 4 );
    for ( int i = 0; i < font.width * font.height; i++ )
    {
      pixelsRGBA[i * 4 + 0] = 255;
      pixelsRGBA[i * 4 + 1] = 255;
      pixelsRGBA[i * 4 + 2] = 255;
      pixelsRGBA[i * 4 + 3] = pixels[i];
    }

    VkDeviceSize imageSize = font.width * font.height * 4;

    VkBuffer       stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    DK_vkCreateBuffer( app,
                       imageSize,
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       &stagingBuffer,
                       &stagingBufferMemory );

    void *data;
    vkMapMemory( app->device, stagingBufferMemory, 0, imageSize, 0, &data );
    memcpy( data, pixelsRGBA, imageSize );
    vkUnmapMemory( app->device, stagingBufferMemory );

    free( pixelsRGBA );
    free( pixels );

    DK_vkCreateImage( app,
                      font.width,
                      font.height,
                      VK_FORMAT_R8G8B8A8_SRGB,
                      VK_IMAGE_TILING_OPTIMAL,
                      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                      &font.image,
                      &font.memory );

    DK_vkTransitionImageLayout( app,
                                font.image,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL );

    DK_vkCopyBufferToImage( app, stagingBuffer, font.image, font.width, font.height );

    DK_vkTransitionImageLayout( app,
                                font.image,
                                VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );

    DK_vkCreateTextureImageView( app, font.image, &font.view );
    DK_vkCreateTextureSampler( app, &font.sampler );

    font.channels = 4;

    // register the font texture int the texture system
    if ( app->textureCount >= app->maxTextures )
    {
      fprintf( stderr, "Maximum texture count reached, cannot add font texture\n" );
      vkDestroyBuffer( app->device, stagingBuffer, NULL );
      vkFreeMemory( app->device, stagingBufferMemory, NULL );
      vkDestroySampler( app->device, font.sampler, NULL );
      vkDestroyImageView( app->device, font.view, NULL );
      vkDestroyImage( app->device, font.image, NULL );
      vkFreeMemory( app->device, font.memory, NULL );
      free( font.char_data );
      free( fontBuffer );
      return font;
    }

    uint32_t fontTextureId = app->textureCount++;
    font.samplerId         = fontTextureId;

    app->textures[fontTextureId].image     = font.image;
    app->textures[fontTextureId].memory    = font.memory;
    app->textures[fontTextureId].view      = font.view;
    app->textures[fontTextureId].sampler   = font.sampler;
    app->textures[fontTextureId].width     = font.width;
    app->textures[fontTextureId].height    = font.height;
    app->textures[fontTextureId].channels  = 4;
    app->textures[fontTextureId].isActive  = true;
    app->textures[fontTextureId].samplerId = fontTextureId;

    DK_vkUpdateDescriptorSetWithTextures( app );

    vkDestroyBuffer( app->device, stagingBuffer, NULL );
    vkFreeMemory( app->device, stagingBufferMemory, NULL );
    free( fontBuffer );

    return font;
  }

  DK_VULKAN_FUNC void DK_vkUnloadFont( DK_vkApplication *app, DK_vkFont *font )
  {
    if ( font->char_data )
    {
      free( font->char_data );
      font->char_data = NULL;
    }

    // Note: (David) DK_vkDestroyTexture is called when cleaning up the application
    font->image   = VK_NULL_HANDLE;
    font->memory  = VK_NULL_HANDLE;
    font->view    = VK_NULL_HANDLE;
    font->sampler = VK_NULL_HANDLE;
  }
  DK_VULKAN_FUNC float DK_vkMeasureTextWidth( DK_vkFont *font, const char *text, float fontSize )
  {
    if ( !text || !font || !font->char_data )
      return 0.0f;

    float scale = fontSize / font->base_size;
    float width = 0.0f;

    for ( int i = 0; i < strlen( text ); i++ )
    {
      if ( text[i] == '\n' )
      {
        continue;
      }

      int char_index = text[i];
      if ( char_index >= 0 && char_index < 126 )
      {
        stbtt_packedchar c = font->char_data[char_index];
        width += c.xadvance * scale;
      }
    }

    return width;
  }

  DK_VULKAN_FUNC float DK_vkMeasureTextHeight( DK_vkFont *font, const char *text, float fontSize )
  {
    if ( !text || !font || !font->char_data )
      return 0.0f;

    float lineHeight = fontSize * DK_VK_DEFAULT_FONT_LINE_SPACEING;
    int   lineCount  = 1;

    for ( const char *ptr = text; *ptr != '\0'; ++ptr )
    {
      if ( *ptr == '\n' )
      {
        ++lineCount;
      }
    }

    return lineCount * lineHeight;
  }

  DK_VULKAN_FUNC void DK_vkDrawText( DK_vkApplication *app,
                                     DK_vkFont        *font,
                                     const char       *text,
                                     DK_vkVec2         position,
                                     float             fontSize,
                                     DK_vkColor        tint )
  {
    if ( !text || !font || !font->char_data )
      return;

    DK_vkTexture *prevTexture   = app->currentTexture;
    int32_t       prevSamplerId = prevTexture ? prevTexture->samplerId : 0;

    DK_vkSetTexture( app, font->samplerId );

    float scale       = fontSize / font->base_size;
    float lineSpacing = fontSize * DK_VK_DEFAULT_FONT_LINE_SPACEING;

    float x = position[0];
    float y = position[1] + ( lineSpacing * 0.5 );

    for ( int i = 0; i < strlen( text ); i++ )
    {
      if ( text[i] == '\n' )
      {
        x = position[0];
        y += lineSpacing;
        continue;
      }

      int char_index = text[i];
      if ( char_index >= 0 && char_index < 126 )
      {
        stbtt_packedchar c = font->char_data[char_index];

        float x0 = c.x0 / (float)font->width;
        float y0 = c.y0 / (float)font->height;
        float x1 = c.x1 / (float)font->width;
        float y1 = c.y1 / (float)font->height;

        DK_vkVec2 uv1 = { x0, y0 };
        DK_vkVec2 uv2 = { x1, y1 };

        float charWidth  = ( c.xoff2 - c.xoff ) * scale;
        float charHeight = ( c.yoff2 - c.yoff ) * scale;

        DK_vkVec2 pos  = { x + c.xoff * scale, y + c.yoff * scale };
        DK_vkSize size = { charWidth, charHeight };

        DK_vkDrawTexturedQuad( app, pos, size, uv1, uv2, tint, font->samplerId );

        x += c.xadvance * scale;
      }
    }

    if ( prevTexture )
    {
      DK_vkSetTexture( app, prevSamplerId );
    }
  }

#endif // DK_VK_IMPLEMENTATION
#ifdef __cplusplus
}
#endif
#endif // __dk_vulkan