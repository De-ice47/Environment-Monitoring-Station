#pragma once

#ifndef NGUI_UINODE_HPP
#define NGUI_UINODE_HPP

#include <stdint.h>
#include <functional>

namespace NGUI
{
    class UINode
    {
    public:
    UINode(const char *name, UINode *parent);
        UINode(const char *name);
        UINode();
        
        const char *name;

        UINode *parent;

        UINode **children;
        uint8_t childCount;

        std::function<void()> ActionOnInteract;

        bool AddChild(UINode *child);
        UINode* FindChild(const char *name);
        UINode* FindChild(uint8_t index);
        uint8_t IndexOf(UINode *child);
    };
}

#endif // NGUI_UINODE_HPP