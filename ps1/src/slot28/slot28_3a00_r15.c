/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002e1d4_slot28[];
extern u8 data_8002e758_slot28[];
extern void (*data_80030664_slot28[])(Object *);
void func_80016e28_slot28(Object *obj, int arg);
extern Object *data_800518fc_slot28[];

void func_80016540_slot28(Object *obj) {
    int one = 1;
    obj->active_count = one;
    obj->field_02 = 0xa5;
    obj->field_90 = (void *)0x80060000;
    obj->data_ptr_a = data_8002e1d4_slot28;
    obj->table_index = 0;
    obj->field_01 = one;
    obj->data_ptr_b = data_8002e758_slot28;
}

void func_8001657c_slot28(Object *obj) {
    data_80030664_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void func_800165d4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    func_80016e28_slot28(obj, 0);
}

void func_80016608_slot28(Object *obj) {
    if (*(s16 *)&data_800518fc_slot28[3]->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
