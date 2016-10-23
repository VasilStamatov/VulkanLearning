#pragma once
#include <vulkan/vulkan.h>
#include <functional>

template<typename T>
class VDeleter
{
public:
  VDeleter() : VDeleter([](T, VkAllocationCallbacks*) {})
  {
    //empty
  }
  //Pass only the object in the deleter function
  VDeleter(std::function<void(T, VkAllocationCallbacks*)> _deleteFunc)
  {
    deleter = [=](T _obj)
    {
      _deleteFunc(_obj, nullptr);
    };
  }

  //Pass the vkInstance as a reference for the deleted object along with the object itself
  VDeleter(const VDeleter<VkInstance>& _instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> _deleteFunc)
  {
    deleter = [&_instance, _deleteFunc](T _obj) 
    {
      _deleteFunc(_instance, _obj, nullptr); 
    };
  }

  //Pass the vkDevice as a reference for the deleted object
  VDeleter(const VDeleter<VkDevice>& _device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> _deleteFunc)
  {
    deleter = [&_device, _deleteFunc](T _obj) 
    {
      _deleteFunc(_device, _obj, nullptr); 
    };
  }
  ~VDeleter()
  {
    cleanup();
  }

  //the address of operator returns a constant pointer to make sure the object stays unchanged
  const T* operator &() const { return &object; }

  //replace is used when you want a pointer to the object with the intent of changing it
  T* replace()
  {
    cleanup();
    return &object;
  }

  //returns the object itself
  operator T() const { return object; }

  void operator=(T _rhs)
  {
    if (_rhs != object)
    {
      cleanup();
      object = _rhs;
    }
  }

  template<typename T2>
  bool operator==(T2 _rhs)
  {
    return object == T(_rhs);
  }
private:
  T object{ VK_NULL_HANDLE };
  std::function<void(T)> deleter;

  void cleanup()
  {
    if (object != VK_NULL_HANDLE)
    {
      deleter(object);
    }
    object = VK_NULL_HANDLE;
  }
};