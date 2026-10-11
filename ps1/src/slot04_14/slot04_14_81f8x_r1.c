/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack them (inferred). */
void func_801b8354_slot04_14(Object *obj);
void func_801b8430_slot04_14(Object *obj, u8 arg);

void func_801b81f8_slot04_14(Object *obj) {
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
        /* The original stores 6 to field_09 twice, here and before the copy of field_1c. Without this second store this function differs from the original in 3 instruction slots. */
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
        if ((s16)other->field_5c != 0x90) {
            palette++;
        }
        obj->field_03 = palette;
        func_801b8430_slot04_14(obj, palette);
        if (other->side == 0) {
            func_80130768(obj, 8, data_1f8000b4);
        } else {
            func_80130768(obj, 8, data_1f800164);
        }
        func_8011ffdc(obj);
    } else {
        func_801b8354_slot04_14(obj);
    }
}
