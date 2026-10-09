/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
Block172 *func_8011f1e0(void);

void func_80015838_slot27(Object *obj) {
    Object *p = (Object *)func_8011f1e0();
    Object *b = p;
    if (b != 0) {
        b->field_00 = 1;
        b->field_02 = 0xaa;
        b->field_03 = game_state.field_40;
        b->field_09 = 8;
        b->field_90 = (void *)0x80038000;
        b->field_98 = data_80017c28_slot27;
        b->field_9c = data_8001aa14_slot27;
        b->field_7a = 0;
        b->field_7c = 0x1e0;
        obj->field_3c = b;
    }
}
