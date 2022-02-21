#pragma once

#include <vulkan/vulkan.h>

#include "buffer_layout.hpp"

namespace cl::Vulkan {

VkVertexInputBindingDescription CreateInputBindingDescription(const BufferLayout& layout);
std::vector<VkVertexInputAttributeDescription>CreateInputAttribDescriptions(const BufferLayout& layout);

}
