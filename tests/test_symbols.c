#include <assert.h>
#include <stdio.h>
#include "../src/symbols.h"

void test_symbols() {
    SymbolTable st;
    symbols_init(&st);
    
    // Test add and find
    assert(symbols_add(&st, "loop", 0x100) == 1);
    assert(symbols_find(&st, "loop") == 0x100);
    
    // Test add another
    assert(symbols_add(&st, "end", 0x200) == 1);
    assert(symbols_find(&st, "end") == 0x200);
    assert(symbols_find(&st, "loop") == 0x100);
    
    // Test update
    assert(symbols_add(&st, "loop", 0x150) == 1);
    assert(symbols_find(&st, "loop") == 0x150);
    
    // Test not found
    assert(symbols_find(&st, "nonexistent") == (uint32_t)-1);
    
    // Test count
    assert(st.count == 2);
}

int main() {
    test_symbols();
    printf("All symbol table tests passed!\n");
    return 0;
}
