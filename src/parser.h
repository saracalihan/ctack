#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "tokenizer.h"

Node node_create(NodeType type){
    Node n;
    n.type = type;
    n.data = NULL;

    return n;
}

void executation_stack_free(ExecutationStack* es){
    for(size_t i=0; i< es->count; i++){
        if(
            es->items[i].data != NULL
            && 
            (
                es->items[i].type != NODE_ARITHMETIC_OPS &&
                es->items[i].type != NODE_LOGIC_OPS
            )
        ){
            free(es->items[i].data);
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

ExecutationStack parse(Tokens* tokens){
    ExecutationStack es = {0};
    DA_INIT(es,1);
    if(tokens->count == 0){
        return es;
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
                Token next_token = DA_SHIFT(*tokens);
                if(next_token.type != TOKEN_DATA){
                    printf("[PARSE ERROR]Push operation can not use with %i typed enum\n", next_token.type);
                    exit(1);
                }
                {
                    stack_type data = data_to_variant(&next_token);
                    memcpy(n.data, &data, sizeof(stack_type));
                }
            break;
            case TOKEN_POP:
                n = node_create(NODE_POP);
            break;
            case TOKEN_PEEK:
                n = node_create(NODE_PEEK);
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
            case TOKEN_DUMP:
                n = node_create(NODE_DUMP);
            break;
        }

        DA_PUSH(es, n);
    }
    return es;
}

#endif // PARSER_H