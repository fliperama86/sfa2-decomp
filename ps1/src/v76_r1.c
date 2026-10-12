#include "game.h"
#include "externs.h"
#include "protos.h"
/* Exact. */
u8 test_random_bit_in_table(Object *object, u32 *table) {
    u32 mask;
    short i, n;
    u32 v;
    n = get_random_byte() & 0x1f;
    mask = 1;
    table += object->table_column;
    v = *table;
    for (i = 0; i < n; i++) mask <<= 1;
    return (mask & v) != 0;
}
