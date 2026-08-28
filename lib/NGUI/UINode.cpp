#include <UINode.hpp>
#include <Arduino.h>

namespace NGUI
{
    UINode::UINode(const char* name, UINode* parent)
        : name(name),
          parent(parent),
          children(nullptr),
          childCount(0),
          childCapacity(0),
          ActionOnInteract(nullptr)
    {
    }

    UINode::UINode(const char* name)
        : UINode(name, nullptr)
    {
    }

    UINode::UINode()
        : UINode("NULL", nullptr)
    {
    }

    bool UINode::AddChild(UINode* child)
    {
        if(child == nullptr)
            return false;

        if(childCount >= childCapacity)
        {
            uint8_t newCapacity =
                childCapacity == 0
                    ? 4
                    : childCapacity * 2;

            UINode** newChildren =
                new UINode*[newCapacity];

            for(uint8_t i = 0; i < childCount; i++)
            {
                newChildren[i] = children[i];
            }

            delete[] children;

            children = newChildren;
            childCapacity = newCapacity;
        }

        children[childCount] = child;
        child->parent = this;
        childCount++;

        return true;
    }

    UINode* UINode::FindChild(const char* name)
    {
        for(uint8_t i = 0; i < childCount; i++)
        {
            UINode* currentNode = children[i];

            if(currentNode != nullptr)
            {
                if(strcmp(currentNode->name, name) == 0)
                    return currentNode;
            }
        }

        return nullptr;
    }

    UINode* UINode::FindChild(uint8_t index)
    {
        if(index < childCount)
            return children[index];

        return nullptr;
    }

    uint8_t UINode::IndexOf(UINode* child)
    {
        for(uint8_t i = 0; i < childCount; i++)
        {
            if(children[i] == child)
                return i;
        }

        return 0xFF;
    }
}