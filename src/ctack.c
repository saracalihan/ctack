#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "common.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpretter.h"


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

    fseek(input, 0, SEEK_END);
    size_t file_size= ftell(input);
    fseek(input, 0, SEEK_SET);

    char input_str[file_size+1];
    memset(&input_str, 0, file_size+1);
    if(fread(&input_str, 1, file_size, input) ==0){
        CTACK_ERROR("Can not read '%s'\n", argv[1]);
        exit(-1);
    }

    Tokens tokens = tokenize_string(input_str);
    ExecutationStack es = parse(&tokens);
    tokens_free(&tokens);
    Stack stack = stack_create();
    int res = interpret(&es, &stack);
    executation_stack_free(&es);
    return 0;
}
