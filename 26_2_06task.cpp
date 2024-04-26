#include <stdlib.h>
#include <string.h>
#include <memory.h>

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
    My_List* list = (My_List*)malloc(sizeof(My_List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void deleteList(My_List** list)
{
    Block *head = (*list)->head;
    Block* next = NULL;
    while (head)
    {
        next = head->next;
        free(head->name);
        head->name = NULL;
        free(head);
        head = next;
    }
    free(*list);
    *list = NULL;
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


Obj** garbage_collector = NULL;
int MAX_NUM = 0;

int cmp(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int duplicat(char* name)
{
    int flag = 0;
    for (int i = 0; i < MAX_NUM; ++i)
    {
        if (garbage_collector[i]->free == 0 && strcmp(garbage_collector[i]->name, name) == 0)
        {
            flag = 1;
            break;
        }
    }
    free(name);
    return flag;
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


int my_create(int size, int num_pages)
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

int my_destroy()
{
    if (garbage_collector != NULL)
    {
        for (int i = 0; i < MAX_NUM; ++i)
        {
            if (garbage_collector[i]->free == 0)
            {
                deleteList(&garbage_collector[i]->links);
                free(garbage_collector[i]->name);
            }
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
        if (garbage_collector[i]->free == 1 && duplicat(strdup(name)) == 0)
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
            if (strcmp(name, garbage_collector[i]->name)==0)
            {
                deleteList(&garbage_collector[i]->links);
                free(garbage_collector[i]->name);
                garbage_collector[i]->name = NULL;
                garbage_collector[i]->free = 1;
                return 1;
            }
            if (garbage_collector[i]->links != NULL)
            {
                Block *iter = garbage_collector[i]->links->head;            
                while (iter != NULL)
                {
                    if (strcmp(iter->name, name)==0)
                    {
                        free(iter->name);
                        iter->name = NULL;
                    }
                    iter = iter->next;
                }
            }
        }
    }
    return 0;
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
    Obj **arr = (Obj**)malloc(sizeof(Obj *) * num);
    int j = 0;
    for (int i = 0; i < MAX_NUM; i++)
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
        if (garbage_collector[i]->free == 0 && strcmp(object1_name, garbage_collector[i]->name) == 0 && duplicat(strdup(object2_name)) != 0)
        {
            Add_Block(garbage_collector[i]->links, object2_name);
            return 1;
        }
    }
    return 0;
}

void collect_live_objects(void)
{
    My_List* list = createList();
    for (int i = 0; i < MAX_NUM; ++i)
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
                    for (int j = 0; j < MAX_NUM; ++j)
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
    
    deleteList(&list);
    printf("\n");
}


void setup_memory_manager(memory_manager_t* mm)
{
    mm->create = my_create;
    mm->destroy = my_destroy;
    mm->create_object = create_object;
    mm->destroy_object = destroy_object;
    mm->print_objects = print_objects;
    mm->set_root = set_root;
    mm->link = link;
    mm->collect_live_objects = collect_live_objects;
}
