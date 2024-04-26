//
//  main.c
//  STLBufferPluse
//
//  Created by Kos on 23.04.2024.
//

#include <stdio.h>
#import "Test.h"
//#import "Test1.h"

int main(int argc, const char * argv[]) {
    memory_manager_t mm;
    setup_memory_manager(&mm);
    mm.create(10, 1); //expected: 1, passed
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
    
//    memory_manager_t mm;
//    setup_memory_manager(&mm);
//    int create_result = mm.create(10, 1);
//    // insert code here...
//    printf("Hello, World!\n");
//    printf("create_result: %i \n", create_result);
//    int create_object_o9 = mm.create_object("o9");
//    printf("create_object_o9: %i \n", create_object_o9);
//    int create_object_o9_dub = mm.create_object("o9");
//    printf("create_object_o9_dub: %i \n", create_object_o9_dub);
//    mm.print_objects();
//    int destroy_result = mm.destroy();
//    printf("destroy_result: %i \n", destroy_result);
//    return 0;
}
