#include <UINode.hpp>
#include <Arduino.h>

namespace NGUI
{
    const char *name = nullptr;
    UINode *parent = nullptr;
    UINode **children = nullptr;

    uint8_t childCount = 0;
    uint8_t childCapacity = 4;

    std::function<void()> ActionOnInteract;

    UINode::UINode(const char *name, UINode *parent)
    {
        this->name = name;
        this->parent = parent;
        this->children = nullptr;
        childCount = 0;
        childCapacity = 8;
        ActionOnInteract = nullptr;
    }
    UINode::UINode(const char *name)
        : UINode(name, nullptr)
    {
    }

    UINode::UINode()
        : UINode("NULL", nullptr)
    {
    }

    bool UINode::AddChild(UINode *child)
    {
        if (childCount >= childCapacity)
        {
            uint8_t newCapacity = childCapacity == 0
                                      ? 4
                                      : childCapacity * 2;

            UINode**newChildren = new UINode *[newCapacity];

            for (uint8_t i = 0; i < childCount; i++)
                newChildren[i] = children[i];

            delete[] children;

            children = newChildren;
            childCapacity = newCapacity;
        }
        child->parent = this;
        children[childCount] = child;
        childCount++;

        child->parent = this;
        return true;
    }
    UINode *UINode::FindChild(const char *name)
    {
        for (uint8_t i = 0; i < childCount; i++)
        {
            UINode *currentNode = children[i];
            if (currentNode != nullptr)
            {
                if (currentNode->name == name)
                    return currentNode;
            }
        }
        return nullptr;
    }
    UINode *UINode::FindChild(uint8_t index)
    {
        if (index < childCount)
            return children[index];
        else
            return nullptr;
    }
    uint8_t UINode::IndexOf(UINode *child)
    {
        for (uint8_t i = 0; i < childCount; i++)
        {
            UINode *currentNode = children[i];
            if (currentNode != nullptr)
            {
                if (currentNode->name == name)
                    return i;
            }
        }
        return 0xff;
    }
}