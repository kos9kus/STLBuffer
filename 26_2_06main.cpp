//#include "26_2_06task.cpp"
#include "os_mem.h"
#include <stdio.h>
#include <stdio.h>

int main()
{
    memory_manager_t mm;
    setup_memory_manager(&mm);
    mm.create(10, 1) != 0; //expected: 1, passed
    mm.create_object("o9");// expected: 1, passed
    mm.create_object("o1");// expected: 1, passed
    mm.create_object("o2");// expected: 1, passed
    mm.create_object("o3");// expected: 1, passed
    mm.create_object("o4");// expected: 1, passed
    mm.create_object("o5");// expected: 1, passed
    mm.create_object("o6");// expected: 1, passed
    mm.create_object("o7");// expected: 1, passed
    mm.create_object("o8");// expected: 1, passed
    mm.create_object("o0");// expected: 1, passed
    mm.set_root("1234567");// expected: 0, passed
    mm.set_root("o1");// expected: 1, passed
    mm.set_root("o2");// expected: 1, passed
    mm.set_root("o9");// expected: 1, passed
    mm.link("o1", "1234567");// expected: 0, passed
    mm.link("o1", "o3"); //expected: 1, passed
    mm.destroy_object("o1");// expected: 1, passed
    mm.link("o1", "o4");// expected: 0, passed
    mm.link("o2", "o4");// expected: 1, passed
    mm.link("o4", "o2");// expected: 1, passed
    mm.link("o4", "o6");// expected: 1, passed
    mm.link("o5", "o7");// expected: 1, passed
    mm.link("o7", "o1");// expected: 0, passed
    mm.link("o4", "o8"); //expected: 1, passed
    mm.print_objects();
    mm.collect_live_objects();
    mm.destroy_object("o4"); //expected: 1, passed
    mm.collect_live_objects();
    mm.destroy_object("o2");// expected: 1, passed
    mm.destroy_object("o9");// expected: 1, passed
    mm.print_objects();
    mm.collect_live_objects();
    mm.destroy();// expected: 1   
    return 0;
}

/*
o0 o2 o3 o4 o5 o6 o7 o8 o9

o2 o4 o6 o8 o9

o2 o9

o0 o3 o5 o6 o7 o8

*/