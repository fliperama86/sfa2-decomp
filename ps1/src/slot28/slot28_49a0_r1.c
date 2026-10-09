/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern ObjectRef data_800518b8_slot28;
extern ObjectRef data_800518cc_slot28;
extern Object *data_800518bc_slot28[];
extern ObjectRef data_800518c8_slot28;
extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern ObjectRef data_80051898_slot28;
extern ObjectRef data_8005189c_slot28;
extern ObjectRef data_800518a0_slot28;
extern SequenceStep *data_8002bdd8_slot28[];
extern SequenceStep *data_8002bdd4_slot28[];
extern SequenceStep *data_8002bddc_slot28[];
extern SequenceStep *data_8002bde4_slot28[];
extern SequenceStep *data_8002be28_slot28[];
extern u8 data_8002acc4_slot28[];
extern u8 data_8002b024_slot28[];

void func_80014d58_slot28(Object *obj);

void func_800149a0_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    Object *o;
    int one;
    h->field_60 = 0xf0;
    h->field_50 = h->field_50 + 1;
    func_801280f0();
    one = 1;
    data_80190568 = one;
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80014d58_slot28(o);
        data_800518b8_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 0;
        func_80130768(o, 0, data_8002bdd8_slot28);
        o->field_01 = one;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80014d58_slot28(o);
        data_800518cc_slot28.p = o;
        o->pos_y = -8;
        o->pos_x = 0;
        o->field_7a = 0;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 0;
        func_80130768(o, 0, data_8002bdd4_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80014d58_slot28(o);
        data_800518bc_slot28[0] = o;
        o->pos_x = 0x60;
        o->pos_y = 0x21;
        o->field_7a = 0x10;
        o->field_7c = 0x1e0;
        o->field_0d = 0;
        o->field_09 = 2;
        func_80130768(o, 0, data_8002bddc_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80014d58_slot28(o);
        data_800518c8_slot28.p = o;
        o->field_7c = 0x1e0;
        o->pos_x = 0x28;
        o->field_7a = 0;
        o->field_0d = 0;
        o->field_03 = one;
        o->field_09 = 0;
        o->field_50 = 0xb8;
        func_80130768(o, 0, data_8002be28_slot28);
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_02 = 0x76;
        o->pos_x = 0xe8;
        o->pos_y = 0xa0;
        o->field_90 = (void *)0x80060000;
        o->field_98 = data_8002acc4_slot28;
        o->field_9c = data_8002b024_slot28;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->field_46 = 0x200;
        o->field_00 = one;
        o->field_03 = 0;
        o->field_0d = 0;
        o->field_0c = 0;
        o->field_09 = one;
        ((Slot28Obj *)o)->field_6c = data_8002bde4_slot28;
        data_80051890_slot28.p = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80014d58_slot28(o);
        o->field_09 = 4;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->pos_x = 0xc0;
        o->field_0d = 0;
        o->pos_y = 0xa0;
        func_80130768(o, 0, data_8002bde4_slot28);
        data_80051894_slot28.p = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80014d58_slot28(o);
        o->field_09 = 3;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->pos_x = 0x70;
        o->field_0d = 0;
        o->pos_y = 0x50;
        func_80130768(o, 1, data_8002bde4_slot28);
        data_80051898_slot28.p = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        func_80014d58_slot28(o);
        o->field_09 = 3;
        o->field_7a = 0x60;
        o->field_7c = 0x1e0;
        o->pos_x = 0xe0;
        o->field_0d = 0;
        o->pos_y = 0x80;
        func_80130768(o, 2, data_8002bde4_slot28);
        data_8005189c_slot28.p = o;
    }
    o = (Object *)func_8011f1e0();
    if (o != 0) {
        o->field_00 = one;
        o->field_02 = 0xa6;
        o->field_03 = 0;
        data_800518a0_slot28.p = o;
    }
    func_80151020(0x206);
    func_8012818c();
}
