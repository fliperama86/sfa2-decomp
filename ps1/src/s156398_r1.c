/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u32 func_80156454(s16 value) {
    u32 result = 0;
    u32 n = value;
    int i;

    for (i = 0; i < 8; i++) {
        u32 digit = n % 10;
        n = n / 10;
        result += digit << (i * 4);
    }
    return result;
}

void func_801564b0(void *a, Object *object) {
    table_8018179c[object->field_aa](a, object);
}
