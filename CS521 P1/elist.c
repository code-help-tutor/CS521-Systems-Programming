WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "logger.h"

#include "elist.h"

#define DEFAULT_INIT_SZ 10
#define RESIZE_MULTIPLIER 2

struct elist {
    size_t capacity;         /*!< Storage space allocated for list items */
    size_t size;             /*!< The actual number of items in the list */
    size_t item_sz;          /*!< Size of the items stored in the list */
    void *element_storage;   /*!< Pointer to the beginning of the array */
};

struct elist* elist_create(size_t list_sz, size_t item_sz)
{
    struct elist* list = malloc(sizeof(struct elist));
    if (list == NULL)
        return NULL;

    if (list_sz == 0)
    {
        list_sz = DEFAULT_INIT_SZ;

    }
    list->capacity = list_sz;
    list->item_sz = item_sz;
    list->size = 0;
    size_t storage_bytes = list->capacity * list->item_sz;
    list->element_storage = malloc(storage_bytes);
    if (list->element_storage == NULL)
    {
        free(list->element_storage);
	free(list);
        return NULL;
    }
    return list;
}

void elist_destroy(struct elist* list)
{
    free(list->element_storage);
    free(list);
}

int elist_set_capacity(struct elist* list, size_t capacity)
{
    if (list == NULL)
        return -1;
    if(list->size > capacity){
    	list->size = capacity;
    }
    list->capacity = capacity;
    list->element_storage = realloc(list->element_storage, capacity* list->item_sz);
    if(list->element_storage == NULL){
	    free(list->element_storage);
	    return -1;
    }
    return 0;
}

size_t elist_capacity(struct elist* list)
{
    return list->capacity;

}

ssize_t elist_add(struct elist* list, void* item)
{
    if (list->size == list->capacity)
    {
        list->capacity = list->capacity * RESIZE_MULTIPLIER;
        void* temp = realloc(list->element_storage, list->item_sz * list->capacity);
        if (temp == NULL)
        {
	    free(temp);
            return -1;
        }
	else{
	    list->element_storage = temp;
	}
    }
    size_t idx = list->size++;
    void* item_ptr = list->element_storage + idx * list->item_sz;
    memcpy(item_ptr, item, list->item_sz);
    return idx;
}

void* elist_add_new(struct elist* list)
{
    if (list->size == list->capacity)
    {
        list->capacity = list->capacity * RESIZE_MULTIPLIER;
        list->element_storage = realloc(list->element_storage, list->item_sz * list->capacity);
            }
    void* temp = list->element_storage + list->item_sz * list->size;
    list->size++;
   // void* newlist = list->item_sz + storage_bytes;
    return temp;
}

int elist_set(struct elist* list, size_t idx, void* item)
{
    if (list == NULL)
        return -1;
    if (idx >= list->size)
        return -1;
    void* item_ptr= list->element_storage+ idx * list->item_sz;
    memcpy(item_ptr, item, list->item_sz);

    return 0;
}

void* elist_get(struct elist* list, size_t idx)
{
    if (list == NULL)
        return NULL;
    if (idx >= list->size)
        return NULL;
    void* item_ptr= list->element_storage+ idx * list->item_sz;
    return item_ptr;

}

size_t elist_size(struct elist* list)
{
    if (list == NULL)
        return 0;
    size_t size = list->size;

    return size;
}

int elist_remove(struct elist* list, size_t idx)
{
    if (list == NULL)
        return -1;
    if (idx > list->size)
        return -1;
    if (list->size == 0)
        return -1;

    if(idx == 0){
    	list->element_storage = list->element_storage + list->item_sz;
	list->size --;
	return 0;
    }
    if(idx == list->size -1){
    	memset(list->element_storage + (list->size -1) * list->item_sz, 0, list->item_sz);
	list->size --;
	return 0;
    }
    
    for (int i = idx; i < list->size -1; i++) {
	
        void* item_ptr = list->element_storage + i * list->item_sz;
        void* item_ptr1 = list->element_storage + (i+1)*list->item_sz;
        memmove(item_ptr, item_ptr1, list->item_sz);
    }  
    list->size = list->size - 1;
    return 0;
}

void elist_clear(struct elist* list)
{
    if (list == NULL)
        return;

    list->size = 0;
    //free(list->element_storage);
}

void elist_clear_mem(struct elist* list)
{

    if (list == NULL)
        return;

    memset( list->element_storage, 0, list->size * list->item_sz);
    list->size = 0;
}

ssize_t elist_index_of(struct elist* list, void* item)
{
    if (list == NULL)
        return -1;
    int * cur = (int *)list->element_storage;
    for (int i = 0; i < list->size; i++)
    {
        if (cur[i] == *(int*)item)
            return i;
    }
    return -1;
}

void elist_sort(struct elist* list, int (*comparator)(const void*, const void*))
{
    if (list == NULL)
        return;
    if (list->size <= 0)
        return;
    qsort(list->element_storage, list->size, list->item_sz, comparator);
    
}

bool idx_is_valid(struct elist* list, size_t idx)
{
    if (list == NULL)
        return false;
    if (idx > list->size && idx <= 0)
        return false;

    return true;
}

