//
//  main.c
//  STLBufferPluse
//
//  Created by Kos on 23.04.2024.
//

#include <stdio.h>
#include <assert.h>
#import "Test.h"
//#import "Test1.h"

int main(int argc, const char * argv[]) {
    memory_manager_t mm;
    setup_memory_manager(&mm);
    assert(mm.create(10, 1)); //expected: 1, passed
    assert(mm.create_object("o9"));// expected: 1, passed
    assert(mm.create_object("o1"));// expected: 1, passed
    assert(mm.create_object("o2"));// expected: 1, passed
    assert(mm.create_object("o3"));// expected: 1, passed
    assert(mm.create_object("o4"));// expected: 1, passed
    assert(mm.create_object("o5"));// expected: 1, passed
    assert(mm.create_object("o6"));// expected: 1, passed
    assert(mm.create_object("o7"));// expected: 1, passed
    assert(mm.create_object("o8"));// expected: 1, passed
    assert(mm.create_object("o0"));// expected: 1, passed
    assert(mm.set_root("1234567") == 0);// expected: 0, passed
    assert(mm.set_root("o1"));// expected: 1, passed
    assert(mm.set_root("o2"));// expected: 1, passed
    assert(mm.set_root("o9"));// expected: 1, passed
    assert(mm.link("o1", "1234567") == 0);// expected: 0, passed
    assert(mm.link("o1", "o3")); //expected: 1, passed
    assert(mm.destroy_object("o1"));// expected: 1, passed
    assert(mm.link("o1", "o4") == 0);// expected: 0, passed
    assert(mm.link("o2", "o4"));// expected: 1, passed
    assert(mm.link("o4", "o2"));// expected: 1, passed
    assert(mm.link("o4", "o6"));// expected: 1, passed
    assert(mm.link("o5", "o7"));// expected: 1, passed
    assert(mm.link("o7", "o1") == 0);// expected: 0, passed
    assert(mm.link("o4", "o8")); //expected: 1, passed
    mm.print_objects();
    mm.collect_live_objects();
    assert(mm.destroy_object("o4")); //expected: 1, passed
    mm.collect_live_objects();
    assert(mm.destroy_object("o2"));// expected: 1, passed
    assert(mm.destroy_object("o9"));// expected: 1, passed
    mm.print_objects();
    mm.collect_live_objects();
    mm.destroy();// expected: 1
    return 0;
}
