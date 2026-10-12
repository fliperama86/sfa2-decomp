/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051a08_slot28;
extern Object *data_80051a04_slot28[];
extern Object *data_800519d8_slot28[];
extern SequenceStep *data_80039290_slot28[];
extern SequenceStep *data_8003929c_slot28[];

void func_8001c460_slot28(Object *obj);
void func_8001c4c0_slot28(void);

void advance_pass_and_spawn_six_scene_objects_slot28(Object *obj) {
    Object *p;
    if (obj->field_f0 == 0) {
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        func_8001c4c0_slot28();
        p = data_80051a08_slot28.p;
        p->field_01 = 0;
        p = data_80051a04_slot28[0];
        set_sequence_from_table_5(p, 1, data_80039290_slot28);
        p->field_01 = 1;
        p->ot_depth = 5;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            func_8001c460_slot28(p);
            p->pos_x = 0xe0;
            p->pos_y = 0x100;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 2, data_8003929c_slot28);
            data_800519d8_slot28[0] = p;
            p->ot_depth = 2;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            func_8001c460_slot28(p);
            p->pos_x = 0x90;
            p->pos_y = 0xd0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 2, data_8003929c_slot28);
            data_800519d8_slot28[1] = p;
            p->ot_depth = 2;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            func_8001c460_slot28(p);
            p->pos_x = 0xe0;
            p->pos_y = 0xe0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 2, data_8003929c_slot28);
            data_800519d8_slot28[2] = p;
            p->ot_depth = 2;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            func_8001c460_slot28(p);
            p->pos_x = 0xb0;
            p->pos_y = 0xf0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 2, data_8003929c_slot28);
            data_800519d8_slot28[3] = p;
            p->ot_depth = 2;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            func_8001c460_slot28(p);
            p->pos_x = 0xb0;
            p->pos_y = 0xd0;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 2, data_8003929c_slot28);
            data_800519d8_slot28[4] = p;
            p->ot_depth = 2;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            func_8001c460_slot28(p);
            p->pos_x = 0xc8;
            p->pos_y = 0x120;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 1, data_8003929c_slot28);
            data_800519d8_slot28[5] = p;
            p->ot_depth = 2;
        }
        start_fade_mode_2001_no_wait();
    }
}
