/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u16 func_80147158(Object *object) {
    u16 result = 0xf;
    int kind;
    if (object->field_167 != 0xb) {
        kind = object->field_255;
        if (kind == 6) {
            result = table_8017cae4[4 + (object->field_d5 >> 4)];
        } else {
            result = table_8017cae4[(kind & 0xfe) >> 1];
        }
        if (game_state.mode != 3) {
            object->field_ec++;
            if (object->field_255 == 6) {
                object->field_ed++;
            }
        }
    }
    return result;
}

void func_801471f8(Block172 *block) {
    Object *object = (Object *)block;
    fn_table_8017cb00[object->field_04](object);
}
