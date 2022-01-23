#ifndef MY_TREE_H
#define MY_TREE_H


#include "..\..\StackLibrary\StackLite.h"
#include "..\MathExpression\MathExpression.h"


/// �������� ���������� ����� �� ���� ������.
#define GRAPHVIZ

/// ������ ������������ �� ����� ��� ���������� (x, y, z).
const size_t MATH_AVERAGE_VAR_COUNT = 3;

/// ���� � ������ ����� ���������� ���������� ������, ��� MATH_AVERAGE_VAR_COUNT,
/// �� ������ �������� ����� �������� � MATH_VAR_COUNT_SCALE ���
const size_t MATH_VAR_COUNT_SCALE   = 2;

struct MathNode
{
    /// �������� ����.
    MathExpression expression;
    /// �������� ����.
    MathNode* parent;
    /// ����� �������.
    MathNode* nodeLeft;
    /// ������ �������.
    MathNode* nodeRight;
    /// ���������� �������� � ���������
    size_t    childCount;
};

struct MathTree
{
    /// ������ ������.
    MathNode* root;
    /// ���������� ���������� ����� ������ � �������, ����� �������� ����� ������� TreeMeasure().
    size_t treeLength;

    /// ����������, ������������ � ������.
    const char**   variables;
    /// �������� ����������, ������������ � ������.
    double* values;
    /// ���������� ����������, ������������ � ������.
    size_t  varCapacity;

    /// ���� true, �� ��� ������� ���� ������ ���� �������� ��������. ���� false, �� root - ��� ������ �����.
    bool    allocatedSeparately;
    /// ��������� �� ������ ������� �����, ���� allocatedSeparately == false.
    MathNode* nodesArrayPtr;
    /// ������ �������
    size_t nodesArraySize;
};

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeConstructor(MathTree* problem);

MathNode* TreeNodeConstructor(const MathExpression* expression);

bool TreeDestructor(MathTree* problem);

bool TreeNodeDestructor(MathNode* node);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

void TreeAddRightNode(MathNode* parent, MathNode* child);

void TreeAddLeftNode(MathNode* parent, MathNode* child);

size_t TreeMeasure(MathNode* node);

MathNode* TreeCopyRecursive(const MathNode* nodeSrc);

bool IsLeaf(const MathNode* node);

bool CompareTrees(const MathNode* node1, const MathNode* node2);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

MathNode* TreeFindObject(MathNode* node, const MathExpression object);

MathNode* TreeFindObjectStack(MathNode* node, const MathExpression object, Stack* stk);

MathNode* GetNodeFromStack(const Stack* stk, const size_t index);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

bool TreeFindVariables(MathTree* problem);

bool TreeFindVariable(const MathTree* problem, const char* variable);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

double TreeCalculate(const MathTree* problem);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief      ������������ ������, �������� ��� �����������
 *             (�� � MathTree.h ������� MathTreeOptimize<X>, ��� <X> - ��� �����������)
 * @param tree ��������� �� ������
*/
void MathTreeOptimize(MathTree* problem);

/**
 * @brief      ������������ ����, ������� ����� ���������. ��������: (4+8)*9 -> 108; sin(Pi/6) -> 0.5;
 *             ������������������ ������� ����������� � ��������!
 * @param node ����, ������� ����� ��������������.
 * @return     true, ���� ���� �����������. false, ���� ������ �������������� ������.
*/
bool MathTreeOptimizeCalculatedNodes(MathNode* node);

/**
 * @brief      ������������ ����, ������� ����� ���������. ��������: x*1 -> x; x^0 -> 1;
 *             ��� ���������� ������ ������ ����������� ������������� ��������� 
 *             while (OptimizeCalculatedNodes(node) == true);
 * @param node ����, ������� ����� ��������������.
 * @return     true, ���� ���� �����������. false, ���� ������ �������������� ������.
*/
bool OptimizeWorthlessNodes(MathNode* node);

/**
 * @brief      ������������ ����������, ������� ����� ���������. �������� (��������� �� A - ���������): A-A -> 0; A/A -> 1;
 *             ��� ���������� ������ ������ ����������� ������������� ���������
 *             while (OptimizeCalculatedNodes(node) == true); � 
 *             while (OptimizeWorthlessNodes(node) == true);
 * @param node ����, ������� ����� ��������������.
 * @return     true, ���� ���� �����������. false, ���� ������ �������������� ������.
*/
bool OptimizeWorthlessTrees(MathNode* node);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#ifdef GRAPHVIZ

bool CreateTreeGraph(const char* outImagefileName, MathNode* node, bool openFile = false);

void CreateNodeGraph(FILE* file, MathNode* node, size_t parentId, bool IsRight);

#else

#define CreateTreeGraph(fileName, tree) false;
#define CreateNodeGraph(file, node, parentId, IsRight) false;

#endif

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\


#endif
