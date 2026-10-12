/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c1630_slot04_05[];
extern int data_801c1700_slot04_05;

void update_input_slot_repeat_or_timeout_slot04_05(Object *obj, int a1, int a2) {
    u8 k = a2;
    u8 i = a1;
    u8 t;

    if (obj->pressed_bits & data_801c1630_slot04_05[k * 3 + 6]) {
        t = obj->slots[i].repeat_count - 1;
        obj->slots[i].repeat_count = t;
        if (t != 0) {
            obj->slots[i].timer = 0xc;
        } else {
            data_801c1700_slot04_05 = -1;
        }
    } else {
        t = obj->slots[i].timer - 1;
        obj->slots[i].timer = t;
        if (t == 0) {
            obj->slots[i].stage = 0;
        }
    }
}
