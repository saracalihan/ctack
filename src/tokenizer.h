#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>

#include "common.h"
#include "da.h"

Tokens tokens_create(){
    Tokens t;
    memset(&t, 0, sizeof(Tokens));
    t.items = malloc(sizeof(Token) * 10);
    if(NULL == t.items){
        CTACK_ERROR("Tokens malloc error");
        exit(1);
    }
    t.capacity = 10;
    return t;
}

void tokens_free(Tokens* t){
    free(t->items);
}

Token token_create(const char* str){
    Token token;

    if (strncmp(str, "push", 4) == 0) {
        token.type = TOKEN_PUSH;
    } else if (strncmp(str, "pop", 3) == 0) {
        token.type = TOKEN_POP;
    } else if (strncmp(str, "peek", 3) == 0) {
        token.type = TOKEN_POP;
    }else if (strncmp(str, "+", 3) == 0) {
        token.type = TOKEN_ADD;
    } else if (strncmp(str, "-", 3) == 0) {
        token.type = TOKEN_SUB;
    } else if (strncmp(str, "print", 5) == 0) {
        token.type = TOKEN_PRINT;
    } else if (strncmp(str, "/", 5) == 0) {
        token.type = TOKEN_DIV;
    } else if (strncmp(str, "*", 5) == 0) {
        token.type = TOKEN_MULT;
    } else if (strncmp(str, "dump", 5) == 0) {
        token.type = TOKEN_DUMP;
    } else {
        token.type = TOKEN_DATA;
        sprintf(&token.value, "%s", str);
    }
    return token;
}

Tokens tokenize_string(const char* code){
    Tokens tokens = tokens_create();

    const char* p = code;
    bool is_string = false;
    while (*p) {
        // Skip spaces and newlines if not inside a string
        if (!is_string) {
            while (*p && isspace(*p)) {
                p++;
            }
        }

        if (*p) {
            const char *start = p;

            // Handle string literals
            if (*p == '"') {
                is_string = !is_string;
                p++; // Skip the opening quote
                start = p;

                while (*p && (*p != '"' || (*(p-1) == '\\' && is_string))) {
                    p++;
                }

                if (*p == '"') {
                    is_string = !is_string;
                    int length = p - start;
                    char command[length + 3]; // 2 " and null char
                    memset(&command, 0, sizeof(char) * (length + 3));
                    command[0] = '"';
                    strncpy(command+1, start, length);
                    command[length+1] = '"';
                    DA_PUSH(tokens, token_create(command));
                    p++; // Skip the closing quote
                }
            } else {
                while (*p && (!isspace(*p) || is_string)) {
                    p++;
                }

                int length = p - start;
                char command[length + 1];
                memset(&command, 0, sizeof(char) * (length + 1));
                strncpy(command, start, length);
                DA_PUSH(tokens, token_create(command));
            }
        }
    }

    return tokens;
}



#endif // TOKENIZER_H
