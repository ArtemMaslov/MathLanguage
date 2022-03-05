#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <Windows.h>
#include <cmath>
#include <string.h>


#include "LanguageTree.h"
#include "..\Logs\Logs.h"

#include "MathDSL.h"


static double CalculateNode(const TreeNode* node, const Tree* problem, bool* canCalculate);

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///            Проведение вычислений, (подстановка значений переменных)
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

double TreeCalculate(const Tree* problem)
{
    LOG_TREE_DBG("TreeCalculate");

    assert(problem);

    return CalculateNode(problem->Root, problem, nullptr);
}

static double CalculateNode(const TreeNode* node, const Tree* problem, bool* canCalculate)
{
    assert(node);
    //assert(tree);
    //assert(canCalculate);

    switch (node->Value.Type)
    {
        case ML_TYPE_NUMBER:
            if (canCalculate)
                *canCalculate = true;
            return GET_NUM(node);

        case ML_TYPE_CONSTANT:
            if (canCalculate)
                *canCalculate = true;
            switch (node->Value.Constant)
            {
                case ML_CONST_PI:
                    return Pi;
                case ML_CONST_EXP:
                    return Exp;
            }
            break;

        case ML_TYPE_IDENTIFIER:
            // Вычислить значения нельзя.
            break;

        case ML_TYPE_STD_FUNCTION:
            if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER))
            {
                if (canCalculate)
                    *canCalculate = true;
                switch (node->Value.StdFunction)
                {
                    case ML_FUNC_SIN:
                        return sin(GET_NUM(node));
                    case ML_FUNC_COS:
                        return cos(GET_NUM(node));
                    case ML_FUNC_TG:
                        return tan(GET_NUM(node));
                    case ML_FUNC_CTG:
                        return 1.0 / tan(GET_NUM(node));
                    case ML_FUNC_SH:
                        return sinh(GET_NUM(node));
                    case ML_FUNC_CH:
                        return cosh(GET_NUM(node));
                    case ML_FUNC_LN:
                        return log(GET_NUM(node));
                    case ML_FUNC_SQRT:
                        return sqrt(GET_NUM(node));
                    case ML_FUNC_CBRT:
                        return cbrt(GET_NUM(node));
                    case ML_FUNC_ARCSIN:
                        return asin(GET_NUM(node));
                    case ML_FUNC_ARCCOS:
                        return acos(GET_NUM(node));
                    case ML_FUNC_ARCTG:
                        return atan(GET_NUM(node));
                    case ML_FUNC_ARCCTG:
                        return Pi/2 - atan(GET_NUM(node));
                }
            }
            break;

        case ML_TYPE_MATH_OPERATOR:
            double leftVal = CALC(LEFT);
            if (canCalculate && !(*canCalculate))
                return 0;

            double rightVal = CALC(RIGHT);
            if (canCalculate && !(*canCalculate))
                return 0;

            switch (node->Value.MathOperator)
            {
                case ML_MATH_ADDITION:
                    return leftVal + rightVal;
                case ML_MATH_SUBTRACTION:
                    return leftVal - rightVal;
                case ML_MATH_MULTIPLICATION:
                    return leftVal * rightVal;
                case ML_MATH_DIVISION:
                    return leftVal / rightVal;
                case ML_MATH_POWER:
                    return pow(leftVal, rightVal);
            }
            break;
    }
    
    if (canCalculate)
        *canCalculate = false;

    return 0;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///              Оптимизация структуры дерева, сокращение лишних узлов
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

static void OptimizeNodeToNumber(TreeNode* node, double number)
{
    assert(node);

    if (LEFT)
        TreeNodeDestructor(LEFT);
    if (RIGHT)
        TreeNodeDestructor(RIGHT);

    LEFT = RIGHT = nullptr;
    node->Value.Type = ML_TYPE_NUMBER;
    node->Value.Number = number;
}

