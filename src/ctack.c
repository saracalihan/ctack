#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STACK_SIZE 10
#define CTACK_ERROR printf

typedef int stack_type;

typedef struct{
    stack_type* items;
    int top;
} Stack;

Stack stack_create(){
    Stack s;
    s.items = malloc(sizeof(stack_type) * STACK_SIZE);
    if(NULL == s.items){
        CTACK_ERROR("Stack create error!");
        exit(-1);
    }
    s.top=-1;
    return s;
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

void stack_free(Stack *s){
    free(s->items);
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

int execute_command(Stack *stack, char *command){
    if(NULL != strstr(command, "//")){
        printf("yorum\n");
        return 1;
    }
     if (strncmp(command, "push", 4) == 0) {
        int value = atoi(command + 5);
        stack_push(stack, value);
    } else if (strncmp(command, "pop", 3) == 0) {
        stack_pop(stack);
    } else if (strncmp(command, "+", 3) == 0) {
        int b = stack_pop(stack);
        int a = stack_pop(stack);
        stack_push(stack, a + b);
    } else if (strncmp(command, "-", 3) == 0) {
        int b = stack_pop(stack);
        int a = stack_pop(stack);
        stack_push(stack, a - b);
    } else if (strncmp(command, "/", 3) == 0) {
        int b = stack_pop(stack);
        int a = stack_pop(stack);
        stack_push(stack, a / b);
    } else if (strncmp(command, "*", 3) == 0) {
        int b = stack_pop(stack);
        int a = stack_pop(stack);
        stack_push(stack, a * b);
    } else if (strncmp(command, "print", 5) == 0) {
        printf("%d\n", stack_peek(stack));
    }  else if (strncmp(command, "dump", 5) == 0) {
        printf("Stack:\n");
        for(int i=0;i<= stack->top; i++){
            char str[256]={0};
            sprintf(str, "%i", stack->items[i]); // for variant translation
            printf(" %s |", str);
        }
        printf("\n");
    } else {
        printf("Unknown command: '%s'\n", command);
        exit(1);
    }
    return 1;
}

int run_program(char **program, size_t count){
    int programSize = 11;//sizeof(program) / sizeof(program[0]);
    Stack s = stack_create();

    for (int i = 0; i < programSize; i++) {
        execute_command(&s, program[i]);
    }
    stack_free(&s);
    return 1;
}
int main( int argc, char** argv){
    if(argc<2){
        CTACK_ERROR("usage: cstack <input-file>\n");
        exit(-1);
    }

    FILE *input = fopen(argv[1], "r");
    if(NULL == input){
        CTACK_ERROR("Can not open '%s' because %m\n", argv[1]);
        exit(-1);
    }

    char * commands[1024];
    size_t command_count=0, len=0;
    while(getline(&commands[command_count], &len, input) != -1){
        if(commands[command_count][strlen(commands[command_count])-1] == '\n'){
            commands[command_count][strlen(commands[command_count])-1] = '\0';
        }
        command_count++;
    }
    run_program(commands, command_count);

    return 0;
}