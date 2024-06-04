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

// Variant variant_create(VariantType type, void* data){
//     Variant (.variant_lookup_table[4])(void*) = {
//         [TYPE_NULL] = &variant_create_null,
//         [TYPTYPE_INT] = &variant_create_int,
//         [TYPETYPE_BOOL] = &variant_create_bool,
//         [TYPE_NTYPE_STRING] = &variant_create_string,
//     };

    
// }

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
        .type = TYPE_BOOL,
        .null_value = d,
    };
}

Variant variant_add(Variant a, Variant b){
    if(a.type != b.type) {
        CTACK_ERROR( "[PLUS OPERATOR]: Can not add diffirint data types\n");
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
        CTACK_ERROR( "[PLUS OPERATOR]: Can not subscruct diffirint data types\n");
        exit(1);
    }

    Variant result;

    result.type = a.type;
    if (a.type == TYPE_INT) {
        result.int_value = a.int_value - b.int_value;
    } else if (a.type == TYPE_STRING) {
        CTACK_ERROR( "[SUBS OPERATOR]: Can not subscruct diffirint data types\n");
        exit(1);
    } else if (a.type == TYPE_BOOL) {
        int r = a.int_value - b.int_value;
        if(r<0){
            r =0;
        }
        result.bool_value = r;
    }   else {
        CTACK_ERROR( "[SUBS OPERATOR]: This type not implemented yet! Type number: %ul\n", a.type);
        exit(1);
    }

    return result;
}

Variant variant_mult(Variant a, Variant b){
    if(a.type != b.type) {
        CTACK_ERROR( "[MULTIPLY OPERATOR]: Can not subscruct diffirint data types\n");
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
        CTACK_ERROR( "[PLUS OPERATOR]: Can not subscruct diffirint data types\n");
        exit(1);
    }

    Variant result;

    result.type = a.type;
    if (a.type == TYPE_INT) {
        result.int_value = a.int_value / b.int_value;
    } else {
        CTACK_ERROR( "[MULTIPLY OPERATOR]: Only intagers can divide!\n");
        exit(1);
    }
    return result;
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

#endif // VARIANT_H