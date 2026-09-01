#include <stdint.h>
#include <functional>
#include <UINode.hpp>
#include <stdlib.h>
#include <Arduino.h>

namespace NGUI
{
    extern uint8_t TreeLevel;
    extern UINode *CurrentNode;
    extern UINode RootNode;
    // Tree structure functions
    void Initialize();
    String DebugStructure();
    // Navigation Functions
    /// @brief Will call the action function of the current Node
    bool CallAction();
    /// @brief Will change the current node to the current node's parent
    bool Rise();
    /// @brief Will change the current node to one of the current node's children
    /// @param childNode
    bool Fall(uint8_t childNode);
    /// @brief Will change between the other children under the current node's parent
    bool Step(uint8_t steps);
}
