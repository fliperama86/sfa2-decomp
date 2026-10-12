/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8003b500_slot28[];
extern SequenceStep *data_8003b504_slot28[];
extern SequenceStep *data_8003b530_slot28[];
extern ObjectRef data_80051a50_slot28;
extern ObjectRef data_80051a54_slot28;
extern ObjectRef data_80051a40_slot28;
void func_8001d578_slot28(Object *obj);
void func_8001d5b4_slot28(void);

void init_slot28_scene_objects_step_2_slot28(Object *obj) {
    int one;
    Object *p;
    Object *b;
    data_8018f5a0->countdown_timer = 0xb4;
    data_8018f5a0->step_index_2 += 1;
    func_80151020(0x305);
    one = 1;
    func_8001d5b4_slot28();
    start_fade_mode_1_and_wait();
    data_80190568 = one;
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        func_8001d578_slot28(p);
        data_80051a54_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        set_sequence_from_table_5(p, 0, data_8003b500_slot28);
        p->field_01 = one;
    }
    p = (Object *)pop_block_from_pool();
    if (p != 0) {
        func_8001d578_slot28(p);
        data_80051a40_slot28.p = p;
        p->pos_x = -8;
        p->pos_y = -8;
        p->spawn_x = 0;
        p->spawn_y = 0x1e0;
        p->image_row = 0;
        p->ot_depth = one;
        set_sequence_from_table_5(p, 1, data_8003b504_slot28);
        p->field_01 = one;
    }
    p = (Object *)pop_block_from_pool();
    b = p;
    if (b != 0) {
        func_8001d578_slot28(b);
        data_80051a50_slot28.p = b;
        b->spawn_y = 0x1e0;
        b->pos_x = 0x28;
        b->spawn_x = 0;
        b->image_row = 0;
        b->table_index = one;
        b->ot_depth = 0;
        b->vel_y = 0xb8;
        b->field_01 = one;
        set_sequence_from_table_5(b, 0, data_8003b530_slot28);
    }
    start_fade_mode_c01_and_wait();
}
