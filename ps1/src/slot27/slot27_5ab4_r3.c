/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028b88_slot27)(Object *);

void func_80015de0_slot27(Object *obj) {
    obj->field_09 = 1;
    obj->field_0c = 1;
    obj->field_0d = 0x16;
    obj->pos_x = 0x320;
    obj->pos_y = 0x54;
    obj->field_48 = 0;
    obj->field_46 = 0x1f;
    obj->field_04++;
    if ((game_state.mode & 1) == 0) {
        obj->field_48 = 1;
        obj->pos_x = 0x2e0;
    }
    data_80028b88_slot27(obj);
}
