
#define SET_NUMBER(number, node) (node)->Value.me_number = number

#define SET_FUNCTION(function, node) (node)->Value.me_function = function

#define SET_OPERATOR(operator, node) (node)->Value.me_operator = operator

#define SET_CONSTANT(constant, node) (node)->Value.me_constant = constant

#define SET_TYPE(expr_type, node) (node)->Value.Type = expr_type

#define GET_TYPE(node) (node)->Value.Type
#define TYPE_EQUAL(node, Type) (GET_TYPE(node) == Type)
#define GET_NUM(node) (node)->Value.Number
#define GET_OPER(node) (node)->Value.MathOperator
#define GET_FUNC(node) (node)->Value.StdFunction
#define GET_CONST(node) (node)->Value.Constant
#define GET_VAR(node) (node)->Value.Identifier

#define LEFT node->NodeLeft
#define RIGHT node->NodeRight
#define X node->nodeRight
#define F node

#define FUNC(Type, arg)                                         \
    NewFunctionNode(Type, arg)

#define NUM(number)                                             \
    NewNumberNode(number)

#define NODE_EXP NewConstantNode(ME_EXP)

#define NODE_PI  NewConstantNode(ME_PI)

#define COPY(node)                                              \
    TreeCopyRecursive(node)

#define ADD(loper, roper)                                       \
    NewOperatorNode(ME_ADDITION, loper, roper)

#define SUB(loper, roper)                                       \
    NewOperatorNode(ME_SUBTRACTION, loper, roper)

#define MUL(loper, roper)                                       \
    NewOperatorNode(ME_MULTIPLICATION, loper, roper)

#define DIV(loper, roper)                                       \
    NewOperatorNode(ME_DIVISION, loper, roper)

#define POW(loper, roper)                                       \
    NewOperatorNode(ME_POWER, loper, roper)

#define POW2(loper)                                             \
    NewOperatorNode(ME_POWER, loper, NUM(2))

#define NEW_NODE(value, Type, leftNode, rightNode)              \
    {                                                           \
        TreeNode* node = TreeNodeConstructor(nullptr);          \
                                                                \
        if (!node)                                              \
            return nullptr;                                     \
                                                                \
        if (leftNode)                                           \
            TreeAddLeftNode(node, leftNode);                    \
        if (rightNode)                                          \
            TreeAddRightNode(node, rightNode);                  \
                                                                \
        SET_##Type(value, node);                                \
        SET_TYPE(ME_##Type, node);                              \
        return node;                                            \
    }

#define CHECK_NODE(node)                                        \
    if (!(node))                                                \
    {                                                           \
        TreeNodeDestructor(node);                               \
        return nullptr;                                         \
    }

#define CALC(node) CalculateNode(node, problem, canCalculate)

#define CHANGE_NODE(child)                                      \
    {                                                           \
        TreeNode* left = child->NodeLeft;                       \
        TreeNode* right = child->NodeRight;                     \
        node->Value = child->Value;                             \
        node->ChildCount = child->ChildCount;                   \
        node->NodeLeft = left;                                  \
        node->NodeRight = right;                                \
        node->Allocated = child->Allocated;                     \
        result = true;                                          \
    }
