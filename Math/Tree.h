#ifndef MY_TREE_H
#define MY_TREE_H

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#include "..\Config.h"
#include "..\LanguageGrammar.h"
#include "..\StackLibrary\StackLite.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

const double NodesArrayCapacityScale = 1.5;
const size_t MinNodesArraySize       = 100;

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
};

struct Tree
{
    /// Корень дерева.
    TreeNode* Root;

    /// Указатель на начало массива узлов
    TreeNode* NodesArrayPtr;
    /// Размер массива
    size_t    NodesArrayCurSize;
    /// Максимальное количество элементов в массиве до увелечения памяти.
    size_t    NodesArrayCapacity;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeConstructor(Tree* programm);

TreeNode* TreeNodeConstructor(Tree* tree, const TreeValue* value);

bool TreeDestructor(Tree* programm);

bool TreeNodeDestructor(TreeNode* node);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void TreeAddRightNode(TreeNode* Parent, TreeNode* child);

void TreeAddLeftNode(TreeNode* Parent, TreeNode* child);

size_t TreeMeasure(TreeNode* node);

TreeNode* TreeCopyRecursive(Tree* tree, const TreeNode* nodeSrc);

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
