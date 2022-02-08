#include "vulkan_check.hpp"

#if CALCIUM_BUILD_DEBUG

#include <cstdio>

namespace cl::Vulkan {

#pragma warning(push)
#pragma warning(disable : 26812)

void PrintVulkanError(VkResult result) {
  switch (result) {
    case (VK_SUCCESS):                                            fprintf(stderr, "[Vulkan] VK_SUCCESS\n");                                            break;
    case (VK_NOT_READY):                                          fprintf(stderr, "[Vulkan] VK_NOT_READY\n");                                          break;
    case (VK_TIMEOUT):                                            fprintf(stderr, "[Vulkan] VK_TIMEOUT\n");                                            break;
    case (VK_EVENT_SET):                                          fprintf(stderr, "[Vulkan] VK_EVENT_SET\n");                                          break;
    case (VK_EVENT_RESET):                                        fprintf(stderr, "[Vulkan] VK_EVENT_RESET\n");                                        break;
    case (VK_INCOMPLETE):                                         fprintf(stderr, "[Vulkan] VK_INCOMPLETE\n");                                         break;
    case (VK_ERROR_OUT_OF_HOST_MEMORY):                           fprintf(stderr, "[Vulkan] VK_ERROR_OUT_OF_HOST_MEMORY\n");                           break;
    case (VK_ERROR_OUT_OF_DEVICE_MEMORY):                         fprintf(stderr, "[Vulkan] VK_ERROR_OUT_OF_DEVICE_MEMORY\n");                         break;
    case (VK_ERROR_INITIALIZATION_FAILED):                        fprintf(stderr, "[Vulkan] VK_ERROR_INITIALIZATION_FAILED\n");                        break;
    case (VK_ERROR_DEVICE_LOST):                                  fprintf(stderr, "[Vulkan] VK_ERROR_DEVICE_LOST\n");                                  break;
    case (VK_ERROR_MEMORY_MAP_FAILED):                            fprintf(stderr, "[Vulkan] VK_ERROR_MEMORY_MAP_FAILED\n");                            break;
    case (VK_ERROR_LAYER_NOT_PRESENT):                            fprintf(stderr, "[Vulkan] VK_ERROR_LAYER_NOT_PRESENT\n");                            break;
    case (VK_ERROR_EXTENSION_NOT_PRESENT):                        fprintf(stderr, "[Vulkan] VK_ERROR_EXTENSION_NOT_PRESENT\n");                        break;
    case (VK_ERROR_FEATURE_NOT_PRESENT):                          fprintf(stderr, "[Vulkan] VK_ERROR_FEATURE_NOT_PRESENT\n");                          break;
    case (VK_ERROR_INCOMPATIBLE_DRIVER):                          fprintf(stderr, "[Vulkan] VK_ERROR_INCOMPATIBLE_DRIVER\n");                          break;
    case (VK_ERROR_TOO_MANY_OBJECTS):                             fprintf(stderr, "[Vulkan] VK_ERROR_TOO_MANY_OBJECTS\n");                             break;
    case (VK_ERROR_FORMAT_NOT_SUPPORTED):                         fprintf(stderr, "[Vulkan] VK_ERROR_FORMAT_NOT_SUPPORTED\n");                         break;
    case (VK_ERROR_FRAGMENTED_POOL):                              fprintf(stderr, "[Vulkan] VK_ERROR_FRAGMENTED_POOL\n");                              break;
    case (VK_ERROR_OUT_OF_POOL_MEMORY):                           fprintf(stderr, "[Vulkan] VK_ERROR_OUT_OF_POOL_MEMORY\n");                           break;
    case (VK_ERROR_INVALID_EXTERNAL_HANDLE):                      fprintf(stderr, "[Vulkan] VK_ERROR_INVALID_EXTERNAL_HANDLE\n");                      break;
    case (VK_ERROR_FRAGMENTATION):                                fprintf(stderr, "[Vulkan] VK_ERROR_FRAGMENTATION\n");                                break;
    case (VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS):               fprintf(stderr, "[Vulkan] VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS\n");               break;
    case (VK_ERROR_SURFACE_LOST_KHR):                             fprintf(stderr, "[Vulkan] VK_ERROR_SURFACE_LOST_KHR\n");                             break;
    case (VK_ERROR_NATIVE_WINDOW_IN_USE_KHR):                     fprintf(stderr, "[Vulkan] VK_ERROR_NATIVE_WINDOW_IN_USE_KHR\n");                     break;
    case (VK_SUBOPTIMAL_KHR):                                     fprintf(stderr, "[Vulkan] VK_SUBOPTIMAL_KHR\n");                                     break;
    case (VK_ERROR_OUT_OF_DATE_KHR):                              fprintf(stderr, "[Vulkan] VK_ERROR_OUT_OF_DATE_KHR\n");                              break;
    case (VK_ERROR_INCOMPATIBLE_DISPLAY_KHR):                     fprintf(stderr, "[Vulkan] VK_ERROR_INCOMPATIBLE_DISPLAY_KHR\n");                     break;
    case (VK_ERROR_VALIDATION_FAILED_EXT):                        fprintf(stderr, "[Vulkan] VK_ERROR_VALIDATION_FAILED_EXT\n");                        break;
    case (VK_ERROR_INVALID_SHADER_NV):                            fprintf(stderr, "[Vulkan] VK_ERROR_INVALID_SHADER_NV\n");                            break;
    case (VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT): fprintf(stderr, "[Vulkan] VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT\n"); break;
    case (VK_ERROR_NOT_PERMITTED_EXT):                            fprintf(stderr, "[Vulkan] VK_ERROR_NOT_PERMITTED_EXT\n");                            break;
    case (VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT):          fprintf(stderr, "[Vulkan] VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT\n");          break;
    case (VK_THREAD_IDLE_KHR):                                    fprintf(stderr, "[Vulkan] VK_THREAD_IDLE_KHR\n");                                    break;
    case (VK_THREAD_DONE_KHR):                                    fprintf(stderr, "[Vulkan] VK_THREAD_DONE_KHR\n");                                    break;
    case (VK_OPERATION_DEFERRED_KHR):                             fprintf(stderr, "[Vulkan] VK_OPERATION_DEFERRED_KHR\n");                             break;
    case (VK_OPERATION_NOT_DEFERRED_KHR):                         fprintf(stderr, "[Vulkan] VK_OPERATION_NOT_DEFERRED_KHR\n");                         break;
    case (VK_PIPELINE_COMPILE_REQUIRED_EXT):                      fprintf(stderr, "[Vulkan] VK_PIPELINE_COMPILE_REQUIRED_EXT\n");                      break;
    default:                                                      fprintf(stderr, "[Vulkan] VK_ERROR_UNKNOWN\n");                                      break;
  }
}

#pragma warning(pop)

}

#endif
