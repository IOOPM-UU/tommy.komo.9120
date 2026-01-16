#include "linked_list.h"
#include "iterator.h"
#include "common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>

struct link
{
    elem_t value;
    link_t *next;
};

struct list
{
    link_t *head;
    size_t size;
    ioopm_eq_function *eq_fun;
};


struct list_iterator
{
    ioopm_list_t *list;
    struct link *current;
};

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    assert(list);
    ioopm_list_iterator_t *iter = calloc(1, sizeof(ioopm_list_iterator_t));
    iter->list = list;
    iter->current = list->head;
    return iter;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    assert(iter);
    return (iter->current != NULL);
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    assert(iter->current != NULL);

    elem_t value = iter->current->value;
    iter->current = iter->current->next;   
    return value; 
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    assert(iter);
    iter->current = iter->list->head;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    assert(iter);
    if (iter->current == NULL)
    {
        assert(false);
    }
    return iter->current->value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}