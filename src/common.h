#define CTACK_ERROR printf
#define STACK_SIZE 10
#ifndef COMMON_H
#define COMMON_H

#include "variant.h"

typedef Variant stack_type;


typedef enum{
    TOKEN_UNKNOWN,
    TOKEN_PUSH,
    TOKEN_POP,
    TOKEN_PEEK,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_DIV,
    TOKEN_MULT,
    TOKEN_PRINT,
    TOKEN_DUMP,
    TOKEN_DATA,
}TokenTypes;

typedef struct{
    TokenTypes type;
    char value[64];
}Token;

typedef struct {
    Token *items;
    size_t count;
    size_t capacity;
}Tokens;

typedef enum{
    NODE_UNKNOWN,
    NODE_PUSH,
    NODE_POP,
    NODE_PEEK,
    NODE_PRINT,
    NODE_DUMP,
    NODE_ADD,
    NODE_SUB,
    NODE_MULT,
    NODE_DIV,
}NodeType;

typedef struct{
    NodeType type;
    void *data; // variable data or func paramaters and body... 
} Node;

typedef struct{
    Node *items;
    size_t count;
    size_t capacity;
    size_t top;
}ExecutationStack;

typedef struct{
    stack_type* items;
    int top;
} Stack;

#endif // COMMON_H