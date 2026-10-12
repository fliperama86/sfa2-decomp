/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9b24_slot06_0f(Object *obj);

extern ObjectFn data_801eb4d8_slot06_0f[];
extern SequenceStep **data_801eb4e0_slot06_0f[];
extern Slot06_0fRecb4e4 data_801eb4e4_slot06_0f[];

void func_801e9a24_slot06_0f(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801eb4d8_slot06_0f[obj->state_1](obj);
        advance_sequence_checked(obj);
    }
    func_80120028(obj);
}

void func_801e9a98_slot06_0f(Object *obj) {
    if (game_state.pause_flag_2 != 0 && game_state.countdown_timer == 0) {
        Slot06Obj *s = (Slot06Obj *)obj;
        obj->state_1++;
        func_80130700(obj, s->sequence_table[obj->variant_index]);
    }
}

void func_801e9afc_slot06_0f(Object *obj) {
}

void func_801e9b04_slot06_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9b24_slot06_0f(Object *obj) {
    Slot06Obj *s = (Slot06Obj *)obj;
    s->sequence_table = data_801eb4e0_slot06_0f[obj->table_index];
    obj->variant_index = data_801eb4e4_slot06_0f[obj->table_index].entry_value;
    func_80130700(obj, s->sequence_table[data_801eb4e4_slot06_0f[obj->table_index].entry_index]);
}
