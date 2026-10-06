/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"


extern Object *data_8002d56c_slot12;
extern int data_8002d578_slot12;
extern int data_8002a390_slot12;
extern ObjectFn data_80022e20_slot12[];
void func_800121ec_slot12(Object *obj, int a);
void func_800124fc_slot12(Object *obj, FrameRecord *f);

void func_800120bc_slot12(Object *obj) {
    if (game_state.field_44 != 0 && data_8002d578_slot12 == 0) {
        obj->field_05 = 3;
        func_800121ec_slot12(obj, 1);
    } else if (game_state.field_ab & 0x80) {
        obj->field_05 = obj->field_05 + 1;
        func_800121ec_slot12(obj, 2);
    }
}
