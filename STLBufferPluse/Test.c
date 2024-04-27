//
//  Test.cpp
//  STLBufferPluse
//
//  Created by Kos on 26.04.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "Test.h"

typedef struct Block Block;
typedef struct My_List My_List;

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

My_List *_create_list()
{
    My_List *list = (My_List*)malloc(sizeof(My_List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void _delete_list(My_List *list)
{
    Block *head = list->head;
    Block *next = NULL;
    while (head)
    {
        next = head->next;
        free(head->name);
        head->name = NULL;
        head->prev = NULL;
        head->next = NULL;
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
    item->prev = NULL;
    
    if (list->head == NULL)
    {
        list->head = item;
        list->tail = item;
    }
    else
    {
        item->prev = list->tail;
        list->tail->next = item;
        list->tail = item;
    }
}

Obj **garbage_collector = NULL;
int MAX_NUM = 0;

Obj* _get_existed_object(const char *name)
{
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 && strcmp(garbage_collector[i]->name, name) == 0)
        {
            return garbage_collector[i];
        }
    }
    return NULL;
}


int _include(const My_List *list, const char* name){
    Block* block = list->head;
    while (block)
    {
        if (strcmp(block->name, name) == 0)
        {
            return 1;
        }
        block = block->next;
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
    if (garbage_collector)
    {
        for (int i = 0; i < MAX_NUM; ++i)
        {
            if (garbage_collector[i]->links)
            {
                _delete_list(garbage_collector[i]->links);
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
    int max_size = 32;
    if (strlen(name) > max_size)
    {
        return 0;
    }
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 1 && _get_existed_object(name) == NULL)
        {
            garbage_collector[i]->name = strdup(name);
            garbage_collector[i]->links = _create_list();
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
                _delete_list(garbage_collector[i]->links);
                free(garbage_collector[i]->name);
                garbage_collector[i]->name = NULL;
                garbage_collector[i]->free = 1;
                garbage_collector[i]->root = 0;
                garbage_collector[i]->links = NULL;
                return 1;
            }

            if (garbage_collector[i]->links != NULL)
            {
                Block *block = garbage_collector[i]->links->head;
                while (block)
                {
                    if (block->name) {
                        if (strcmp(block->name, name) == 0)
                        {
                            free(block->name);
                            block->name = NULL;
                        }
                    }
                    block = block->next;
                }
            }
        }
    }
    return 0;
}

void _alphabet_sort(char **objects, int size) {
    char *tmp;
    
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size - 1; j++)
        {
            if(strcmp(objects[i], objects[j]) < 0)
            {
                tmp = objects[i];
                objects[i] = objects[j];
                objects[j] = tmp;
            }
        }
    }
}

void print_objects()
{
    int size = 0;
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 )
        {
            ++size;
        }
    }
    char** names = (char**)malloc(sizeof(char*) * size);
    for (int i = 0, j = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0)
        {
            names[j] = garbage_collector[i]->name;
            j++;
        }
    }
    
    _alphabet_sort(names, size);
    
    for (int i = 0; i < size; ++i)
    {
        printf("%s\n", names[i]);
    }
    free(names);
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
    int max_links_size = 16;
    for (int i = 0; i < MAX_NUM; ++i)
    {
        Obj *object1 = _get_existed_object(object1_name);
        Obj *object2 = _get_existed_object(object2_name);
        if (object1 != NULL  && object2 != NULL)
        {
            int size = 0;
            Block *head = object1->links->head;
            while (head)
            {
                ++size;
                head = head->next;
            }
            if (size > max_links_size)
            {
                return 0;
            }
            else
            {
                _add_block(object1->links, object2_name);
                return 1;
            }
        }
    }
    return 0;
}

void _collect_live_objects_recursive(My_List *list, Block *head) {
    while (head)
    {
        if (head->name != NULL && _include(list, head->name) == 0)
        {
            _add_block(list, head->name);
            Obj *object = _get_existed_object(head->name);
            _collect_live_objects_recursive(list, object->links->head);
        }
        head = head->next;
    }
}

void collect_live_objects(void)
{
    My_List *list = _create_list();
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 && garbage_collector[i]->root == 1)
        {
            Obj *live_object = garbage_collector[i];
            _add_block(list, live_object->name);
            _collect_live_objects_recursive(list, live_object->links->head);
        }
    }
    
    int size = 0;
    Block* head = list->head;
    while (head)
    {
        ++size;
        head = head->next;
    }

    char** names = (char**)malloc(sizeof(char*) * size);
    Block* item = list->head;
    for (int i = 0; i < size; ++i)
    {
        names[i] = item->name;
        item = item->next;
    }
    
    _alphabet_sort(names, size);
    
    for (int i = 0; i != size; ++i)
    {
        printf("%s\n", names[i]);
    }
    free(names);
    names = NULL;
    
    _delete_list(list);
    list = NULL;
    
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
