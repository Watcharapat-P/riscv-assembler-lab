#include <assert.h>
#include <stdio.h>
#include "../src/registers.h"

void test_parse_register() {
    // Valid registers x0 to x31
    assert(parse_register("x0") == 0);
    assert(parse_register("x1") == 1);
    assert(parse_register("x5") == 5);
    assert(parse_register("x10") == 10);
    assert(parse_register("x31") == 31);
    
    // Invalid: out of range
    assert(parse_register("x32") == -1);
    assert(parse_register("x-1") == -1);
    
    // Invalid: wrong prefix
    assert(parse_register("y0") == -1);
    assert(parse_register("X0") == -1);
    
    // Invalid: malformed
    assert(parse_register("x") == -1);
    assert(parse_register("x5a") == -1);
    assert(parse_register("x 5") == -1);
    
    // Invalid: NULL input
    assert(parse_register(NULL) == -1);
}

int main() {
    test_parse_register();
    printf("All tests passed!\n");
    return 0;
}
