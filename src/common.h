#define CTACK_ERROR printf
#define STACK_SIZE 10
#define STACK_ERROR printf
#define STACK_SIZE 10
#define VARIABLE_NAME_SIZE 64

#ifndef COMMON_H
#define COMMON_H

#include "variant.h"
#include "variable.h"

typedef Variant stack_type;

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_PUSH,
    TOKEN_POP,
    TOKEN_PEEK,
    TOKEN_PICK,
    TOKEN_ADD,
    TOKEN_SUB,
    TOKEN_DIV,
    TOKEN_MULT,
    TOKEN_PRINT,
    TOKEN_PRINTLN,
    TOKEN_DUMP,
    TOKEN_DATA,
    TOKEN_LOGIC_AND,
    TOKEN_LOGIC_OR,
    TOKEN_LOGIC_GT,
    TOKEN_LOGIC_LT,
    TOKEN_LOGIC_GTE,
    TOKEN_LOGIC_LTE,
    TOKEN_LOGIC_NOT,
    TOKEN_LOGIC_EQ,
    TOKEN_LOGIC_NOT_EQ,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_END,
    TOKEN_VARIABLE_NAME,
    TOKEN_VAR,
    TOKEN_STORE,
    TOKEN_LOAD,
    TOKEN_DELETE,
    TOKEN_EXIT,
} TokenTypes;

typedef struct {
    TokenTypes type;
    char value[64];
} Token;

typedef struct {
    Token *items;
    size_t count;
    size_t capacity;
} Tokens;

// Forward declaration of Node
typedef struct Node Node;

typedef struct ExecutationStack {
    Node *items;
    size_t count;
    size_t capacity;
    size_t top;
    VariableHashTable variable_table;
} ExecutationStack;

typedef enum {
    NODE_UNKNOWN,
    NODE_PUSH,
    NODE_POP,
    NODE_PEEK,
    NODE_PICK,
    NODE_PRINT,
    NODE_PRINTLN,
    NODE_DUMP,
    NODE_ARITHMETIC_OPS,
    NODE_LOGIC_OPS,
    NODE_IF_STMT,
    NODE_VARIABLE_OPS,
    NODE_EXIT,
} NodeType;

typedef enum{
    VARIABLE_DECLARATION,
    VARIABLE_LOAD,
    VARIABLE_STORE,
    VARIABLE_DELETE,
} VariableOperationType;

struct Node {
    NodeType type;
    union {
        void *data; // NODE_DATA
        struct {    // NODE_IF_STMT
            ExecutationStack consequent;
            ExecutationStack alternate;
        } if_statement;
        struct {
            VariableOperationType type;
            char name[VARIABLE_NAME_SIZE];
        } variable_ops;
    };
};

typedef struct {
    stack_type *items;
    int top;
} Stack;

#endif // COMMON_H
