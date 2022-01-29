#ifndef MY_TREE_H
#define MY_TREE_H

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#include "..\Config.h"
#include "..\LanguageGrammar.h"
#include "..\StackLibrary\StackLite.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

typedef Expression TreeValue;

struct TreeNode
{
    /// Значение узла.
    TreeValue Value;
    /// Родитель узла.
    TreeNode* Parent;
    /// Левый потомок.
    TreeNode* NodeLeft;
    /// Правый потомок.
    TreeNode* NodeRight;
    /// Количество потомков у поддерева.
    size_t    ChildCount;
    /// Если true, то память для узла была выделена отдельно и нужно вызвать TreeNodeDestructor.
    bool      Allocated;
};

struct Tree
{
    /// Корень дерева.
    TreeNode* root;

    /// Указатель на начало массива узлов, если allocatedSeparately == false.
    TreeNode* nodesArrayPtr;
    /// Размер массива
    size_t nodesArraySize;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeConstructor(Tree* programm);

TreeNode* TreeNodeConstructor(const TreeValue* expression);

bool TreeDestructor(Tree* programm);

bool TreeNodeDestructor(TreeNode* node);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void TreeAddRightNode(TreeNode* Parent, TreeNode* child);

void TreeAddLeftNode(TreeNode* Parent, TreeNode* child);

size_t TreeMeasure(TreeNode* node);

TreeNode* TreeCopyRecursive(const TreeNode* nodeSrc);

bool IsLeaf(const TreeNode* node);

bool CompareTrees(const TreeNode* node1, const TreeNode* node2);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

TreeNode* TreeFindObject(TreeNode* node, const TreeValue object);

TreeNode* TreeFindObjectStack(TreeNode* node, const TreeValue object, Stack* stk);

TreeNode* GetNodeFromStack(const Stack* stk, const size_t index);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\


#endif
