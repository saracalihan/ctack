#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "tokenizer.h"

Node node_create(NodeType type){
    Node n;
    memset(&n, 0, sizeof(n));
    n.type = type;

    if(type == NODE_IF_STMT){
        DA_INIT(n.if_statement.consequent,1);
        DA_INIT(n.if_statement.alternate,1);
    }
    return n;
}

void executation_stack_free(ExecutationStack* es){
    for(size_t i=0; i< es->count; i++){
        if(
            es->items[i].data != NULL
            && 
            (
                es->items[i].type != NODE_ARITHMETIC_OPS &&
                es->items[i].type != NODE_LOGIC_OPS && 
                es->items[i].type != NODE_VARIABLE_OPS
            )
        ){
            free(es->items[i].data);
        }

        if(es->items[i].type != NODE_VARIABLE_OPS){
            // SİLLLL *******************
        }

    }
    free(es->items);
}

stack_type node_stack_data_convert(const Node t){
    return *(stack_type*)t.data;
}

stack_type data_to_variant(const Token* t){
    stack_type data;
    char* s = t->value;
    if(is_integer(s)){
        data = variant_create_int(atoi(s));
    } else if(is_string(s)){
        char* str = strdup(s+1);
        // remove "" chars
        str[strlen(str) -1] = '\0';
        data = variant_create_string(str);
    } else if(is_boolean(s)){
        data = variant_create_bool(strcmp(s, "true") == 0);
    } else if(is_null(s)){
        data = variant_create_null(1);
    } else {
        CTACK_ERROR("[PARSER ERROR] Unexpected data type!\nValue: '%s'\n", s);
        exit(1);
    }
    return data;
}

typedef struct{
  ExecutationStack **items;
  size_t count;
  size_t capacity;
} ESScopes;

