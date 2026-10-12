/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 func_80149c30(Object *object) {
    u8 mode = object->script_byte_lo & 0xfe;
    if (mode == 0) return 0;
    if (mode == 2) return func_80149cb0(object);
    if (mode == 4) return func_80149cc8(object);
    if (mode == 6) return func_80149cf8(object);
}

u8 func_80149cb0(Object *object) {
    return !((s16)object->compare_limit > object->limit_hi_2);
}

u8 func_80149cc8(Object *object) {
    u8 result;
    if (object->countdown_3 != 0) {
        object->countdown_3--;
        result = 0;
    } else {
        result = 1;
    }
    return result;
}

u8 func_80149cf8(Object *object) {
    Object *other;
    int margin;
    if (!((s16)object->compare_limit > object->limit_hi_2)) {
        other = object->other;
        margin = (u16)object->pos_y - (u16)other->pos_y;
        margin += object->y_margin;
        return !(object->y_margin * 2 > (s16)margin);
    } else {
        return 0;
    }
}

u8 func_80149d48(Object *object) {
    return func_80149d6c(object);
}
