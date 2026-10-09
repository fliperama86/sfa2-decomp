/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_80024488_slot12[];
extern SequenceStep *data_800280a0_slot12[];

void func_80014d08_slot12(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        int t = obj->field_46 - 1;
        obj->field_46 = t;
        if ((s16)t < 0) {
            obj->field_05++;
            game_state.field_2bc = 9;
        }
    }
    func_80131094(obj);
}

void func_80014d70_slot12(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->field_04++;
    }
    func_80131094(obj);
}

void func_80014db0_slot12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80014dd0_slot12(Object *obj, int a) {
    char *base = (char *)0x8005b000;
    obj->field_90 = base;
    obj->field_90 = base + data_80024488_slot12[obj->field_60];
    func_80130768(obj, (a + obj->field_60 * 2) & 0xff, data_800280a0_slot12);
}
