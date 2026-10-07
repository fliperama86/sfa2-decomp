/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e1e00_slot0f(Object *o);
extern Slot0fObj data_800f857c_slot0f;
extern void (*data_800e8778_slot0f[])(Object *);

void func_800e1640_slot0f(void) {
    Slot0fObj *o = &data_800f857c_slot0f;
    data_800f857c_slot0f.field_03 = 0;
    data_800f857c_slot0f.field_02 = 0;
    data_800f857c_slot0f.field_01 = 0;
    o->field_00 = 0;
    data_800f857c_slot0f.field_08 = 1;
    do {
        o->field_10 = data_801a696a | data_801a6976;
        data_800e8778_slot0f[(s8)o->field_00]((Object *)o);
        func_800e1e00_slot0f((Object *)o);
        func_801192bc(1);
    } while (o->field_08 != 0);
}

void func_800e16f4_slot0f(Object *o) {
    u8 n = o->field_00;
    o->field_07 = 0;
    o->field_06 = 0;
    o->field_05 = 0;
    o->field_04 = 0;
    o->field_09 = 0;
    o->field_0a = 0;
    o->field_00 = n + 1;
}
