/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80022de8_slot12[];

void func_800119f8_slot12(Object *obj) {
    int n;
    obj->pos_y += (s8)data_80022de8_slot12[(s16)obj->field_46];
    n = obj->field_46 - 1;
    obj->field_46 = n;
    if ((s16)n < 0) {
        obj->field_05++;
    }
}

void func_80011a50_slot12(Object *obj) {
}

void func_80011a58_slot12(Object *obj) {
    func_8011f240();
}
