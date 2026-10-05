#include "game.h"
#include "externs.h"
#include "protos.h"
/* Exact. */
u8 func_8014a170(Object *object, u32 *table) {
    u32 mask;
    short i, n;
    u32 v;
    n = func_80151184() & 0x1f;
    mask = 1;
    table += object->field_cf;
    v = *table;
    for (i = 0; i < n; i++) mask <<= 1;
    return (mask & v) != 0;
}
