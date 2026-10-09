/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern SequenceStep *data_800268b0_slot27[];
extern int data_80190464[];
void func_8001188c_slot27(Object *obj);
void func_80011978_slot27(Object *obj);

void func_8001152c_slot27(Object *obj) {
    Object *p;
    u32 *g;
    int d;
    int v;

    obj->field_09 = 0xc;
    obj->field_98 = data_80017c28_slot27;
    obj->field_9c = data_8001aa14_slot27;
    obj->field_7c = 0x1e0;
    obj->field_90 = (void *)0x80038000;
    p = obj->other;
    obj->field_7a = 0;
    obj->field_0d = 0;
    obj->field_01 = 0;
    obj->field_05 = obj->field_05 + 1;
    g = (u32 *)data_80190464;
    v = p->side;
    *g = v;
    d = 2;
    if (v != 0) {
        d = 5;
    }
    obj->field_0d = d;
    obj->field_0b = *(u8 *)g;
    d = p->kind;
    obj->field_03 = d;
    obj->field_48 = d;
    func_80011978_slot27(obj);
    func_8001188c_slot27(obj);
}

void func_800115e8_slot27(Object *obj) {
    Object *p = obj->other;

    if (((game_state.mode | game_state.field_07) >> p->side) & 1) {
        obj->field_05 = 0;
        obj->field_04 = obj->field_04 + 1;
        func_80130768(obj, obj->field_48, data_800268b0_slot27);
        obj->field_01 = 1;
    }
}
