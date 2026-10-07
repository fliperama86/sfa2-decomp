/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_8011f240(Slab172 *s);

extern Slot12Prim data_8002c3c8_slot12[];
extern HudSlot data_8002c4c8_slot12[4];
extern Slot12Prim data_8002c528_slot12[];
extern u16 data_80028a58_slot12[];
extern u16 data_80028a60_slot12[];

void func_800172d0_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    Slot12Prim *p;
    int i = 0;
    if (obj->field_5c >= 0) {
        p = &data_8002c528_slot12[data_801a27d0];
        do {
            p[i * 2].x = obj->field_12 + i * 16 + 16;
            p[i * 2].y = obj->field_16;
            i++;
        } while (obj->field_5c >= i);
    }
}

void func_80017334_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    int i = 0;
    int t = obj->field_5c << 4;
    Slot12Prim *p = &data_8002c3c8_slot12[data_801a27d0];
    HudSlot *q;
    do {
        p[i * 2].x = obj->field_12 + data_80028a58_slot12[i] + t + 16;
        p[i * 2].y = obj->field_16;
        i++;
    } while (i < 4);
    i = 0;
    q = data_8002c4c8_slot12;
    do {
        q[i].field_04 = obj->field_12 + data_80028a60_slot12[i] + t + 16;
        q[i].field_06 = obj->field_16 + 8;
        i++;
    } while (i < 4);
}

void func_800173e8_slot12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
