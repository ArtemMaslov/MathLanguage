#ifndef LANGUAGE_TREE_H_
#define LANGUAGE_TREE_H_

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#include "Tree.h"

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

double TreeCalculate(const Tree* programm);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

/**
 * @brief      Оптимизирует дерево, выполняя все оптимизации
 *             (см в Tree.h функции MathTreeOptimize<X>, где <X> - вид оптимизации)
 * @param tree Указатель на дерево
*/
void MathTreeOptimize(Tree* programm);

/**
 * @brief      Оптимизирует узлы, которые можно вычислить. Например: (4+8)*9 -> 108; sin(Pi/6) -> 0.5;
 *             Тригонометрические функции вычисляются в радианах!
 * @param node Узел, который нужно оптимизировать.
 * @return     true, если была оптимизация. false, если дерево оптимизировать нельзя.
*/
bool MathTreeOptimizeCalculatedNodes(TreeNode* node);

/**
 * @brief      Оптимизирует узлы, которые можно сократить. Например: x*1 -> x; x^0 -> 1;
 *             Для корректной работы данной оптимизации первоначально выполнить 
 *             while (OptimizeCalculatedNodes(node) == true);
 * @param node Узел, который нужно оптимизировать.
 * @return     true, если была оптимизация. false, если дерево оптимизировать нельзя.
*/
bool OptimizeWorthlessNodes(TreeNode* node);

/**
 * @brief      Оптимизирует поддеревья, которые можно сократить. Например (обозначим за A - поддерево): A-A -> 0; A/A -> 1;
 *             Для корректной работы данной оптимизации первоначально выполнить
 *             while (OptimizeCalculatedNodes(node) == true); и 
 *             while (OptimizeWorthlessNodes(node) == true);
 * @param node Узел, который нужно оптимизировать.
 * @return     true, если была оптимизация. false, если дерево оптимизировать нельзя.
*/
bool OptimizeWorthlessTrees(TreeNode* node);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#ifdef GRAPHVIZ

extern size_t TreeGraphNodeId; 

bool CreateTreeGraph(const char* outImagefileName, TreeNode* node, bool openFile = false);

void CreateNodeGraph(FILE* file, TreeNode* node, size_t parentId, bool IsRight);

#else

#define CreateTreeGraph(fileName, tree) false;
#define CreateNodeGraph(file, node, parentId, IsRight) false;

#endif

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\


#endif