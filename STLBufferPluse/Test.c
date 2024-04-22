//
//  Test.c
//  STLBufferPluse
//
//  Created by Kos on 23.04.2024.
//

#include <string.h>
#include <stdlib.h>
#include "Test.h"

typedef struct Block Block;

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
}typedef My_List;

struct Obj
{
    char* name;
    int free;
    int root;
    My_List* links;
}typedef Obj;

Obj** garbage_collector = NULL;
int gb_size = 0;

My_List* create_list()
{
    My_List* list = (My_List*)malloc(sizeof(My_List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void delete_list(My_List *list)
{
    if (list == NULL) {
        return;
    }
    Block *head = list->head;
    while (head)
    {
        Block *next = head->next;
        free(head->name);
        head->name = NULL;
        head->next = NULL;
        head->prev = NULL;
        free(head);
        head = next;
    }
    free(list);
    list = NULL;
}

void Add_Block(My_List* list, const char* name)
{
    Block* item = (Block*)malloc(sizeof(Block));
    item->name = strdup(name);
    item->next = NULL;
    item->prev = list->tail;
    if (list->tail)
    {
        list->tail->next = item;
    }
    list->tail = item;
    if (!list->head)
    {
        list->head = item;
    }
}

int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int duplicate(const char *name)
{
    for (int i = 0; i < gb_size; ++i)
    {
        if (garbage_collector[i]->free == 0 && strcmp(garbage_collector[i]->name, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}


int include(My_List* list, char* name){
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
        garbage_collector = (Obj**)malloc(size * sizeof(Obj*));
        if (garbage_collector == NULL) {
            return 0;
        }
        gb_size = size;
        for (int i = 0; i < size; ++i)
        {
            Obj* obj = (Obj*)malloc(sizeof(Obj));
            if (obj == NULL) {
                return 0;
            }
            obj->free = 1;
            obj->root = 0;
            obj->name = NULL;
            obj->links = NULL;
            garbage_collector[i] = obj;
        }
        return 1;
    }
    return 0;
}

int destroy()
{
    if (garbage_collector != NULL)
    {
        for (int i = 0; i < gb_size; ++i)
        {
            Obj* obj = garbage_collector[i];
            garbage_collector[i] = NULL;
            delete_list(obj->links);
            obj->links = NULL;
            free(obj->name);
            obj->name = NULL;
            free(obj);
        }
        free(garbage_collector);
        garbage_collector = NULL;
        gb_size = 0;
        return 1;
    }
    return 0;
}

int create_object(const char *name)
{
    int max_size = 32;
    if (strlen(name) > max_size) 
    {
        return 0;
    }
    for (int i = 0; i < gb_size; ++i)
    {
        if (garbage_collector[i]->free == 1 && duplicate(name) == 0)
        {
            garbage_collector[i]->name = strdup(name);
            garbage_collector[i]->links = create_list();
            garbage_collector[i]->free = 0;
            return 1;
        }
    }
    return 0;
}

int destroy_object(const char *name)
{
    for (int i = 0; i < gb_size; i++)
    {
        if (garbage_collector[i]->free == 0)
        {
            if (strcmp(name, garbage_collector[i]->name) == 0)
            {
                delete_list(garbage_collector[i]->links);
                free(garbage_collector[i]->name);
                garbage_collector[i]->name = NULL;
                garbage_collector[i]->free = 1;
                return 1;
            }
                // надо ли это делать ??? 
//            if (garbage_collector[i]->links != NULL)
//            {
//                Block *head = garbage_collector[i]->links->head;
//                while (head)
//                {
//                    Block *next = head->next;
//                    if (strcmp(head->name, name) == 0)
//                    {
//                        free(head->name);
//                        head->name = NULL;
//                    }
//                    head = next;
//                }
//            }
        }
    }
    return 0;
}

void print_objects()
{
    int num = 0;
    for (int i = 0; i < gb_size; ++i)
    {
        if (garbage_collector[i]->free == 0 )
        {
            num++;
        }
    }
    Obj **arr = (Obj**)malloc(sizeof(Obj *) * num);
    int j = 0;
    for (int i = 0; i < gb_size; i++)
    {
        if (garbage_collector[i]->free == 0)
        {
            arr[j] = garbage_collector[i];
            j++;
        }
    }
    Obj* t;
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num - 1; j++)
        {
            if (strcmp(arr[j]->name, arr[j + 1]->name) == 1)
            {
                t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }

    

    for (int i = 0; i < num; ++i)
    {
        printf("%s\n", arr[i]->name);
    }
    free(arr);
    printf("\n");
}

int set_root(const char* name)
{
    for (int i = 0; i < gb_size; ++i)
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
//    for (int i = 0; i < gb_size; ++i)
//    {
//        if (garbage_collector[i]->free == 0 && strcmp(object1_name, garbage_collector[i]->name) == 0 && duplicat(strdup(object2_name)) != 0)
//        {
//            Add_Block(garbage_collector[i]->links, object2_name);
//            return 1;
//        }
//    }
    return 0;
}

void collect_live_objects(void)
{
    My_List* list = create_list();
    for (int i = 0; i < gb_size; ++i)
    {
        if (garbage_collector[i]->free == 0 && garbage_collector[i]->root)
        {
            Add_Block(list, garbage_collector[i]->name);
            Block* item = garbage_collector[i]->links->head;
            while (item != NULL)
            {
                if (item->name != NULL)
                {
                    Add_Block(list, item->name);
                    for (int j = 0; j < gb_size; ++j)
                    {
                        if (garbage_collector[j]->free == 0 && strcmp(item->name, garbage_collector[j]->name) == 0)
                        {
                            Block* iter = garbage_collector[j]->links->head;
                            while (iter!=0)
                            {
                                if(iter->name != NULL && include(list, iter->name) == 1)
                                {
                                    Add_Block(list, iter->name);
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
    qsort(arr, size, sizeof(arr[0]), cmp);
    for (int i = 0; i != size; ++i)
    {
        printf("%s\n", arr[i]);
    }
    free(arr);
    arr = NULL;
    
//    delete_list(&list);
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

