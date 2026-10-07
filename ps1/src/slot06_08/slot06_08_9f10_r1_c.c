/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ea230_slot06_08(Object *obj);

extern SequenceStep *data_801ed480_slot06_08[];
extern ObjectFn data_801ed4e4_slot06_08[];

void func_801ea210_slot06_08(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801ea230_slot06_08(Object *obj) {
    if (player_left.field_06 == 8 || player_right.field_06 == 8) {
        int idx = 11;
        obj->field_05 = 1;
        if (obj->field_03 != 0) {
            idx = 9;
        }
        func_80130700(obj, data_801ed480_slot06_08[idx]);
    } else {
        func_80131094(obj);
    }
}

void func_801ea2b0_slot06_08(Object *obj) {
    data_801ed4e4_slot06_08[obj->field_04](obj);
}
