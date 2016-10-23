#include "HelloVulkanTriangle.h"
#include <iostream>
#include <vector>
#include <cstring>

//vector of cstring names of validation layers
const std::vector<const char*> validationLayers =
{
  "VK_LAYER_LUNARG_standard_validation"
};

void HelloVulkanTriangle::run()
{
  initWindow();
  initVulkan();
  mainLoop();
}

void HelloVulkanTriangle::initWindow()
{
  //Init glfw
  if (glfwInit() != GLFW_TRUE)
  {
    //error initializing
    throw std::runtime_error("failed to initialize glfw");
  }
  //tell glfw to not create an opengl context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  //Set window resizing to false
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  //create the window
  m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Vulkan", nullptr, nullptr);

  if (m_window == nullptr)
  {
    throw std::runtime_error("failed to create glfw window");
  }
}

std::vector<const char*> HelloVulkanTriangle::getRequiredExtensions()
{
  std::vector<const char*> extensions;
  unsigned int glfwExtensionCount{ 0 };
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  for (size_t i = 0; i < glfwExtensionCount; i++)
  {
    extensions.push_back(glfwExtensions[i]);
  }
  if (m_enableValidationLayers)
  {
    extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
  }
  return extensions;
}

bool HelloVulkanTriangle::checkValidationLayerSupport()
{
  uint32_t layerCount{ 0 };
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  
  //get the data of the available layers
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  // check if all of the layers in validationLayers exist in the availableLayers list.
  for (const char* layerName : validationLayers)
  {
    bool layerFound{ false };

    for (const auto& layerProperties : availableLayers)
    {
      //compare validation layer name with available layer name
      if (strcmp(layerName, layerProperties.layerName) == 0)
      {
        //return true if layers are supported
        layerFound = true;
        break;
      }
    }
    if (!layerFound)
    {
      return false;
    }
  }
  return true;
}

void HelloVulkanTriangle::createInstance()
{
  if (m_enableValidationLayers && !checkValidationLayerSupport())
  {
    throw std::runtime_error("Validation layers requested, but not available");
  }
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);;
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t extensionCount{ 0 };
  //get the number of extensions
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> extensions(extensionCount);
  //query the extension details
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
  //Each VkExtensionProperties struct contains the name and version of an extension
  std::cout << "Available Extensions: " << std::endl;
  for (const auto& extension : extensions)
  {
    std::cout << "\t" << extension.extensionName << std::endl;
  }

  auto glfwExtensions = getRequiredExtensions();
  createInfo.enabledExtensionCount = glfwExtensions.size();
  createInfo.ppEnabledExtensionNames = glfwExtensions.data();

  if (m_enableValidationLayers)
  {
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
  }
  else
  {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateInstance(&createInfo, nullptr, m_instance.replace()) != VK_SUCCESS)
  {
    throw std::runtime_error("failed to create instance");
  }
}

void HelloVulkanTriangle::initVulkan()
{
  createInstance();
}

void HelloVulkanTriangle::mainLoop()
{
  while (!glfwWindowShouldClose(m_window))
  {
    glfwPollEvents();
  }
}
