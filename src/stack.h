#ifndef STACK_H
#define STACK_H

#include "common.h"


Stack stack_create(){
    Stack s;
    s.items = malloc(sizeof(stack_type) * STACK_SIZE);
    if(NULL == s.items){
        CTACK_ERROR("Stack create error!");
        exit(-1);
    }
    s.top= -1;
    return s;
}

void stack_free(Stack *s){
    free(s->items);
}

bool stack_is_empty(const Stack s){
    return -1 == s.top;
}

bool stack_is_full(const Stack s){
    return STACK_SIZE == s.top+1;
}

int stack_push(Stack *s, stack_type data){
    if(stack_is_full(*s)){
        CTACK_ERROR("Stack overflow!\n");
        exit(-1);
    }
    s->items[++s->top] = data;
    return s->top;
}

stack_type stack_pop(Stack *s){
    if(stack_is_empty(*s)){
        CTACK_ERROR("Stack underflow!\n");
        exit(-1);
    }
    return s->items[s->top--];
}

stack_type stack_peek(const Stack *s){
    if(stack_is_empty(*s)){
        CTACK_ERROR("Stack is empty!");
        exit(-1);
    }
    return s->items[s->top];
}

stack_type stack_get(const Stack *s, size_t index){
    if(stack_is_empty(*s)){
        CTACK_ERROR("Stack is empty!");
        exit(-1);
    }
    if(index < 0 || index > s->top){
        CTACK_ERROR("Stack out of range! Stack size is %d given index is %zu.\n", s->top +1, index);
        exit(-1);
    }

    return s->items[index];
}

#endif // STACK_H
