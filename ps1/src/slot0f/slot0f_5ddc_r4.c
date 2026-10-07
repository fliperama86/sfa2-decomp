/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Pos data_800efa30_slot0f[];
extern void (*data_800efad8_slot0f[])(Object *);

void func_800e61c4_slot0f(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    u16 *q;
    int i;
    func_80151184();
    i = obj->field_60;
    obj->pos_x = data_800efa30_slot0f[i].x;
    q = &data_800efa30_slot0f[i].y;
    obj->pos_y = *q;
    obj->field_70 = *q;
    obj->pos_x -= 0x10;
    obj->field_62 = 0;
}

void func_800e6240_slot0f(Object *object) {
    data_800efad8_slot0f[object->field_05](object);
}

void func_800e6280_slot0f(Object *obj) {
    if (game_state.field_2bc == 2) {
        obj->field_05++;
    }
}

void func_800e62ac_slot0f(Object *obj) {
}

void func_800e62b4_slot0f(Object *obj) {
}

void func_800e62bc_slot0f(Object *obj) {
}

void func_800e62c4_slot0f(Object *obj) {
}

void func_800e62cc_slot0f(Object *obj) {
}

void func_800e62d4_slot0f(Object *obj) {
}
