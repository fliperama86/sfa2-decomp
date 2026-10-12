/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e5160_slot2a[];
extern u16 *data_801e52c8_slot2a;
void func_801e14cc_slot2a(Object *obj);
void func_801e1480_slot2a(Object *obj);
void func_801e13ac_slot2a(Object *obj);

void func_801e1360_slot2a(Object *obj) {
    obj->health = 1;
    obj->pos_y = 0x48;
    obj->state_1++;
    if (obj->table_index) {
        obj->pos_y = 0xa8;
    }
    func_801e14cc_slot2a(obj);
}

void func_801e13ac_slot2a(Object *obj) {
    Slot2aSel *sel;
    unsigned t;
    if (game_state.status_flags) {
        sel = (Slot2aSel *)data_801e52c8_slot2a;
        if (sel->select_flags & 0x8000) {
            func_801e1480_slot2a(obj);
        } else {
            t = sel->selection_value;
            obj->state_1 = 2;
            obj->countdown = 0x13;
            if (obj->table_index == (t >> 15)) {
                obj->state_1 = 0;
            } else {
                data_801e5160_slot2a[obj->table_index] = 1;
            }
        }
    }
}

void func_801e143c_slot2a(Object *obj) {
    s16 t;
    t = obj->countdown;
    t--;
    obj->countdown = t;
    if (t < 0) {
        obj->state_1 = 1;
        obj->countdown = 0;
        obj->health = 0;
        func_801e13ac_slot2a(obj);
    }
}
void func_801e1480_slot2a(Object *obj) {
    obj->state_0++;
    obj->health = 0;
    data_801e5160_slot2a[obj->table_index] = 0;
}

void func_801e14ac_slot2a(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
