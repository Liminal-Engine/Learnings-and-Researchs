#include "vulkanhdl/include/_queues/_queues.hpp"

#include <vector>
#include <stdexcept>

namespace vulkanhdl {
    namespace _queues {

        bool _QueueFamilies::_isComplete(void) const {
            return this->_graphicsFamily.has_value() && this->_presentFamily.has_value();
        }

        std::set<_QueueFamilyIndex_t> _QueueFamilies::_toSet(void) const {
            return std::set<_QueueFamilyIndex_t> {this->_graphicsFamily.value(), this->_presentFamily.value()};
        }

        std::vector <_QueueFamilyIndex_t> _QueueFamilies::_toVector(void) const {
            return std::vector<_QueueFamilyIndex_t> {this->_graphicsFamily.value(), this->_presentFamily.value()};
        }

    } // _queues
} // vulkanhdl
