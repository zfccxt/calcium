#pragma once

#include <vector>

#include "buffer_element.hpp"

namespace cl {

class BufferLayout {
public:
  BufferLayout() = default;
  BufferLayout(const std::vector<BufferElement>& buffer_elements);
  BufferLayout(const std::initializer_list<BufferElement>& buffer_elements);

  inline size_t GetStride() const                                 { return stride_;                  }
  inline size_t GetSize() const                                   { return size_;                    }
  inline size_t GetNumElements() const                            { return buffer_elements_.size();  }
  inline const std::vector<BufferElement>& GetElements() const    { return buffer_elements_;         }

  inline std::vector<BufferElement>::iterator begin()             { return buffer_elements_.begin(); }
  inline std::vector<BufferElement>::iterator end()               { return buffer_elements_.end();   }
  inline std::vector<BufferElement>::const_iterator begin() const { return buffer_elements_.begin(); }
  inline std::vector<BufferElement>::const_iterator end() const   { return buffer_elements_.end();   }
  inline BufferElement operator [](int i) const                   { return buffer_elements_[i];      }

private:
  void CalculateOffsetsAndSize();

private:
  std::vector<BufferElement> buffer_elements_;
  size_t size_   = 0; 
  size_t stride_ = 0;
};

}
