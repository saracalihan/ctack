#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>

#include "common.h"
#include "da.h"

bool is_integer(const char* str) {
    if (*str == '-' || *str == '+') {
        str++;
    }
    if (*str == '\0') {
        return false;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

bool is_null(const char* str) {
    return strcmp(str, "null") == 0;
}

bool is_boolean(const char* str) {
    return strcmp(str, "true") == 0 || strcmp(str, "false") == 0;
}

bool is_string(const char* str) {
    if (*str == '"') {
        str++;
        while (*str) {
            if (*str == '"' && *(str - 1) != '\\') {
                return *(str + 1) == '\0';
            }
            str++;
        }
    }
    return false;
}

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
    } else if (strncmp(str, "peek", 4) == 0) {
        token.type = TOKEN_PEEK;
    } else if (strncmp(str, "pick", 4) == 0) {
        token.type = TOKEN_PICK;
    } else if (strncmp(str, "+", 1) == 0) {
        token.type = TOKEN_ADD;
    } else if (strncmp(str, "-", 1) == 0 && !is_integer(str)) {
        token.type = TOKEN_SUB;
    } else if (strncmp(str, "println", 7) == 0) {
        token.type = TOKEN_PRINTLN;
    } else if (strncmp(str, "print", 5) == 0) {
        token.type = TOKEN_PRINT;
    } else if (strncmp(str, "/", 1) == 0) {
        token.type = TOKEN_DIV;
    } else if (strncmp(str, "*", 1) == 0) {
        token.type = TOKEN_MULT;
    } else if (strncmp(str, "dump", 4) == 0) {
        token.type = TOKEN_DUMP;
    } else if (strncmp(str, "&&", 2) == 0) {
        token.type = TOKEN_LOGIC_AND;
    }else if (strncmp(str, "||", 2) == 0) {
        token.type = TOKEN_LOGIC_OR;
    }else if (strncmp(str, "==", 2) == 0) {
        token.type = TOKEN_LOGIC_EQ;
    }else if (strncmp(str, "!=", 2) == 0) {
        token.type = TOKEN_LOGIC_NOT_EQ;
    }else if (strncmp(str, ">=", 2) == 0) {
        token.type = TOKEN_LOGIC_GTE;
    }else if (strncmp(str, "<=", 2) == 0) {
        token.type = TOKEN_LOGIC_LTE;
    }else if (strncmp(str, "!", 1) == 0) {
        token.type = TOKEN_LOGIC_NOT;
    }else if (strncmp(str, "<", 1) == 0) {
        token.type = TOKEN_LOGIC_LT;
    }else if (strncmp(str, ">", 1) == 0) {
        token.type = TOKEN_LOGIC_GT;
    }else if (strncmp(str, "if", 2) == 0) {
        token.type = TOKEN_IF;
    }else if (strncmp(str, "else", 4) == 0) {
        token.type = TOKEN_ELSE;
    }else if (strncmp(str, "end", 3) == 0) {
        token.type = TOKEN_END;
    } else if (strncmp(str, "var", 3) == 0) {
        token.type = TOKEN_VAR;
    } else if (strncmp(str, "store", 5) == 0) {
        token.type = TOKEN_STORE;
    } else if (strncmp(str, "load", 4) == 0) {
        token.type = TOKEN_LOAD;
    } else if (strncmp(str, "delete", 6) == 0) {
        token.type = TOKEN_DELETE;
    } else if (strncmp(str, "exit", 4) == 0) {
        token.type = TOKEN_EXIT;
    }else {
        if(!( // check data types
            is_integer(str) ||
            is_string(str)  ||
            is_boolean(str) ||
            is_null(str)
        )){
            token.type = TOKEN_VARIABLE_NAME;
            strcpy(token.value, str);
            return token;
            // CTACK_ERROR("[TOKENIZER ERROR] Invalid data value: '%s'\n", str);
            // exit(1);
        }
        token.type = TOKEN_DATA;
        sprintf(token.value, "%s", str);
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
            if(strncmp(p, "//", 2) == 0){ // skip comment
                while(*p != '\n'){
                    p++;
                }
                p++;
                continue;
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
