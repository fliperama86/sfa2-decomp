/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b7d58_slot04_02(Object *obj);
void func_801b7e34_slot04_02(Object *obj, u8 arg);

/*
 * What it does (inferred, not an original name): a state of a helper object
 * that follows another one (field_3c). While game_state.field_64 is not 0 it
 * copies fields of the other object, places itself at fixed coordinates, steps
 * field_03 up by one unless the other object's field_5c is 0x90, loads a
 * palette copy for that value, starts a sequence table chosen by the other
 * object's side and registers itself. Otherwise it only calls
 * func_801b7d58_slot04_02.
 */
void func_801b7bfc_slot04_02(Object *obj) {
    Object *other;
    u8 palette;

    obj->state_0++;
    other = obj->field_3c;
    if (game_state.field_64 != 0) {
        obj->field_0f = 0;
        obj->field_0e = 0;
        obj->field_0b = 0;
        obj->field_09 = 6;
        obj->field_1c = other->field_1c;
        /* The original stores 6 to field_09 twice; without the second store the build differs in 3 instruction lines. */
        obj->field_09 = 6;
        obj->field_90 = other->field_90;
        obj->field_98 = other->field_98;
        obj->field_9c = other->field_9c;
        obj->field_7a = other->field_7a;
        obj->field_7c = other->field_7c;
        obj->pos_x = box_margin[0] + 0xb8;
        obj->pos_y = 0x78 - data_801aa5ea[0];
        obj->field_0d = 0x1f;
        obj->field_44 = 1;
        obj->field_46 = 0x15;
        obj->accel_x = 6;
        obj->field_a0 = 0;
        palette = obj->field_03;
        /* Without the (s16) cast the build differs in 1 instruction line. */
        if ((s16)other->field_5c != 0x90) {
            palette++;
        }
        obj->field_03 = palette;
        func_801b7e34_slot04_02(obj, palette);
        if (other->side == 0) {
            func_80130768(obj, 8, data_1f8000b4);
        } else {
            func_80130768(obj, 8, data_1f800164);
        }
        func_8011ffdc(obj);
    } else {
        func_801b7d58_slot04_02(obj);
    }
}