/**
 * @brief      Оптимизирует дерево, выполняя все оптимизации
 *             (см в Tree.h функции MathTreeOptimize<X>, где <X> - вид оптимизации)
 * @param tree Указатель на дерево
*/
void MathTreeOptimize(TreeNode* problem)
{
    LOG_TREE_DBG("MathTreeOptimize");

    assert(problem);

    bool result = false;
    do
    { 
        result = false;
        while (MathTreeOptimizeCalculatedNodes(problem))
            result = true;
        while (OptimizeWorthlessNodes(problem))
            result = true;
        while (OptimizeWorthlessTrees(problem))
            result = true;
    }
    while (result);
}

/**
 * @brief      Оптимизирует узлы, которые можно вычислить. Например: (4+8)*9 -> 108; sin(Pi/6) -> 0.5;
 *             Тригонометрические функции вычисляются в радианах!
 * @param node Узел, который нужно оптимизировать.
 * @return     true, если была оптимизация. false, если дерево оптимизировать нельзя.
*/
bool MathTreeOptimizeCalculatedNodes(TreeNode* node)
{
    assert(node);

    bool result = false;
    if (LEFT)
        result |= MathTreeOptimizeCalculatedNodes(LEFT);
    if (RIGHT)
        result |= MathTreeOptimizeCalculatedNodes(RIGHT);

    if (TYPE_EQUAL(node, ML_TYPE_MATH_OPERATOR) || TYPE_EQUAL(node, ML_TYPE_STD_FUNCTION))
    {
        bool canCalculate = true;
        double calcRes = CalculateNode(node, nullptr, &canCalculate);

        if (!canCalculate)
            return result;
        
        OptimizeNodeToNumber(node, calcRes);
        return true;
    }
    return result;
}

/**
 * @brief      Оптимизирует узлы, которые можно сократить. Например: x*1 -> x; x^0 -> 1;
 *             Для корректной работы данной оптимизации первоначально выполнить 
 *             while (OptimizeCalculatedNodes(node) == true);
 * @param node Узел, который нужно оптимизировать.
 * @return     true, если была оптимизация. false, если дерево оптимизировать нельзя.
*/
bool OptimizeWorthlessNodes(TreeNode* node)
{
    assert(node);

    bool result = false;
    if (TYPE_EQUAL(node, ML_TYPE_MATH_OPERATOR))
    {
        switch (node->Value.MathOperator)
        {
            case ML_MATH_ADDITION:
            {
                // Оптимизируем:
                // 0 + x = x
                // x + 0 = x
                if (TYPE_EQUAL(LEFT, ML_TYPE_NUMBER) && GET_NUM(LEFT) == 0)
                {
                    TreeNodeDestructor(LEFT);
                    memmove(node, RIGHT, sizeof(TreeNode));
                    result = true;
                }
                else if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 0)
                {
                    TreeNodeDestructor(RIGHT);
                    memmove(node, LEFT, sizeof(TreeNode));
                    result = true;
                }
                break;
            }
            case ML_MATH_SUBTRACTION:
            {
                // 0 - x = -x (не оптимизируем).
                // x - 0 = x (оптимизируем)
                if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 0)
                {
                    TreeNodeDestructor(RIGHT);
                    memmove(node, LEFT, sizeof(TreeNode));
                    result = true;
                }
                break;
            }
            case ML_MATH_MULTIPLICATION:
            {
                // Оптимизируем:
                // 0 * x = 0
                // x * 0 = 0
                // 1 * x = x
                // x * 1 = x
                if (TYPE_EQUAL(LEFT, ML_TYPE_NUMBER)  && GET_NUM(LEFT) == 0 || 
                    TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 0)
                {
                    OptimizeNodeToNumber(node, 0);
                    result = true;
                }
                else if (TYPE_EQUAL(LEFT, ML_TYPE_NUMBER) && GET_NUM(LEFT) == 1)
                {
                    TreeNodeDestructor(LEFT);
                    memmove(node, RIGHT, sizeof(TreeNode));
                    result = true;
                }
                else if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 1)
                {
                    TreeNodeDestructor(RIGHT);
                    memmove(node, LEFT, sizeof(TreeNode));
                    result = true;
                }
                break;
            }
            case ML_MATH_DIVISION:
            {
                // Оптимизируем:
                // 0 / x = 0
                // x / 0 = ERROR
                // x / 1 = x
                if (TYPE_EQUAL(LEFT, ML_TYPE_NUMBER) && GET_NUM(LEFT) == 0)
                {
                    OptimizeNodeToNumber(node, 0);
                    result = true;
                }
                else if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 0)
                {
                    LOG_TREE_ERR("Внимание! В выражении выполнено деление на 0.");
                    return false;
                }
                else if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 1)
                {
                    TreeNodeDestructor(RIGHT);
                    CHANGE_NODE(LEFT);
                }
                break;
            }
            case ML_MATH_POWER:
            {
                // Оптимизируем:
                // x^1 = x
                // x^0 = 1
                // 1^x = 1
                // 0^x = 0
                // 0^0 = ERROR
                if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 1)
                {
                    TreeNodeDestructor(RIGHT);
                    CHANGE_NODE(LEFT);
                }
                else if (TYPE_EQUAL(LEFT, ML_TYPE_NUMBER) && GET_NUM(LEFT) == 0 && TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 0)
                {
                    LOG_TREE_ERR("Выражение 0^0 лишено смысла. Вычисление выражения будет прервано");
                    return false;
                }
                else if (TYPE_EQUAL(RIGHT, ML_TYPE_NUMBER) && GET_NUM(RIGHT) == 0 || 
                         TYPE_EQUAL(LEFT, ML_TYPE_NUMBER)  && GET_NUM(LEFT) == 1)
                {
                    OptimizeNodeToNumber(node, 1);
                    result = true;
                }
                else if (TYPE_EQUAL(LEFT, ML_TYPE_NUMBER) && GET_NUM(LEFT) == 0)
                {
                    OptimizeNodeToNumber(node, 0);
                    result = true;
                }
                break;
            }
        }
    }

    if (!result)
    {
        if (LEFT)
            result |= OptimizeWorthlessNodes(LEFT);
        if (RIGHT)
            result |= OptimizeWorthlessNodes(RIGHT);
    }
    return result;
}

