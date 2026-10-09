/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_801c0168_slot04_00[];
extern u32 data_801c0314_slot04_00[];
extern u32 data_801c052c_slot04_00[];

void func_801b3454_slot04_00(Object *obj) {
    ref_other.p = obj->field_3c;
    ref_other.p->field_240--;
    if (ref_other.p->field_240 & 0x80) {
        ref_other.p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)obj);
}

void func_801b34c4_slot04_00(Object *obj) {
    if (obj->field_ac == 6) {
        game_state.cursor = data_801c0168_slot04_00;
    } else if (obj->field_ac == 8) {
        game_state.cursor = data_801c0314_slot04_00;
    } else {
        game_state.cursor = data_801c052c_slot04_00;
    }
    game_state.cursor = (u32 *)game_state.cursor[(s16)obj->field_5c];
    obj->box_tables = (BoxTables *)game_state.cursor;
}
