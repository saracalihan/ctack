#include "common.h"
#include "stack.h"
#include "variant.h"
#include "da.h"

typedef bool (*LOGICAL_FUNC_TWO_PARAMATER)(Variant, Variant);

typedef struct{
    char* name;
    Variant value;
}NameAndValue;

typedef struct {
    NameAndValue* items;
    size_t count;
    size_t capacity;
} VariableNames;

VariableNames get_variable_names(ExecutationStack* es){
    VariableNames names ={0};
    DA_INIT(names,1);
    for(int i=0; i< es->variable_table.capacity; i++){
        VariableHashTableNode itr=DA_GET(es->variable_table, i);

        if(itr.variable.key[0] !='\0'){
            NameAndValue nv = {
                .name = strdup(itr.variable.key),
                .value = itr.variable.value,
            };
            DA_PUSH(
                names,
                nv
            );

            for(size_t j=0; j<itr.nexts.count; j++){
                nv.name = strdup(itr.nexts.items[j].key);
                nv.value = itr.nexts.items[j].value;

                DA_PUSH(names, nv);
            }
        }
    }
    return names;
}

int interpret(ExecutationStack* es, Stack* stack){
    if(es->count == 0){
        return 1;
    }
    Variant (*variant_arith_funcs[])(Variant, Variant) = {
        [TOKEN_ADD] = &variant_add,
        [TOKEN_SUB] = &variant_sub,
        [TOKEN_DIV] = &variant_div,
        [TOKEN_MULT] = &variant_mult,
    };
    LOGICAL_FUNC_TWO_PARAMATER not_func = (LOGICAL_FUNC_TWO_PARAMATER)variant_not;
    bool (*variant_logic_funcs[])(Variant, Variant) = {
        [TOKEN_LOGIC_AND] = &variant_and,
        [TOKEN_LOGIC_OR] = &variant_or,
        [TOKEN_LOGIC_GT] = &variant_gt,
        [TOKEN_LOGIC_LT] = &variant_lt,
        [TOKEN_LOGIC_GTE] = &variant_gte,
        [TOKEN_LOGIC_LTE] = &variant_lte,
        [TOKEN_LOGIC_NOT] = &not_func,
        [TOKEN_LOGIC_EQ] = &variant_eq,
        [TOKEN_LOGIC_NOT_EQ] = &variant_not_eq,

    };
    while(es->top < es->count){
        Node* node = &es->items[es->top];
        stack_type a,b;
        switch(node->type){
            case NODE_PUSH:
                stack_push(stack, node_stack_data_convert(*node));
                break;
            case NODE_POP:
                stack_pop(stack);
                break;
            case NODE_PEEK:
                stack_push(stack, stack_peek(stack));
                break;
            case NODE_PICK:
                stack_push(stack, stack_get(stack, *(size_t*)node->data));
                break;
            case NODE_ARITHMETIC_OPS:
                b = stack_pop(stack);
                a = stack_pop(stack);
                stack_push(stack, variant_arith_funcs[(size_t)(TokenTypes*)node->data](a, b));
                break;
            case NODE_LOGIC_OPS:
                b = stack_pop(stack);
                a = stack_pop(stack);
                bool is_true = variant_logic_funcs[(size_t)(TokenTypes*)node->data](a, b);
                stack_push(stack, variant_create_bool(is_true));
                break;
            case NODE_PRINT:
                {
                    stack_type data = stack_peek(stack);
                    printf("%s\n", variant_to_string(&data));
                }
                break;
            case NODE_DUMP:
            {
                VariableNames variable_names =get_variable_names(es);

                printf("Variables(%li):\n", variable_names.count);
                NameAndValue var;
                for(size_t i=0;i< variable_names.count; i++){
                    var = DA_GET(variable_names, i);
                    char* value_str = variant_to_printf_string(&var.value);
                    printf("  %s = %s\n", var.name, value_str);
                }

                printf("Stack(%i):\n  ", stack->top +1 );
                for(int i=0;i<= stack->top; i++){
                    printf("| %s ", variant_to_printf_string(&stack->items[i]));
                }
                printf("\n");
                break;
            }
            case NODE_IF_STMT:
                stack_type data = stack_pop(stack);
                interpret(
                    variant_is_truthy(data)
                        ? &node->if_statement.consequent
                        : &node->if_statement.alternate
                    ,stack
                );
            break;
            case NODE_VARIABLE_OPS:
                switch (node->variable_ops.type){

                    case VARIABLE_DECLARATION:
                        size_t hash = variable_hashtable_add(
                            &es->variable_table,
                            node->variable_ops.name,
                            (Variant){
                                .type = TYPE_UNDEFINED,
                        });
                    break;
                    case VARIABLE_STORE:
                        variable_hashtable_set(
                            &es->variable_table,
                            node->variable_ops.name,
                            stack_peek(stack)
                        );
                    break;
                    case VARIABLE_LOAD:
                    {
                        Variable* var = variable_hashtable_get(
                            &es->variable_table,
                            node->variable_ops.name
                        );
                        stack_push(stack, var->value);
                    }
                    break;
                    case VARIABLE_DELETE:
                        variable_hashtable_delete(
                            &es->variable_table,
                            node->variable_ops.name
                        );
                    break;
                }
                break;
        }
        es->top++;
    }
    return 1;
}
