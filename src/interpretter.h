#include "common.h"
#include "stack.h"
#include "variant.h"

int interpret(ExecutationStack* es){
    Stack stack = stack_create();
    if(es->count == 0){
        return 1;
    }

    while(es->top < es->count){
        Node* node = &es->items[es->top];
        stack_type a,b;
        switch(node->type){
            case NODE_PUSH:
                stack_push(&stack, node_stack_data_convert(*node));
                break;
            case NODE_POP:
                stack_pop(&stack);
                break;
            case NODE_ADD:
                b = stack_pop(&stack);
                a = stack_pop(&stack);
                stack_push(&stack, variant_add(a, b));
                break;
            case NODE_SUB:
                b = stack_pop(&stack);
                a = stack_pop(&stack);
                stack_push(&stack, variant_sub(a, b));
                break;
            case NODE_MULT:
                b = stack_pop(&stack);
                a = stack_pop(&stack);
                stack_push(&stack, variant_mult(a, b));
                break;
            case NODE_DIV:
                b = stack_pop(&stack);
                a = stack_pop(&stack);
                stack_push(&stack, variant_div(a, b));
                break;
            case NODE_PRINT:
                stack_type data = stack_peek(&stack);
                printf("%s\n", variant_to_string(&data));
                break;
            case NODE_DUMP:
                printf("Stack(%i):\n", stack.top +1 );
                for(int i=0;i<= stack.top; i++){
                    printf("| %s ", variant_to_string(&stack.items[i]));
                }
                printf("\n");
                break;
        }
        es->top++;
    }
    stack_free(&stack);
return 1;
}
