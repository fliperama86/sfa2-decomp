/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80048d48_slot28[][2];
void func_80022f04_slot28(Object *obj, int idx);

void func_80022f84_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Slot28Obj *o = (Slot28Obj *)obj;
    int n;
    if (obj->table_index != 0) {
        o->pos_x += obj->vel_x;
        o->pos_y -= obj->vel_y;
        n = obj->table_index;
        if (obj->pos_x < 0x30) {
            obj->pos_x = data_80048d48_slot28[n][0];
            obj->pos_y = data_80048d48_slot28[obj->table_index][1];
        }
    } else if (obj->variant_index != 0) {
        obj->variant_index = 0;
        obj->state_1++;
        func_80022f04_slot28(obj, 1);
    }
}

void func_80023044_slot28(Object *obj) {
    ((Slot28Obj *)obj)->pos_x += 0xc000;
    if (obj->pos_x >= 0xd8) {
        obj->pos_x = 0xd8;
        obj->state_1++;
    }
}
