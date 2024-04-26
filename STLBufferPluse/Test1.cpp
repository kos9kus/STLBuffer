//
//  Test1.cpp
//  STLBufferPluse
//
//  Created by Kos on 26.04.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "Test1.h"

struct Block
{
    char* name;
    Block* next;
    Block* prev;
}typedef Block;

struct My_List
{
    Block* head;
    Block* tail;
}typedef Spisok;

struct Obj
{
    char* name;
    int free;
    int root;
    My_List* links;
}typedef Obj;

My_List* createList()
{
    My_List *list = (My_List*)malloc(sizeof(My_List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void deleteList(My_List *list)
{
    Block *head = list->head;
    Block *next = NULL;
    while (head)
    {
        next = head->next;
        free(head->name);
        head->name = NULL;
        free(head);
        head = next;
    }
    free(list);
}

void _add_block(My_List* list, const char* name)
{
    Block* item = (Block*)malloc(sizeof(Block));
    item->name = strdup(name);
    item->next = NULL;
    
    if (!list->head)
    {
        list->head = item;
        list->tail = item;
    } else {
        item->prev = list->tail;
        list->tail->next = item;
        list->tail = item;
    }
}


Obj **garbage_collector = NULL;
int MAX_NUM = 0;

int _cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int _is_free(const char *name)
{
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 && strcmp(garbage_collector[i]->name, name) == 0)
        {
            return 0;
        }
    }
    return 1;
}


int _include(My_List *list, char* name){
    Block* iter = list->head;
    while (iter != NULL){
        if (strcmp(iter->name, name) == 0){
            return 1;
        }
        iter = iter->next;
    }
    return 0;
}


int create(int size, int num_pages)
{
    if (garbage_collector == NULL && size > 0)
    {
        garbage_collector = (Obj**)malloc(sizeof(Obj*) * size);
        MAX_NUM = size;
        for (int i = 0; i < size; ++i)
        {
            garbage_collector[i] = (Obj*)malloc(sizeof(Obj));
            garbage_collector[i]->free = 1;
            garbage_collector[i]->root = 0;
            garbage_collector[i]->name = NULL;
            garbage_collector[i]->links = NULL;
        }
        return 1;
    }
    return 0;
}

int destroy()
{
    if (garbage_collector != NULL)
    {
        for (int i = 0; i < MAX_NUM; ++i)
        {
            if (garbage_collector[i]->links)
            {
                deleteList(garbage_collector[i]->links);
            }
            garbage_collector[i]->links = NULL;
            free(garbage_collector[i]->name);
            garbage_collector[i]->name = NULL;
            free(garbage_collector[i]);
            garbage_collector[i] = NULL;
        }
        free(garbage_collector);
        garbage_collector = NULL;
        return 1;
    }
    return 0;
}

int create_object(const char* name)
{
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 1 && _is_free(name) == 1)
        {
            garbage_collector[i]->name = strdup(name);
            garbage_collector[i]->links = createList();
            garbage_collector[i]->free = 0;
            return 1;
        }
    }
    return 0;
}

int destroy_object(const char *name)
{
    for (int i = 0; i < MAX_NUM; i++)
    {
        if (garbage_collector[i]->free == 0)
        {
            if (strcmp(name, garbage_collector[i]->name) == 0)
            {
                deleteList(garbage_collector[i]->links);
                free(garbage_collector[i]->name);
                garbage_collector[i]->name = NULL;
                garbage_collector[i]->free = 1;
                garbage_collector[i]->root = 0;
                return 1;
            }

//            if (garbage_collector[i]->links != NULL) ???
//            {
//                Block *iter = garbage_collector[i]->links->head;
//                while (iter)
//                {
//                    if (strcmp(iter->name, name)==0)
//                    {
//                        free(iter->name);
//                        iter->name = NULL;
//                    }
//                    iter = iter->next;
//                }
//            }
        }
    }
    return 0;
}

void _alphabet_sort(Obj **objects) {
    char *tmp;
    for(int i = 0; i < MAX_NUM; i++) 
    {
        for(int j = 0; objects[j]; j++) 
        {
            if(strcmp(objects[i]->name, objects[j]->name) < 0)
            {
                tmp = objects[i]->name;
                objects[i]->name = objects[j]->name;
                objects[j]->name = tmp;
            }
        }
    }
}

void print_objects()
{
    int num = 0;
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 )
        {
            num++;
        }
    }
    Obj **objects = (Obj**)malloc(sizeof(Obj *) * num);
    for (int i = 0, j = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0)
        {
            objects[j] = garbage_collector[i];
            j++;
        }
    }
    _alphabet_sort(objects);
    for (int i = 0; i < num; ++i)
    {
        printf("%s\n", objects[i]->name);
    }
    free(objects);
    printf("\n");
}

int set_root(const char* name)
{
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 && strcmp(name, garbage_collector[i]->name) == 0)
        {
            garbage_collector[i]->root = 1;
            return 1;
        }
    }
    return 0;
}

int link(const char* object1_name, const char* object2_name)
{
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (_is_free(object1_name) == 0  && _is_free(object2_name) == 0)
        {
            _add_block(garbage_collector[i]->links, object2_name);
            return 1;
        }
    }
    return 0;
}

void collect_live_objects(void)
{
    My_List *list = createList();
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 && garbage_collector[i]->root)
        {
            _add_block(list, garbage_collector[i]->name);
            Block* item = garbage_collector[i]->links->head;
            while (item != NULL)
            {
                if (item->name != NULL)
                {
                    _add_block(list, item->name);
                    for (int j = 0; j < MAX_NUM; ++j)
                    {
                        if (garbage_collector[j]->free == 0 && strcmp(item->name, garbage_collector[j]->name) == 0)
                        {
                            Block* iter = garbage_collector[j]->links->head;
                            while (iter!=0)
                            {
                                if(iter->name != NULL && _include(list, iter->name) == 1)
                                {
                                    _add_block(list, iter->name);
                                }
                                iter = iter->next;
                            }
                            
                        }
                    }
                }
                item = item->next;
            }
        }
    }
    

    int size = 0;
    Block* head = list->head;
    while (head != NULL)
    {
        size += 1;
        head = head->next;
    }

    char** arr = (char**)malloc(sizeof(char*) * size);
    Block* item = list->head;
    for (int i = 0; i < size; ++i)
    {
        arr[i] = item->name;
        item = item->next;
    }
    qsort(arr, size, sizeof(arr[0]), _cmp);
    for (int i = 0; i != size; ++i)
    {
        printf("%s\n", arr[i]);
    }
    free(arr);
    arr = NULL;
    
    deleteList(list);
    printf("\n");
}


void setup_memory_manager(memory_manager_t* mm)
{
    mm->create = create;
    mm->destroy = destroy;
    mm->create_object = create_object;
    mm->destroy_object = destroy_object;
    mm->print_objects = print_objects;
    mm->set_root = set_root;
    mm->link = link;
    mm->collect_live_objects = collect_live_objects;
}
