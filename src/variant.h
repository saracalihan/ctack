#ifndef VARIANT_H
#define VARIANT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "common.h"


typedef enum {
    TYPE_NULL,
    TYPE_INT,
    TYPE_BOOL,
    TYPE_STRING,
} VariantType;

typedef struct {
    VariantType type;
    union {
        int int_value;
        bool bool_value;
        bool null_value;
        char *string_value;
    };
} Variant;

Variant variant_create_int(int d){
    return (Variant){
        .type = TYPE_INT,
        .int_value = d,
    };
}

Variant variant_create_bool(bool d){
    return (Variant){
        .type = TYPE_BOOL,
        .bool_value = d,
    };
}
Variant variant_create_string(char* d){
    return (Variant){
        .type = TYPE_STRING,
        .string_value = d,
    };
}
Variant variant_create_null(bool d){
    return (Variant){
        .type = TYPE_NULL,
        .null_value = d,
    };
}

char* variant_to_string(const Variant* v){
    char str[256] = {0};
    if(v->type == TYPE_STRING){
        return v->string_value;
    }

    switch (v->type){

        case TYPE_BOOL:
            strcpy(str, v->bool_value ? "true" : "false");
            break;
        case TYPE_INT:
            sprintf(str, "%d", v->int_value);
            break;
        case TYPE_NULL:
            sprintf(str, "null");
            break;
    }
    return strdup(str);
}

// char* variant_to_printf_string(const Variant* v){
//     char *s = variant_to_string(v);
//     if(v->type == TYPE_STRING){
//         size_t len = strlen(s)+3;
//         char temp[len];
//         memset(&temp, 0, len);
//         // printf("p: %s\n", s);
//         sprintf(&temp, "'%s'", s);
//         free(s);
//         return strdup(temp);
//     }
//     return s;
// }

