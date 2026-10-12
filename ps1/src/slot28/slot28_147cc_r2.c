/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80051c4c_slot28;
extern Object *data_80051c50_slot28[];
extern Object *data_80051c24_slot28[];
extern ObjectRef data_80051c28_slot28;
extern SequenceStep *data_8004bd74_slot28[];
extern SequenceStep *data_8004bd84_slot28[];
void func_80025114_slot28(Object *o);
void func_800250e4_slot28(Object *o, int arg);

void init_pass_objects_two_spawned_slot28(Object *obj) {
    HudState *h;
    Object *p;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 0x258;
        h->pass_index++;
        set_sequence_from_table_5(data_80051c4c_slot28.p, 0, data_8004bd74_slot28);
        p = data_80051c50_slot28[0];
        p->pos_y = -0x60;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0xa6;
            p->table_index = 0xb;
            data_80051c28_slot28.p = p;
        }
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            func_80025114_slot28(p);
            p->pos_x = 0xb8;
            p->pos_y = 0x90;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = 0;
            p->ot_depth = 2;
            set_sequence_from_table_5(p, 0, data_8004bd84_slot28);
            data_80051c24_slot28[0] = p;
        }
        func_800250e4_slot28(obj, 1);
        start_fade_mode_1a01_and_wait();
    }
}