/**
 * @brief      Оптимизирует поддеревья, которые можно сократить. Например (обозначим за A - поддерево): A-A -> 0; A/A -> 1;
 *             Для корректной работы данной оптимизации первоначально выполнить
 *             while (OptimizeCalculatedNodes(node) == true); и 
 *             while (OptimizeWorthlessNodes(node) == true);
 * @param node Узел, который нужно оптимизировать.
 * @return     true, если была оптимизация. false, если дерево оптимизировать нельзя.
*/
bool OptimizeWorthlessTrees(TreeNode* node)
{
    assert(node);

    bool result = false;
    // Оптимизируем поддеревья
    // A-A = 0
    // A/A = 1
    // A*A = A^2
    // A+A = 2*A
    if (LEFT)
        result = OptimizeWorthlessTrees(LEFT);
    if (RIGHT)
        result = OptimizeWorthlessTrees(RIGHT);

    if (TYPE_EQUAL(node, ML_TYPE_MATH_OPERATOR))
    {
        bool LeftEqualRight = CompareTrees(LEFT, RIGHT);
        if (LeftEqualRight)
        {
            switch (node->Value.MathOperator)
            {
                case ML_MATH_ADDITION:
                    node->Value.MathOperator = ML_MATH_MULTIPLICATION;
                    OptimizeNodeToNumber(LEFT, 2);
                    break;
                case ML_MATH_SUBTRACTION:
                    OptimizeNodeToNumber(node, 0);
                    break;
                case ML_MATH_MULTIPLICATION:
                    node->Value.MathOperator = ML_MATH_POWER;
                    OptimizeNodeToNumber(RIGHT, 2);
                    break;
                case ML_MATH_DIVISION:
                    OptimizeNodeToNumber(node, 1);
                    break;
            }
            result = true;
        }
    }
    return result;
}

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///                   Построение графа дерева с помощью GraphViz
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#ifdef GRAPHVIZ

size_t TreeGraphNodeId = 1;

