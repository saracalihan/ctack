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
Variant variant_create_string(const char* d){
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
        CTACK_ERROR(stderr, "[PLUS OPERATOR]: Can not add diffirint data types\n");
        exit(1);
    }

    Variant result;

    if (a.type == TYPE_INT) {
        result.type = TYPE_INT;
        result.intValue = a.intValue + b.intValue;
    } else if (a.type == TYPE_STRING) {
        result.type = TYPE_STRING;
        result.stringValue = (char *)malloc(strlen(a.stringValue) + strlen(b.stringValue) + 1);
        strcpy(result.stringValue, a.stringValue);
        strcat(result.stringValue, b.stringValue);
    } else if (a.type == TYPE_BOOL) {
        result.type = TYPE_STRING;
        result.stringValue = (char *)malloc(strlen(a.stringValue) + strlen(b.stringValue) + 1);
        strcpy(result.stringValue, a.stringValue);
        strcat(result.stringValue, b.stringValue);
    }   else {
        CTACK_ERROR(stderr, "[PLUS OPERATOR]: This type not implemented yet! Type number: %ul\n", a.type);
        exit(1);
    }

    return result;
}

#endif // VARIANT_H