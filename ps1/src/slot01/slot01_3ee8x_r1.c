/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80015cdc_slot01)(Object *obj);

/* The form of func_80015de0_slot27 of another module, with other values and
   three more fields cleared. */
void func_80013ee8_slot01(Object *obj) {
    obj->field_09 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_24 = 0;
    obj->field_0c = 1;
    obj->field_0d = 0;
    obj->pos_x = 0x1e0;
    obj->pos_y = 0x54;
    obj->field_48 = 0;
    obj->field_46 = 0x1f;
    obj->field_04++;
    if ((game_state.mode & 1) == 0) {
        obj->field_48 = 1;
        obj->pos_x = 0x194;
    }
    data_80015cdc_slot01(obj);
}
