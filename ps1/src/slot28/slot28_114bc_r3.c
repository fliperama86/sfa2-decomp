/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051b8c_slot28;
extern Object *data_80051b90_slot28[];
extern ObjectRef data_80051b94_slot28;
extern Object *data_80051b64_slot28[];
extern SequenceStep *data_80045c40_slot28[];
extern SequenceStep *data_80045c58_slot28[];
void func_80021d64_slot28(Object *obj, int arg);
void func_80021d94_slot28(Object *obj);
void func_80021df4_slot28(void);

void advance_slot28_pass_to_step_two_c_slot28(Object *obj) {
    Object *b;
    if (obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        func_80021df4_slot28();
        set_sequence_from_table_5(data_80051b8c_slot28.p, 2, data_80045c40_slot28);
        b = data_80051b90_slot28[0];
        b->pos_y = -0x60;
        b = data_80051b94_slot28.p;
        b->field_01 = 0;
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            func_80021d94_slot28(b);
            b->pos_x = 0xb8;
            b->pos_y = 0x70;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 3;
            set_sequence_from_table_5(b, 2, data_80045c58_slot28);
            data_80051b64_slot28[0] = b;
        }
        func_80021d64_slot28(obj, 2);
        start_fade_mode_1a01_and_wait();
    }
}
