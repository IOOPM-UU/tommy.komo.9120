// common.h
#pragma once
#include <stdbool.h>
#include <stddef.h>


typedef union {
    int    i;
    unsigned u;
    long   l;
    double f;
    void  *p;    
    bool   b;
} elem_t;

typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef size_t ioopm_hash_function(elem_t key);

typedef bool ioopm_predicate(elem_t key, elem_t value, void *extra);
typedef void ioopm_apply_function(elem_t key, elem_t *value, void *extra);

typedef bool ioopm_elem_predicate(elem_t value, void *extra);
typedef void ioopm_apply_elem_function(elem_t *value, void *extra);

static inline size_t ioopm_extract_int_hash(elem_t key)
{
    return (size_t) key.i;
}

#define int_elem(x)   (elem_t){ .i = (x) }
#define uint_elem(x)  (elem_t){ .u = (x) }
#define long_elem(x)  (elem_t){ .l = (x) }
#define dbl_elem(x)   (elem_t){ .f = (x) }
#define ptr_elem(x)   (elem_t){ .p = (x) }
#define bool_elem(x)  (elem_t){ .b = (x) }