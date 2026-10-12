/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8002e190_slot28[];
void func_80015fcc_slot28(Object *obj, SeqRec **table, u8 index);
void func_80015f50_slot28(Object *obj);

void func_80015ea4_slot28(Object *obj) {
    obj->variant_index = 0;
    obj->state_0++;
    func_80015fcc_slot28(obj, (SeqRec **)data_8002e190_slot28, obj->table_index);
}

void func_80015edc_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_0++;
    }
    if (game_state.wait_flag == 0) {
        func_80015f50_slot28(obj);
    }
}
