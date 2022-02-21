#include "vulkan_pipeline_layout.hpp"

#include "vulkan_check.hpp"

namespace cl::Vulkan {

VkPipelineLayout CreatePipelineLayout(VulkanContextData* context, VkDescriptorSetLayout descriptor_set_layout) {
  // Here we let the graphics pipeline know ahead of time about any uniforms we intend to implement
  // We also use this to set up push constants
  VkPipelineLayoutCreateInfo pipeline_layout_info { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
  pipeline_layout_info.setLayoutCount = 1;
  pipeline_layout_info.pSetLayouts = &descriptor_set_layout;
  pipeline_layout_info.pushConstantRangeCount = 0;    // optional
  pipeline_layout_info.pPushConstantRanges = nullptr; // optional
  
  VkPipelineLayout pipeline_layout;
  VK_CHECK(vkCreatePipelineLayout(context->device, &pipeline_layout_info, context->allocator, &pipeline_layout));
  return pipeline_layout;
}

}
