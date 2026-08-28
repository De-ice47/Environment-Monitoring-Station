#include <UITree.hpp>
namespace NGUI
{
    void SearchNode(UINode *node, String &output, uint8_t depth);

    UINode *CurrentNode;
    UINode RootNode;
    void Initialize()
    {
        RootNode = UINode("ROOT");
        CurrentNode = &RootNode;
    }
    String DebugStructure()
    {
        String result = "ROOT";

        Serial.println("Starting Search");

        Serial.printf("RootNode address: %p\n", (void *)&RootNode);
        Serial.printf("childCount: %u\n", RootNode.childCount);
        Serial.printf("children address: %p\n", (void *)RootNode.children);

        Serial.println("Before loop");

        for (uint8_t i = 0; i < RootNode.childCount; i++)
        {
            Serial.printf("Loop i = %u\n", i);

            UINode *child = RootNode.children[i];

            Serial.printf("Child pointer: %p\n", (void *)child);

            if (child == nullptr)
                continue;

            Serial.println("Child exists");
        }

        Serial.println("Finished Search");

        return result;
    }
    void SearchNode(UINode *node, String &output, uint8_t depth)
    {
        Serial.printf(
            "SearchNode: node=%p depth=%u\n",
            (void *)node,
            depth);

        if (node == nullptr)
        {
            Serial.println("NULL NODE");
            return;
        }

        if (depth >= 10)
            return;

        Serial.printf(
            "  name ptr=%p\n",
            (void *)node->name);

        output += "\n";

        for (uint8_t i = 0; i < depth; i++)
            output += "\t";

        Serial.println("  adding name...");

        output += node->name;

        Serial.println("  name added");

        Serial.printf(
            "  childCount=%u\n",
            node->childCount);

        for (uint8_t i = 0; i < node->childCount; i++)
        {
            Serial.printf(
                "  child[%u] = %p\n",
                i,
                (void *)node->children[i]);

            SearchNode(node->children[i], output, depth + 1);
        }
    }

    bool CallAction()
    {
        if (CurrentNode == nullptr)
            return false;
        if (CurrentNode->ActionOnInteract == nullptr)
            return false;
        CurrentNode->ActionOnInteract();
        return true;
    }
    bool Rise()
    {
        if (CurrentNode == nullptr)
            return false;
        if (CurrentNode->parent == nullptr)
            return false;
        UINode *targetRise = CurrentNode->parent;
        CurrentNode = targetRise;
        return true;
    }
    bool Fall(uint8_t childNode)
    {
        if (CurrentNode == nullptr)
            return false;
        if (CurrentNode->children == nullptr)
            return false;
        UINode *NodeToFallTo = CurrentNode->FindChild(childNode);
        if (NodeToFallTo == nullptr)
            return false;
        CurrentNode = NodeToFallTo;
        return true;
    }
    bool Step(uint8_t steps)
    {
        if (CurrentNode == nullptr)
            return false;
        if (CurrentNode->parent == nullptr)
            return false;
        if (CurrentNode->parent->children == nullptr)
            return false;
        UINode *parent = CurrentNode->parent;
        uint8_t indexToGoTo = parent->IndexOf(CurrentNode) + steps;
        UINode *NodeToSwitchTo = parent->FindChild(indexToGoTo);
        if (NodeToSwitchTo == nullptr)
            return false;
        UINode *target = NodeToSwitchTo;
        CurrentNode = target;
        return true;
    }
}