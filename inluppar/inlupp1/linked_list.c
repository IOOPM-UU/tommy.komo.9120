#include <CUnit/Basic.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "linked_list.h"
#include "hash_table.h"
#include <stddef.h>

typedef struct link link_t;



struct link
{
    int value;
    link_t *next;
};

struct list 
{
    link_t *head;
    link_t *tail;
    size_t size;
};


static link_t *link_create(link_t *next, int value)
{
    link_t *link = calloc(1,sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}

ioopm_list_t *ioopm_linked_list_create()
{
    return calloc(1, sizeof(ioopm_list_t));
};

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    assert(list);
    
    link_t *cursor = list->head;
    while(cursor != NULL)
    {
        link_t *tmp = cursor;
        cursor = cursor->next;
        free(tmp);
    }
    free(list);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, int value)
{
    assert(list);

    link_t *new = link_create(list->head,value); // skapar ny länk 
    list->head = new;

    if(list->tail == NULL)
    {
        list->tail = new;
    }

    list->size++;
}


void ioopm_linked_list_append(ioopm_list_t *list, int value)
{
    assert(list);

    link_t *new = link_create(NULL, value);
    
    if(list->head == NULL)
    {
        list->head = new;
        list->tail = new;
    }
    else
    {
        list->tail->next = new;
        list->tail = new;  
    }
    list->size++;  

}

void ioopm_linked_list_insert(ioopm_list_t *list, size_t index, int value)
{
    assert(list);
    assert(index >= 0 && index <= list->size);

    if(index == 0)
    {
        ioopm_linked_list_prepend(list, value);
        return;
    }
    if(index == list->size)
    {
        ioopm_linked_list_append(list, value);
        return;
    }
    
    
    link_t *cursor = list->head;
    for(size_t i = 1; i < index;i++)
    {
        cursor = cursor->next;
    }
    link_t *nextfornewlink = cursor->next;
    cursor->next = link_create(nextfornewlink, value);
    list->size++;
}   

int ioopm_linked_list_remove(ioopm_list_t *list, size_t index)
{
    assert(list);
    assert(index >= 0 && index < list->size);

    if(index == 0){
        link_t *removed = list->head;
        int value_removed = removed->value;
        
        list->head = removed->next;
        if (list->head == NULL) {
            list->tail = NULL;
        }
        free(removed);
        list->size--;      
        return value_removed;
    }

    link_t *cursor = list->head;
    for(size_t i = 1; i < index;i++){  //stanna på linken innan
        cursor = cursor->next;
    }


    int value_removed = cursor->next->value;
    link_t *link_removed = cursor->next;
    cursor->next = cursor->next->next;
    
    if (link_removed == list->tail) {           
        list->tail = cursor;
    }

    free(link_removed);
    list->size--;
    return value_removed;

}


int ioopm_linked_list_get(ioopm_list_t *list, size_t index){
    
    assert(list);
    assert(index >= 0 && index < list->size);

    link_t *cursor = list->head;
    for(size_t i = 0; i < index;i++){
        cursor = cursor->next;
    }
    return cursor->value;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, int element){
    
    assert(list);
    
    link_t *cursor = list->head;
    while (cursor != NULL)
    {
        if (cursor->value == element)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}


size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    assert(list);
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{ 
    assert(list);
    return list->size == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list){
    assert(list);

    while(list->head != NULL){
        link_t *to_remove = list->head;
        list->head = list->head->next;
        free(to_remove);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}


bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate *prop, void *extra){
    
    assert(list);
    assert(prop);

    link_t *cursor = list->head;
    
    while(cursor != NULL){
        if(!prop(cursor->value, extra)){
            return false;
        }
        cursor = cursor->next;
    }

    return true; 
}

bool ioopm_linked_list_any(ioopm_list_t* list, ioopm_int_predicate *prop, void *extra){
    assert(list);
    assert(prop);

    link_t *cursor = list->head;

    while(cursor != NULL){
        if(prop(cursor->value, extra)){
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_int_function *fun, void *extra){
    assert(list);
    assert(fun);

    link_t *cursor = list->head;
    while(cursor != NULL){
        fun(&cursor->value, extra);
        cursor = cursor->next;
    }
}


