#ifndef PARSER_H_
#define PARSER_H_


struct Parser
{
    TreeNode*       nodes;
    size_t          nodeIndex;
    size_t          nodesSize;

    MathParserError status;
};


#endif