bool CreateTreeGraph(const char* outImagefileName, TreeNode* node, bool openFile)
{
    LOG_TREE_DBG("CreateTreeGraph");

    assert(outImagefileName);
    assert(node);

    FILE* file = fopen("treeGraph.gv", "w");

    if (!file)
    {
        LOG_TREE_ERR("CreateTreeGraph: ошибка открытия файла.");
        return false;
    }
    CreateGraphCallCount++;
    TreeGraphNodeId = 1;

    fputs("digraph G\n"
          "{\n"
          "    bgcolor=\"lightblue\"\n"
          "    node [fontcolor=gray11, style=filled, fontsize = 18];\n", file);

    CreateNodeGraph(file, node, 0, true);

    fputs("}", file);

    fclose(file);

    char cmd[500];

    sprintf(cmd, "dot \"treeGraph.gv\" -Tsvg > \"%s%zd.svg\"", outImagefileName, CreateGraphCallCount);
    system(cmd);

    remove("treeGraph.gv");

    if (openFile)
    {
        sprintf(cmd, "%s%zd.svg", outImagefileName, CreateGraphCallCount);
        system(cmd);
    }

    return true;
}

void CreateNodeGraph(FILE* file, TreeNode* node, size_t parentId, bool IsRight)
{
    assert(file);
    assert(node);

    size_t _nodeId = TreeGraphNodeId;
    
    char closeLabelSymbol = '\"';

    switch (node->Value.Type)
    {
        case ML_TYPE_NUMBER:
            fprintf(file, "    node%zd[shape = pentagon,  fillcolor=goldenrod1,                     label = \"", _nodeId);
            break;                                                                                  
                                                                                                    
        case ML_TYPE_IDENTIFIER:                                                                    
            fprintf(file, "    node%zd[shape = oval, fillcolor=azure4,                         label = \"", _nodeId);
            break;                                                                                  
                                                                                                    
        case ML_TYPE_CONSTANT:                                                                      
            fprintf(file, "    node%zd[shape = house,     fillcolor=darkslateblue,                  label = \"", _nodeId);
            break;                                                                                  
                                                                                                    
        case ML_TYPE_MATH_OPERATOR:                                                                 
            fprintf(file, "    node%zd[shape = square,      fillcolor=cadetblue1,      fontsize = 28, label = \"", _nodeId);
            break;                                                                                  
                                                                                                    
        case ML_TYPE_STD_FUNCTION:                                                                  
            fprintf(file, "    node%zd[shape = hexagon,   fillcolor=cadetblue3,                     label = \"", _nodeId);
            break;                                                                                  
                                                                                                    
        case ML_TYPE_FUNC_OPERATOR:                                                                 
            fprintf(file, "    node%zd[shape = polygon,   fillcolor=burlywood3,                     label = \"", _nodeId);
            break;                                                                                  
                                                                                                    
        case ML_TYPE_LANG_CSTR:                                                                     
            fprintf(file, "    node%zd[shape = octagon,   fillcolor=coral3,                         label = \"", _nodeId);
            break;

        case ML_TYPE_SPECIAL_SYMBOL:
            fprintf(file, "    node%zd[shape = septagon,  fillcolor=darkolivegreen3, fontsize = 28, label = ", _nodeId);

            if (node->Value.SpecSymbol == ML_SPEC_DOUBLE_QUOTES)
            {
                fputc('<', file);
                closeLabelSymbol = '>';
            }
            else
                fputc('\"', file);
            break;

        case ML_TYPE_UNKNOWN:
        default:
            assert(!"Неизвестный тип лексемы");
            //fprintf(file, "    node%zd[shape = rect,       fillcolor=darkred,     label = \"", _nodeId);
            break;
    }
    
    PrintExpression(&node->Value, file);
    fputc(closeLabelSymbol, file);
    fputs("];\n", file);

    if (_nodeId > 1)
        fprintf(file, "        node%zd->node%zd;\n", _nodeId-1, _nodeId);

    TreeGraphNodeId++;

    if (parentId != 0)
        fprintf(file, "        node%zd -> node%zd [color=%s];\n", 
                parentId, _nodeId, IsRight?"darkorchid3":"coral1");

    if (LEFT)
        CreateNodeGraph(file, LEFT, _nodeId, false);
    
    if (RIGHT)
        CreateNodeGraph(file, RIGHT, _nodeId, true);
}

#endif

///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\
///***///***///---\\\***\\\***\\\___///***___***\\\___///***///***///---\\\***\\\***\\\

#include "UndefMathDSL.h"