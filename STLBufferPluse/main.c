//
//  main.c
//  STLBufferPluse
//
//  Created by Kos on 23.04.2024.
//

#include <stdio.h>
#import "Test.h"

int main(int argc, const char * argv[]) {
    memory_manager_t mm;
    setup_memory_manager(&mm);
    int create_result = mm.create(10, 1) != 0;
    int destroy_result = mm.destroy() != 0;
    
    // insert code here...
    printf("Hello, World!\n");
    printf("create_result: %i \n", create_result);
    printf("destroy_result: %i \n", destroy_result);
    mm.create_object("o9");
    return 0;
}
