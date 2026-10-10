/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern s16 data_8002b386_slot27;
void func_80016810_slot27(Object *obj);

void func_80015ca4_slot27(Object *obj) {
    Object *b;
    if (obj->field_60 != 0) {
        func_80131094(obj);
        func_8011ffdc(obj);
    } else if (obj->field_05 != 0) {
        if (game_state.field_04 != 0) {
            obj->field_04++;
        }
        func_8011ffdc(obj);
    } else if (data_8002b386_slot27 < 0) {
        obj->field_05++;
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0xaa;
            b->field_01 = 1;
            b->field_60 = 1;
            b->field_03 = game_state.field_40;
            b->field_90 = (void *)0x80038000;
            b->field_98 = data_80017c28_slot27;
            b->field_9c = data_8001aa14_slot27;
            b->field_7a = 0;
            b->field_7c = 0x1e0;
            b->field_0d = 0;
        }
    } else {
        func_80016810_slot27(obj);
    }
}
