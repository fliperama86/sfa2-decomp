/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8002e190_slot28[];
void func_80015fcc_slot28(Object *obj, SeqRec **table, u8 index);
void func_80015f50_slot28(Object *obj);

void func_80015ea4_slot28(Object *obj) {
    obj->field_48 = 0;
    obj->field_04++;
    func_80015fcc_slot28(obj, (SeqRec **)data_8002e190_slot28, obj->field_03);
}

void func_80015edc_slot28(Object *obj) {
    if (obj->field_48 != 0) {
        obj->field_04++;
    }
    if (game_state.field_f0 == 0) {
        func_80015f50_slot28(obj);
    }
}
