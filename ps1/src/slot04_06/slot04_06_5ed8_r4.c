/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c56a4_slot04_06[];

void func_801b6334_slot04_06(Object *obj) {
    if (game_state.gate_flag != 0) {
        obj->state_0 = 2;
    } else {
        data_801c56a4_slot04_06[obj->table_index](obj);
    }
}

void func_801b638c_slot04_06(Object *obj) {
    int t;
    int a;

    t = obj->variant_index + 1;
    obj->variant_index = t & 0x1f;
    a = 1;
    if (t & 0x10) {
        a = -1;
    }
    obj->pos_y -= a;
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}
