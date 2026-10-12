/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


u32 convert_to_bcd(s16 value) {
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

void call_phase_function(void *a, Object *object) {
    table_8018179c[object->phase_index](a, object);
}
