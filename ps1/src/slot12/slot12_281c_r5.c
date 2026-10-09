/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern u8 data_80019054_slot12[];
extern u8 data_80019548_slot12[];
extern SequenceStep *data_8001d4ec_slot12[];
extern int data_8002d57c_slot12;

void func_80012cdc_slot12(Object *obj) {
    u16 *in = &data_801ae066;

    *in = obj->field_3a;
    if (*in & 0x8000) {
        obj->field_01 = 0;
        obj->field_04++;
        game_state.field_06 = 0;
        data_801ae02c = 0;
    }
    if (data_8002d57c_slot12 != 0) {
        func_80131094(obj);
    }
    if (*in & 0x80) {
        data_8002d57c_slot12 = 0;
    }
}
