#ifndef VARIABLE_H
#define VARIABLE_H

#include "variant.h"
#include "common.h"
#include "da.h"

typedef struct{
    char key[VARIABLE_NAME_SIZE];
    Variant value;
} Variable;

// We use dynamic array, not linked list for support full CPU cache
typedef struct {
    Variable* items;
    size_t count;
    size_t capacity;
}VariableHashTableNextList;

typedef struct{
    Variable variable;
    VariableHashTableNextList nexts;
} VariableHashTableNode;


typedef struct{
    VariableHashTableNode *items;
    size_t count;
    size_t capacity;
} VariableHashTable;

void variable_hashtable_free(VariableHashTable* table){
    for(size_t i=0; i< table->count;i++){
        VariableHashTableNode *node = table->items;
        if(node->variable.value.type == TYPE_STRING){
            free(node->variable.value.string_value);
        }

        while(node->nexts.count>0){
        Variable var = DA_POP(node->nexts);
            if(var.value.type == TYPE_STRING){
                free(var.value.string_value);
            }
        }
        free(node->nexts.items);
    }
    free(table->items);
}

VariableHashTable variable_create_hashtable(size_t size){
    VariableHashTable table = {0};
    DA_INIT(table, size);
    memset(table.items, 0, sizeof(VariableHashTableNode)*size);
    return table;
}

size_t variable_hash(const char* key){
    size_t hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

size_t variable_hashtable_add(VariableHashTable *table, const char* key, Variant value){
    if(table->capacity < table->count +1 ){
        // realloc table then rehash all elements
        VariableHashTable temp = variable_create_hashtable(table->capacity *2);

        while(table->count>0){
            VariableHashTableNode itr=DA_POP(*table);

            if(itr.variable.key[0]!='\0'){ // reassign value
                variable_hashtable_add(&temp, itr.variable.key, itr.variable.value);

                for(size_t j=0; j<itr.nexts.count; j++){ // reassign nexts
                    variable_hashtable_add(&temp, itr.nexts.items[j].key, itr.nexts.items[j].value);
                }
            }
        }
        variable_hashtable_free(table);
        memcpy(table, &temp, sizeof(VariableHashTable));
    }

    size_t hash = variable_hash(key) % table->capacity;
    Variable var = (Variable){
                .key = {0},
                .value = value
    };
    strcpy(var.key, key);

    if(table->items[hash].variable.key[0]=='\0'){ // if node is avaible
        table->items[hash] = (VariableHashTableNode){
            .variable = var,
            .nexts = {0}
        };
        table->count++;
    } else{
        DA_PUSH(table->items[hash].nexts, var);
    }
    return hash;
}

Variable* variable_hashtable_get(const VariableHashTable *table, const char* key){
    size_t hash = variable_hash(key) % table->capacity;
    VariableHashTableNode *root_node = &table->items[hash];
    if(strcmp(root_node->variable.key, key) == 0){
        return &root_node->variable;
    }

    size_t index =0;
    while(index < root_node->nexts.count){
        Variable* var = &DA_GET(root_node->nexts, index);
        if (strcmp(var->key, key) == 0) {
            return var;
        }
        index++;
    }
    CTACK_ERROR("Variable could not found! Key: '%s'\n", key);
    exit(1);
}


void variable_hashtable_delete(VariableHashTable *table, const char* key){
    Variable *var = variable_hashtable_get(table, key);
    memset(var, 0, sizeof(Variable));
}

void variable_hashtable_set(VariableHashTable *table, const char* key, const Variant value){
    Variable* var = variable_hashtable_get(table, key);
    memcpy(&var->value, &value, sizeof(value));
}

#endif // VARIABLE_H