#include "vulkan/global_structs.hpp"

namespace vulkan {
    bool QueueFamilies::isComplete(void) const {
        return (
            this->graphicsFamily.has_value() &&
            this->presentFamily.has_value()
        );
    }
}