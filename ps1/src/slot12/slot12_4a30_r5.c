/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800282c4_slot12[])(Object *);
extern u16 data_800282d4_slot12[];
extern Slot12Cell data_8002bc38_slot12[];

void func_80014e30_slot12(Object *obj) {
    data_800282c4_slot12[obj->field_04](obj);
}

void func_80014e70_slot12(Object *obj) {
    Slot12Cell *c;
    int i;
    obj->field_09 = 2;
    obj->field_04++;
    if (obj->field_03 == 0) {
        obj->pos_x = data_800282d4_slot12[0];
        obj->pos_y = data_800282d4_slot12[1];
    } else {
        obj->pos_x = data_800282d4_slot12[2];
        obj->pos_y = data_800282d4_slot12[3];
    }
    c = data_8002bc38_slot12 + obj->field_03 * 2;
    for (i = 0; i < 2; i++) {
        c->field_03 = 3;
        c->field_07 = 0x60;
        c->field_08 = obj->pos_x;
        c->field_0a = obj->pos_y;
        if (obj->field_03 == 0) {
            c->field_0c = 0x58;
        } else {
            c->field_0c = 0x80;
        }
        c->field_0e = 0x58;
        c->field_04 = 1;
        c->field_05 = 1;
        c->field_06 = 1;
        c++;
    }
}
