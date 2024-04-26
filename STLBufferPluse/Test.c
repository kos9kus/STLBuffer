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
    
    if (!list->head)
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
    if (garbage_collector != NULL)
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
        if (garbage_collector[i]->free == 1 && _is_free(name) == 1)
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

void _alphabet_sort(char **objects, int size) {
    char *tmp;
    
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; objects[j]; j++)
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
    int max_links_size = 32;
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (_is_free(object1_name) == 0  && _is_free(object2_name) == 0)
        {
            int size = 0;
            Block *head = garbage_collector[i]->links->head;
            while (head)
            {
                ++size;
                head = head->next;
            }
                
            if (size > 32)
            {
                return 0;
            }
            else
            {
                _add_block(garbage_collector[i]->links, object2_name);
                return 1;
            }
        }
    }
    return 0;
}

void collect_live_objects(void)
{
    My_List *list = _create_list();
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 && garbage_collector[i]->root)
        {
            _add_block(list, garbage_collector[i]->name);
            Block* item = garbage_collector[i]->links->head;
            while (item)
            {
                _add_block(list, item->name);
                
                for (int j = 0; j < MAX_NUM; ++j)
                {
                    if (garbage_collector[j]->free == 0 && strcmp(item->name, garbage_collector[j]->name) == 0)
                    {
                        Block* iter = garbage_collector[j]->links->head;
                        while (iter)
                        {
                            if(_include(list, iter->name))
                            {
                                _add_block(list, iter->name);
                            }
                            iter = iter->next;
                        }
                        
                    }
                }
                item = item->next;
            }
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
