/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017c28_slot27[];
extern u8 data_8001aa14_slot27[];
extern s16 data_8002b386_slot27;
void func_80016810_slot27(Object *obj);

void func_80015ca4_slot27(Object *obj) {
    Object *b;
    if (obj->field_60 != 0) {
        advance_sequence_checked(obj);
        func_8011ffdc(obj);
    } else if (obj->state_1 != 0) {
        if (game_state.field_04 != 0) {
            obj->state_0++;
        }
        func_8011ffdc(obj);
    } else if (data_8002b386_slot27 < 0) {
        obj->state_1++;
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0xaa;
            b->field_01 = 1;
            b->field_60 = 1;
            b->table_index = game_state.kind_index;
            b->field_90 = (void *)0x80038000;
            b->data_ptr_a = data_80017c28_slot27;
            b->data_ptr_b = data_8001aa14_slot27;
            b->spawn_x = 0;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
        }
    } else {
        func_80016810_slot27(obj);
    }
}
