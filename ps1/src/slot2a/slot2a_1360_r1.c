/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_8011f240(Slab172 *s);

extern u8 data_801e5160_slot2a[];
extern u16 *data_801e52c8_slot2a;
void func_801e14cc_slot2a(Object *obj);
void func_801e1480_slot2a(Object *obj);
void func_801e13ac_slot2a(Object *obj);

void func_801e1360_slot2a(Object *obj) {
    obj->field_5c = 1;
    obj->pos_y = 0x48;
    obj->field_05++;
    if (obj->field_03) {
        obj->pos_y = 0xa8;
    }
    func_801e14cc_slot2a(obj);
}

void func_801e13ac_slot2a(Object *obj) {
    Slot2aSel *sel;
    unsigned t;
    if (game_state.field_ab) {
        sel = (Slot2aSel *)data_801e52c8_slot2a;
        if (sel->field_00 & 0x8000) {
            func_801e1480_slot2a(obj);
        } else {
            t = sel->field_02;
            obj->field_05 = 2;
            obj->field_46 = 0x13;
            if (obj->field_03 == (t >> 15)) {
                obj->field_05 = 0;
            } else {
                data_801e5160_slot2a[obj->field_03] = 1;
            }
        }
    }
}

void func_801e143c_slot2a(Object *obj) {
    s16 t;
    t = obj->field_46;
    t--;
    obj->field_46 = t;
    if (t < 0) {
        obj->field_05 = 1;
        obj->field_46 = 0;
        obj->field_5c = 0;
        func_801e13ac_slot2a(obj);
    }
}
void func_801e1480_slot2a(Object *obj) {
    obj->field_04++;
    obj->field_5c = 0;
    data_801e5160_slot2a[obj->field_03] = 0;
}

void func_801e14ac_slot2a(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
