#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <Windows.h>
#include <cmath>
#include <string.h>


#include "Tree.h"
#include "..\Logs\Logs.h"


static bool TreeNodeFindVariables(const TreeNode* node, Tree* tree);

static double CalculateNode(const TreeNode* node, const Tree* tree, bool* canCalculate);

static bool TreeAddVariable(Tree* tree, const char* variable);

static void OptimizeNodeToNumber(TreeNode* node, const double number);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                           Конструкторы и деструкторы
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeConstructor(Tree* tree)
{
    LOG_TREE_DBG("Вызван TreeConstructor()");

    assert(tree);

    tree->root                = nullptr;
    
    tree->nodesArrayPtr       = nullptr;
    tree->nodesArraySize      = 0;

    return true;
}

TreeNode* TreeNodeConstructor(const TreeValue* value)
{
    LOG_TREE_DBG("Вызван TreeNodeConstructor()");

    //assert(expression);

    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));

    if (!node)
    {
        LOG_TREE_ERR("NodeConstructor: не хватает памяти для создания нового узла");
        return nullptr;
    }

    node->NodeLeft  = nullptr;
    node->NodeRight = nullptr;
    node->Allocated = true;

    if (value)
        memcpy(&node->Value, value, sizeof(TreeValue));

    return node;
}

bool TreeDestructor(Tree* problem) 
{
    LOG_TREE_DBG("Вызван TreeDestructor()");

    // assert(tree);
    
    if (problem)
    {
        for (size_t st = 0; st < problem->nodesArraySize; st++)
        {
            ExpressionDestructor(&problem->nodesArrayPtr[st].Value);
        }
        free(problem->nodesArrayPtr);
    }

    return true;
}

bool TreeNodeDestructor(TreeNode* node)
{
    LOG_TREE_DBG("Вызван TreeNodeDestructor()");

    // assert(node);

    if (node && node->Allocated)
    {
        if (node->NodeLeft)
            TreeNodeDestructor(node->NodeLeft);
        if (node->NodeRight)
            TreeNodeDestructor(node->NodeRight);
        free(node);
    }

    return true;
}


///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                     Работа с деревом (стандартные функции)
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void TreeAddLeftNode(TreeNode* parent, TreeNode* child)
{
    assert(parent);
    assert(child);

    parent->NodeLeft = child;
    child->Parent = parent;
}

void TreeAddRightNode(TreeNode* parent, TreeNode* child)
{
    assert(parent);
    assert(child);

    parent->NodeRight = child;
    child->Parent = parent;
}

size_t TreeMeasure(TreeNode* node)
{
    LOG_TREE_DBG("TreeMeasure");

    assert(node);

    size_t result = 1;
    if (node->NodeLeft)
        result += TreeMeasure(node->NodeLeft);
    if (node->NodeRight)
        result += TreeMeasure(node->NodeRight);
    node->ChildCount = result;

    return result;
}

/**
 * @brief               Рекурсивно копирует дерево.
 * @param nodeSrc       Исходное дерево.
 * @param nodeParentDst Указатель, куда надо скопировать дерево.
 * @return              false в случае ошибки, true в случае успеха.
*/
TreeNode* TreeCopyRecursive(const TreeNode* nodeSrc)
{
    LOG_TREE_DBG("TreeCopyRecursive");

    assert(nodeSrc);

    TreeNode* node = TreeNodeConstructor(&nodeSrc->Value);

    if (!(node))
    {
        TreeNodeDestructor(node);
        return nullptr;
    }
    
    if (nodeSrc->NodeLeft)
    {
        node->NodeLeft = TreeCopyRecursive(nodeSrc->NodeLeft);
        
        if (!(node->NodeLeft))
        {
            TreeNodeDestructor(node);
            return nullptr;
        }
            
        TreeAddLeftNode(node, node->NodeLeft);
    }
    
    if (nodeSrc->NodeRight)
    {
        node->NodeRight = TreeCopyRecursive(nodeSrc->NodeRight);
        
        if (!(node->NodeRight))
        {
            TreeNodeDestructor(node);
            return nullptr;
        }
            
        TreeAddRightNode(node, node->NodeRight);
    }
    
    return node;
}

bool IsLeaf(const TreeNode* node)
{
    assert(node);

    if (node && (node->NodeLeft == nullptr || node->NodeRight == nullptr))
        return true;
    else
        return false;
}

/**
 * @brief       Сравнивает два поддерева.
 * @param node1 Первое поддерево.
 * @param node2 Второе поддерево.
 * @return      true, если поддеревья равны.
*/
bool CompareTrees(const TreeNode* node1, const TreeNode* node2)
{
    LOG_TREE_DBG("CompareTrees");

    assert(node1);
    assert(node2);

    if (ExpressionsEqual((Expression*)&node1->Value, (Expression*)&node2->Value))
    {
        bool result = true;
        if (node1->NodeLeft && node2->NodeLeft)
            result = CompareTrees(node1->NodeLeft, node2->NodeLeft);
        else
            result = false;

        if (node1->NodeRight && node2->NodeRight)
            result &= CompareTrees(node1->NodeRight, node2->NodeRight);
        else
            result = false;

        return result;
    }
    return false;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                              Поиск элементов дерева
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

TreeNode* TreeFindObject(TreeNode* node, const TreeValue object)
{
    LOG_TREE_DBG("TreeFindObject");

    assert(node);
    assert(node);
    
    if (ExpressionsEqual((Expression*)&object, (Expression*)&node->Value) == 0)
        return node;
    
    TreeNode* result = nullptr;

    if (node->NodeLeft)
        result = TreeFindObject(node->NodeLeft, object);
    
    if (node->NodeRight && !result)
        result = TreeFindObject(node->NodeRight, object);
    
    return result;
}

TreeNode* TreeFindObjectStack(TreeNode* node, const TreeValue object, Stack* stk)
{
    LOG_TREE_DBG("TreeFindeObjectStack");

    assert(stk);
    assert(node);

    StackPush(stk, &node);

    if (ExpressionsEqual((Expression*)&object, (Expression*)&node->Value) == 0)
        return node;

    TreeNode* result = nullptr;

    if (node->NodeLeft)
        result = TreeFindObjectStack(node->NodeLeft, object, stk);
        
    if (node->NodeRight && !result)
        result = TreeFindObjectStack(node->NodeRight, object, stk);

    if (!result)
        StackPop(stk, nullptr);

    return result;
}

TreeNode* GetNodeFromStack(const Stack* stk, const size_t index)
{
    LOG_TREE_DBG("GetNodeFromStack");

    assert(stk);

    TreeNode** nodeptr = (TreeNode**)StackGetElemAt(stk, index);

    if (nodeptr)
        return *nodeptr;

    return nullptr;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\