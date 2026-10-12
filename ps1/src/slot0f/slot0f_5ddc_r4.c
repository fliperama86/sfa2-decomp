/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Pos data_800efa30_slot0f[];
extern void (*data_800efad8_slot0f[])(Object *);

void set_position_from_table_entry_slot0f(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    u16 *q;
    int i;
    get_random_byte();
    i = obj->field_60;
    obj->pos_x = data_800efa30_slot0f[i].x;
    q = &data_800efa30_slot0f[i].y;
    obj->pos_y = *q;
    obj->ground_y = *q;
    obj->pos_x -= 0x10;
    obj->field_62 = 0;
}

void call_handler_by_state_1_slot0f(Object *object) {
    data_800efad8_slot0f[object->state_1](object);
}

void func_800e6280_slot0f(Object *obj) {
    if (game_state.field_2bc == 2) {
        obj->state_1++;
    }
}

void noop_slot00_3_slot0f(Object *obj) {
}

void noop_slot00_4_slot0f(Object *obj) {
}

void noop_slot00_5_slot0f(Object *obj) {
}

void noop_slot00_6_slot0f(Object *obj) {
}

void noop_slot00_7_slot0f(Object *obj) {
}

void noop_slot00_8_slot0f(Object *obj) {
}
