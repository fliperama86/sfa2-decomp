/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 check_script_condition_by_mode(Object *object) {
    u8 mode = object->script_byte_lo & 0xfe;
    if (mode == 0) return 0;
    if (mode == 2) return is_compare_limit_within_limit_hi(object);
    if (mode == 4) return tick_counter_21c_is_done(object);
    if (mode == 6) return is_within_limit_and_y_range_of_other(object);
}

u8 is_compare_limit_within_limit_hi(Object *object) {
    return !((s16)object->compare_limit > object->limit_hi_2);
}

u8 tick_counter_21c_is_done(Object *object) {
    u8 result;
    if (object->countdown_3 != 0) {
        object->countdown_3--;
        result = 0;
    } else {
        result = 1;
    }
    return result;
}

u8 is_within_limit_and_y_range_of_other(Object *object) {
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

u8 check_enemy_unit_or_other_in_range(Object *object) {
    return is_enemy_unit_or_other_in_range(object);
}
