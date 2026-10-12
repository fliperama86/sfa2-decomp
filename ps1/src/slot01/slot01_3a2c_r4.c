/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern int data_80055f3c_slot01;
extern s16 data_80055e9a_slot01;
void func_80012020_slot01(Object *obj);

void func_80013da4_slot01(Object *obj) {
    Object *p;
    if (obj->field_60 != 0) {
        advance_sequence_checked(obj);
        func_8011ffdc(obj);
    } else if (obj->state_1 != 0) {
        if (game_state.field_04 != 0) {
            obj->state_0++;
        }
        func_8011ffdc(obj);
    } else if (data_80055e9a_slot01 < 0) {
        obj->state_1++;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x30;
            p->field_01 = 1;
            p->field_0c = 0;
            p->image_row = 0;
            p->field_60 = 1;
            p->spawn_x = 0x20;
            p->spawn_y = 0x1e8;
            p->data_ptr_a = data_80019330_slot01;
            p->field_90 = (void *)0x80059000;
            p->data_ptr_b = data_800195b8_slot01;
            p->table_index = *(u8 *)&data_80055f3c_slot01;
        }
    } else {
        func_80012020_slot01(obj);
    }
}
