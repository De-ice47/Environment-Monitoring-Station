#include <UITree.hpp>
namespace NGUI
{
    void SearchNode(UINode *node, String &output, uint8_t depth);

    uint8_t TreeLevel;
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

        for (uint8_t i = 0; i < RootNode.childCount; i++)
        {
            UINode *child = RootNode.children[i];

            Serial.printf("Node: %p\n", (void *)child);

            if (child == nullptr)
                continue;
            SearchNode(child,result,1);
        }

        Serial.println("Finished Search");

        return result;
    }
    void SearchNode(UINode *node, String &output, uint8_t depth)
    {
        if (node == nullptr)
        {
            return;
        }
        if (depth >= 10)
            return;
        // putting name on the result
        output += "\n";
        for (uint8_t i = 0; i < depth; i++){
            if(i == depth-1) output += "└─ ";
            else  output += "   ";
        }
           
        output += node->name;
        // searching it children
        for (uint8_t i = 0; i < node->childCount; i++)
        {
            Serial.printf(
                "  node[%u] = %p\n",
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
        TreeLevel--;
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
        TreeLevel++;
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
        int8_t indexToGoTo = parent->IndexOf(CurrentNode) + steps;

        indexToGoTo = min(parent->childCount - 1, max(0, (int)indexToGoTo));
        if(indexToGoTo >= parent->childCount){
            indexToGoTo %= parent->childCount;
        }
        else if(indexToGoTo < 0){
            indexToGoTo = parent->childCount + indexToGoTo;
        }
        
        UINode *NodeToSwitchTo = parent->FindChild(indexToGoTo);
        if (NodeToSwitchTo == nullptr)
            return false;
        UINode *target = NodeToSwitchTo;
        CurrentNode = target;
        return true;
    }
}