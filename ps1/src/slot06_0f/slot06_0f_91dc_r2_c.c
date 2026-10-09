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
        data_801eb4d8_slot06_0f[obj->field_05](obj);
        func_80131094(obj);
    }
    func_80120028(obj);
}

void func_801e9a98_slot06_0f(Object *obj) {
    if (game_state.field_47 != 0 && game_state.field_5c == 0) {
        Slot06Obj *s = (Slot06Obj *)obj;
        obj->field_05++;
        func_80130700(obj, s->field_6c[obj->field_48]);
    }
}

void func_801e9afc_slot06_0f(Object *obj) {
}

void func_801e9b04_slot06_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9b24_slot06_0f(Object *obj) {
    Slot06Obj *s = (Slot06Obj *)obj;
    s->field_6c = data_801eb4e0_slot06_0f[obj->field_03];
    obj->field_48 = data_801eb4e4_slot06_0f[obj->field_03].field_01;
    func_80130700(obj, s->field_6c[data_801eb4e4_slot06_0f[obj->field_03].field_00]);
}
