/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051a98_slot28[];
extern ObjectRef data_80051ac0_slot28;
extern Object *data_80051ac4_slot28[];
extern SequenceStep *data_8003f084_slot28[];
extern SequenceStep *data_8003f090_slot28[];
extern SequenceStep *data_8003f09c_slot28[];
void func_8001ef04_slot28(void);
void func_8001eea4_slot28(Object *obj);
void func_8001ee74_slot28(Object *obj, int arg);

void advance_slot28_pass_if_flag_to_step_two_slot28(Object *obj) {
    Object *b;
    if (data_80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index++;
        func_8001ef04_slot28();
        set_sequence_from_table_5(data_80051ac0_slot28.p, 2, data_8003f084_slot28);
        b = data_80051ac4_slot28[0];
        b->pos_x = 0x78;
        b->pos_y = 0x20;
        b->field_01 = 1;
        set_sequence_from_table_5(b, 1, data_8003f090_slot28);
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            func_8001eea4_slot28(b);
            b->pos_x = 0xb0;
            b->pos_y = 0xa0;
            b->spawn_x = 0x60;
            b->spawn_y = 0x1e0;
            b->image_row = 0;
            b->ot_depth = 3;
            set_sequence_from_table_5(b, 6, data_8003f09c_slot28);
            data_80051a98_slot28[0] = b;
        }
        func_8001ee74_slot28(obj, 2);
        start_fade_mode_1a01_and_wait();
    }
}
