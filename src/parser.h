#ifndef PARSER_H
#define PARSER_H

#include "common.h"

Node node_create(NodeType type){
    Node n;
    n.type = type;
    n.data = NULL;

    return n;
}

void executation_stack_free(ExecutationStack* es){
    for(size_t i=0; i< es->count; i++){
        if(es->items[i].data != NULL){
            free(es->items[i].data);
        }
    }
    free(es->items);
}

stack_type node_stack_data_convert(const Node t){
    return *(stack_type*)t.data;
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
            case TOKEN_PUSH:
                n = node_create(NODE_PUSH);
                n.data = malloc(sizeof(stack_type));
                Token next_token = DA_SHIFT(*tokens);
                if(next_token.type != TOKEN_DATA){
                    printf("Push operation can not use with %i\n", next_token.type);
                    exit(1);
                }
                stack_type data = atoi(next_token.value); // convert data from string
                memcpy(n.data, &data, sizeof(stack_type));
            break;
            case TOKEN_POP:
                n = node_create(NODE_POP);
            break;
            case TOKEN_PEEK:
                n = node_create(NODE_PEEK);
            break;
            case TOKEN_ADD:
                n = node_create(NODE_ADD);
            break;
            case TOKEN_SUB:
                n = node_create(NODE_SUB);
            break;
            case TOKEN_DIV:
                n = node_create(NODE_DIV);
            break;
            case TOKEN_MULT:
                n = node_create(NODE_MULT);
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