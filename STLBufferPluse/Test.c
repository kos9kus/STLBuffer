//
//  Test.c
//  STLBufferPluse
//
//  Created by Kos on 23.04.2024.
//

#include <string.h>
#include <stdlib.h>
#include "Test.h"

typedef struct Object Object;

struct Object
{
    char* name;
    int is_live;
    int is_root;
    Object* next;
    Object* prev;
    
    Object* head; // ???
    Object* tail; // ???
}typedef Object;

Object** garbage_collector = NULL;
int gb_size = 0;

int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int duplicate(const char *name)
{
    for (int i = 0; i < gb_size; ++i)
    {
        Object *object = garbage_collector[i];
        if (object != NULL && object->is_live == 1 && strcmp(object->name, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int create(int size, int num_pages)
{
    if (garbage_collector == NULL && size > 0)
    {
        garbage_collector = (Object**)malloc(size * sizeof(Object*));
        if (garbage_collector == NULL) {
            return 0;
        }
        gb_size = size;
        for (int i = 0; i < size; ++i)
        {
            garbage_collector[i] = NULL;
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
            Object* object = garbage_collector[i];
            garbage_collector[i] = NULL;
            if (object) {
                free(object->name);
                free(object);
            }
        }
        free(garbage_collector);
        garbage_collector = NULL;
        gb_size = 0;
        return 1;
    }
    return 0;
}

Object* create_empty_object(const char *name)
{
    Object *object = (Object*)malloc(sizeof(Object));
    object->name = strdup(name);
    
    object->is_live = 0;
    object->is_root = 0;
    
    object->next = NULL;
    object->prev = NULL;
    
    object->head = NULL;
    object->tail = NULL;
    
    return object;
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
        Object *object = garbage_collector[i];
        if (object != NULL && strcmp(object->name, name) == 0) 
        {
            return 0;
        }
        if (object == NULL)
        {
            Object *object = create_empty_object(name);
            garbage_collector[i] = object;
            return 1;
        }
    }
    return 0;
}

int destroy_object(const char *name)
{
    for (int i = 0; i < gb_size; i++)
    {
        Object *object = garbage_collector[i];
        if ((object != NULL) && strcmp(name, object->name) == 0)
        {
            free(object->name);
            free(object);
            garbage_collector[i] = 0;
            return 1;
        }
    }
    return 0;
}

void print_objects()
{
//    int num = 0;
//    for (int i = 0; i < gb_size; ++i)
//    {
//        if (garbage_collector[i]->free == 0 )
//        {
//            num++;
//        }
//    }
//    Object **arr = (Object**)malloc(sizeof(Object *) * num);
//    int j = 0;
//    for (int i = 0; i < gb_size; i++)
//    {
//        if (garbage_collector[i]->free == 0)
//        {
//            arr[j] = garbage_collector[i];
//            j++;
//        }
//    }
//    Object* t;
//    for (int i = 0; i < num; i++)
//    {
//        for (int j = 0; j < num - 1; j++)
//        {
//            if (strcmp(arr[j]->name, arr[j + 1]->name) == 1)
//            {
//                t = arr[j];
//                arr[j] = arr[j + 1];
//                arr[j + 1] = t;
//            }
//        }
//    }
//    for (int i = 0; i < num; ++i)
//    {
//        printf("%s\n", arr[i]->name);
//    }
//    free(arr);
//    printf("\n");
}

int set_root(const char* name)
{
//    for (int i = 0; i < gb_size; ++i)
//    {
//        if (garbage_collector[i]->free == 0 && strcmp(name, garbage_collector[i]->name) == 0)
//        {
//            garbage_collector[i]->root = 1;
//            return 1;
//        }
//    }
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
//    My_List* list = create_list();
//    for (int i = 0; i < gb_size; ++i)
//    {
//        if (garbage_collector[i]->free == 0 && garbage_collector[i]->root)
//        {
//            Add_Block(list, garbage_collector[i]->name);
//            Block* item = garbage_collector[i]->links->head;
//            while (item != NULL)
//            {
//                if (item->name != NULL)
//                {
//                    Add_Block(list, item->name);
//                    for (int j = 0; j < gb_size; ++j)
//                    {
//                        if (garbage_collector[j]->free == 0 && strcmp(item->name, garbage_collector[j]->name) == 0)
//                        {
//                            Block* iter = garbage_collector[j]->links->head;
//                            while (iter!=0)
//                            {
//                                if(iter->name != NULL && include(list, iter->name) == 1)
//                                {
//                                    Add_Block(list, iter->name);
//                                }
//                                iter = iter->next;
//                            }
//                            
//                        }
//                    }
//                }
//                item = item->next;
//            }
//        }
//    }
//    
//
//    int size = 0;
//    Block* head = list->head;
//    while (head != NULL)
//    {
//        size += 1;
//        head = head->next;
//    }
//
//    char** arr = (char**)malloc(sizeof(char*) * size);
//    Block* item = list->head;
//    for (int i = 0; i < size; ++i)
//    {
//        arr[i] = item->name;
//        item = item->next;
//    }
//    qsort(arr, size, sizeof(arr[0]), cmp);
//    for (int i = 0; i != size; ++i)
//    {
//        printf("%s\n", arr[i]);
//    }
//    free(arr);
//    arr = NULL;
//    
////    delete_list(&list);
//    printf("\n");
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