Variant variant_add(Variant a, Variant b){
    if(a.type != b.type) {
        CTACK_ERROR( "[PLUS OPERATOR]: Can not add %s and %s\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }

    Variant result;

    result.type = a.type;
    if (a.type == TYPE_INT) {
        result.int_value = a.int_value + b.int_value;
    } else if (a.type == TYPE_STRING) {
        result.string_value = (char *)malloc(strlen(a.string_value) + strlen(b.string_value) + 1);
        strcpy(result.string_value, a.string_value);
        strcat(result.string_value, b.string_value);
    } else if (a.type == TYPE_BOOL) {
        result.bool_value = (bool)(a.bool_value | b.bool_value);
    }   else {
        CTACK_ERROR( "[PLUS OPERATOR]: This type not implemented yet! Type number: %ul\n", a.type);
        exit(1);
    }

    return result;
}

Variant variant_sub(Variant a, Variant b){
    if(a.type != b.type) {
             CTACK_ERROR( "[SUBTRACTION OPERATOR]: Can not subtract %s and %s\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }

    Variant result;

    result.type = a.type;
    if (a.type == TYPE_INT) {
        result.int_value = a.int_value - b.int_value;
    } else if (a.type == TYPE_STRING) {
        CTACK_ERROR( "[SUBTRACTION OPERATOR]: Can not subtract string types\n");
        exit(1);
    } else if (a.type == TYPE_BOOL) {
        int r = a.int_value - b.int_value;
        if(r<0){
            r =0;
        }
        result.bool_value = r;
    }   else {
        CTACK_ERROR( "[SUBTRACTION OPERATOR]: This type not implemented yet! Type number: %ul\n", a.type);
        exit(1);
    }

    return result;
}

Variant variant_mult(Variant a, Variant b){
    if(a.type != b.type) {
        CTACK_ERROR( "[MULTIPLY OPERATOR]: Can not multiply %s and %s\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }

    Variant result;

    result.type = a.type;
    if (a.type == TYPE_INT) {
        result.int_value = a.int_value * b.int_value;
    } else {
        CTACK_ERROR( "[MULTIPLY OPERATOR]: Only intagers can multiply!\n");
        exit(1);
    }
    return result;
}
Variant variant_div(Variant a, Variant b){
        if(a.type != b.type) {
        CTACK_ERROR( "[DIVIDE OPERATOR]: Can not divide %s and %s\n", variant_to_string(&a), variant_to_string(&b));

        exit(1);
    }

    Variant result;

    result.type = a.type;
    if (a.type == TYPE_INT) {
        result.int_value = a.int_value / b.int_value;
    } else {

        CTACK_ERROR( "[DIVIDE OPERATOR]: Only intagers can divide!\n");
        exit(1);
    }
    return result;
}

bool variant_is_truthy(const Variant a){
    bool ret = false;

    switch (a.type){
        case TYPE_BOOL:
            ret = a.bool_value;
            break;
        case TYPE_STRING:
            ret = strlen(a.string_value) > 0;
            break;
        case TYPE_INT:
            ret = a.int_value > 0;
            break;
        case TYPE_NULL:
            ret = !a.null_value; // null is always falsy
            break;
        default: 
            CTACK_ERROR("[TRUTHY OPERATOR] Not implemented yet!\n");
            exit(1);
    }
    return ret;
}

bool variant_and(const Variant a, const Variant b){
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use && operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
    return variant_is_truthy(a) && variant_is_truthy(b); 
}
bool variant_or(const Variant a, const Variant b){
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use || with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }

    return variant_is_truthy(a) || variant_is_truthy(b); 
}
bool variant_gt(const Variant a, const Variant b){
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use > operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
    if(a.type == TYPE_INT){
        return a.int_value > b.int_value;
    } else if(a.type == TYPE_BOOL){
        return a.bool_value > b.bool_value;
    } else{
        CTACK_ERROR("[LOGIC OPERATOR] Can use > operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
}
bool variant_lt(const Variant a, const Variant b){
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use < operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
    if(a.type == TYPE_INT){
        return a.int_value < b.int_value;
    } else if(a.type == TYPE_BOOL){
        return a.bool_value < b.bool_value;
    } else{
        CTACK_ERROR("[LOGIC OPERATOR] Can use < operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
}

bool variant_gte(const Variant a, const Variant b){
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use >= operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
    if(a.type == TYPE_INT){
        return a.int_value >= b.int_value;
    } else if(a.type == TYPE_BOOL){
        return a.bool_value >= b.bool_value;
    } else{
        CTACK_ERROR("[LOGIC OPERATOR] Can use >= operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
}

bool variant_lte(const Variant a, const Variant b){
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use <= operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
    if(a.type == TYPE_INT){
        return a.int_value <= b.int_value;
    } else if(a.type == TYPE_BOOL){
        return a.bool_value <= b.bool_value;
    } else{
        CTACK_ERROR("[LOGIC OPERATOR] Can use <= operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }
}
bool variant_not(const Variant a){
    if(a.type != TYPE_STRING){
        CTACK_ERROR("[NOT OPERATOR] Can get oposite of %s!\n", variant_to_string(&a));
        exit(1);
    }

    return !variant_is_truthy(a);
}
bool variant_eq(const Variant a, const Variant b){
    if((a.type == TYPE_INT && b.type == TYPE_BOOL) || a.type == TYPE_BOOL && b.type == TYPE_INT){ // int can use like bool
        return variant_is_truthy(a) == variant_is_truthy(b);
    }
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use == operator with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }

    if(a.type == TYPE_STRING){
        return strcmp(a.string_value, b.string_value) == 0;
    }

    return a.string_value == b.string_value;
}
bool variant_not_eq(const Variant a, const Variant b){
    if((a.type == TYPE_INT && b.type == TYPE_BOOL) || a.type == TYPE_BOOL && b.type == TYPE_INT){ // int can use like bool
        return variant_is_truthy(a) != variant_is_truthy(b);
    }
    if(a.type != b.type){
        CTACK_ERROR("[LOGIC OPERATOR] Can not use != with %s and %s!\n", variant_to_string(&a), variant_to_string(&b));
        exit(1);
    }

    if(a.type == TYPE_STRING){
        return strcmp(a.string_value, b.string_value) != 0;
    }

    return a.string_value != b.string_value;
}

#endif // VARIANT_H