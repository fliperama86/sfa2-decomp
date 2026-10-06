/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801e52c8_slot2a;
extern u8 data_801e25d8_slot2a[];
extern SequenceStep *data_801e21f4_slot2a[];
extern u32 data_801e21fc_slot2a[];

void func_801e0850_slot2a(Object *obj) {
    SlotInput *in;
    int k;

    if (game_state.field_ab != 0) {
        in = (SlotInput *)data_801e52c8_slot2a;
        if (in->buttons & 0x8000) {
            obj->field_04++;
        } else if ((k = obj->field_03) == (in->field_02 >> 15)) {
            if (data_801e25d8_slot2a[k] == 0) {
                obj->field_05 = 0;
            }
        }
    }
}

void func_801e08d4_slot2a(Object *obj) {
    func_8011f240();
}

void func_801e08f4_slot2a(Object *obj, u8 idx) {
    data_801e21f4_slot2a[obj->field_03]->field_04 = data_801e21fc_slot2a[idx];
    func_80130768(obj, obj->field_03, data_801e21f4_slot2a);
}
