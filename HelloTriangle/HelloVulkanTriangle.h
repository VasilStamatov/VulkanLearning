#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VDeleter.h"
#include <vector>
class HelloVulkanTriangle
{
  using uint = unsigned int;
public:
  void run();

private:
  void initWindow();
  std::vector<const char*> getRequiredExtensions();
  bool checkValidationLayerSupport();
  void createInstance();
  void initVulkan();
  void mainLoop();

  GLFWwindow* m_window{ nullptr };
  uint m_windowWidth{ 800 };
  uint m_windowHeight{ 600 };

#ifdef NDEBUG
  const bool enableValidationLayers{ false };
#else
  const bool m_enableValidationLayers{ true };
#endif // NDEBUG

  VDeleter<VkInstance> m_instance{ vkDestroyInstance };
};