ExecutationStack parse(Tokens* tokens){
    ExecutationStack es = {0};
    ESScopes scopes = {0};
    DA_INIT(scopes, 1);

    ExecutationStack* esp = &es;
    DA_PUSH(scopes, esp);
    // its point es or temp for loop/statements nodes
    // With esp, we dont need to recursive calls.
    // When you need recursive call you can switch the stacks

    DA_INIT(es,1);
    if(tokens->count == 0){
        return *esp;
    }
    while(tokens->count > 0){
        Node n = {0};
        Token t = DA_SHIFT(*tokens);
        switch(t.type){
            case TOKEN_DATA:
                n = node_create(NODE_PUSH);
                n.data = malloc(sizeof(stack_type));
                {
                    stack_type data = data_to_variant(&t);
                    memcpy(n.data, &data, sizeof(stack_type));
                }
            break;
            case TOKEN_PUSH:
                n = node_create(NODE_PUSH);
                n.data = malloc(sizeof(stack_type));
                {
                    Token next_token = DA_SHIFT(*tokens);
                    if(next_token.type != TOKEN_DATA){
                        printf("[PARSE ERROR]Push operation can not use with %i typed enum\n", next_token.type);
                        exit(1);
                    }
                    {
                        stack_type data = data_to_variant(&next_token);
                        memcpy(n.data, &data, sizeof(stack_type));
                    }
                }
            break;
            case TOKEN_POP:
                n = node_create(NODE_POP);
            break;
            case TOKEN_PEEK:
                n = node_create(NODE_PEEK);
            break;
            case TOKEN_PICK:
                n = node_create(NODE_PICK);
                n.data = malloc(sizeof(size_t));
                {
                    Token next_token = DA_SHIFT(*tokens);
                    if(next_token.type != TOKEN_DATA){
                        printf("[PARSE ERROR] Pick operation can not use with %i typed enum\n", next_token.type);
                        exit(1);
                    }
                    int pick_index = atoi(next_token.value);
                    if(pick_index <0){
                        printf("[PARSE ERROR] Can not pick from stack index of %d\n", pick_index);
                        exit(1);
                    }
                    size_t d = (size_t)pick_index;
                    memcpy(n.data, &d, sizeof(d));
                }
            break;
            case TOKEN_ADD:
            case TOKEN_SUB:
            case TOKEN_DIV:
            case TOKEN_MULT:
                n = node_create(NODE_ARITHMETIC_OPS);
                n.data = (void *)t.type;
            break;
            case TOKEN_LOGIC_AND:
            case TOKEN_LOGIC_OR:
            case TOKEN_LOGIC_NOT:
            case TOKEN_LOGIC_EQ:
            case TOKEN_LOGIC_NOT_EQ:
            case TOKEN_LOGIC_GT:
            case TOKEN_LOGIC_GTE:
            case TOKEN_LOGIC_LT:
            case TOKEN_LOGIC_LTE:
              n = node_create(NODE_LOGIC_OPS);
                n.data = (void *)t.type;
            break;
            case TOKEN_PRINT:
                n = node_create(NODE_PRINT);
            break;
            case TOKEN_PRINTLN:
                n = node_create(NODE_PRINTLN);
            break;
            case TOKEN_DUMP:
                n = node_create(NODE_DUMP);
            break;
            case TOKEN_IF:
                    // DONT USE REFERANCE VALUES HERE
                    DA_PUSH(*esp, node_create(NODE_IF_STMT));
                    DA_PUSH(scopes, esp);

                    // switch the stack content
                    // all nodes push into if's consequent stack 
                    esp = &(DA_GET_LAST(*esp).if_statement.consequent);
                        continue; // go to firs step
            case TOKEN_ELSE:
                Node* if_node =  &DA_GET_LAST(*DA_GET_LAST(scopes));
                if(if_node->type != NODE_IF_STMT){
                    CTACK_ERROR("[PARSE ERROR] Else must be has if keyword on before\n");
                    exit(1);
                }
                esp = &(if_node->if_statement.alternate);
            continue;
            case TOKEN_END:
                esp = DA_POP(scopes);
            continue;
            case TOKEN_VAR:
                {
                    Token next_token = DA_SHIFT(*tokens);
                    if(next_token.type != TOKEN_VARIABLE_NAME){
                        printf("[PARSE ERROR] Can not create variable without name!\n");
                        exit(1);
                    }
                    n = node_create(NODE_VARIABLE_OPS);
                    n.type = NODE_VARIABLE_OPS;
                    n.variable_ops.type = VARIABLE_DECLARATION;
                    strcpy(n.variable_ops.name, next_token.value);
                }
            break;
            case TOKEN_LOAD:
                {
                    Token next_token = DA_SHIFT(*tokens);
                    if(next_token.type != TOKEN_VARIABLE_NAME){
                        printf("[PARSE ERROR] Can not load variable without name!\n");
                        exit(1);
                    }
                    n = node_create(NODE_VARIABLE_OPS);
                    n.type = NODE_VARIABLE_OPS;
                    n.variable_ops.type = VARIABLE_LOAD;
                    strcpy(n.variable_ops.name, next_token.value);
                }
            break;
            case TOKEN_STORE:
                {
                    Token next_token = DA_SHIFT(*tokens);
                    if(next_token.type != TOKEN_VARIABLE_NAME){
                        printf("[PARSE ERROR] Can not store variable without name!\n");
                        exit(1);
                    }
                    n = node_create(NODE_VARIABLE_OPS);
                    n.type = NODE_VARIABLE_OPS;
                    n.variable_ops.type = VARIABLE_STORE;
                    strcpy(n.variable_ops.name, next_token.value);
                }
            break;
            case TOKEN_DELETE:
                {
                    Token next_token = DA_SHIFT(*tokens);
                    if(next_token.type != TOKEN_VARIABLE_NAME){
                        printf("[PARSE ERROR] Can not delete variable without name!\n");
                        exit(1);
                    }
                    n = node_create(NODE_VARIABLE_OPS);
                    n.type = NODE_VARIABLE_OPS;
                    n.variable_ops.type = VARIABLE_DELETE;
                    strcpy(n.variable_ops.name, next_token.value);
                }
            break;
            case TOKEN_EXIT:
                n.type = NODE_EXIT;
            break;
        }

        DA_PUSH(*esp, n);
    }

    DA_FREE(scopes);
    return *esp;
}

#endif // PARSER_H
