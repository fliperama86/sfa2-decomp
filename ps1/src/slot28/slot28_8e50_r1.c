/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051980_slot28[];
extern ObjectRef data_80051984_slot28;
extern ObjectRef data_80051994_slot28;
extern Object *data_80051954_slot28[];
extern SequenceStep *data_80034520_slot28[];
extern SequenceStep *data_80034528_slot28[];
extern SequenceStep *data_80034530_slot28[];
void func_80019190_slot28(void);
void func_80019130_slot28(Object *obj);
void func_80019100_slot28(Object *obj, int arg);

void start_pass_reposition_objects_on_flag_slot28(Object *obj) {
    Object *o;
    Object *n;
    int one;
    int x60;

    if (data_80190949 != 0 && obj->field_f0 == 0) {
        data_8018f5a0->countdown_timer = 0x258;
        data_8018f5a0->pass_index++;
        func_80019190_slot28();
        x60 = 0x60;
        o = data_80051980_slot28[0];
        o->pos_x = x60;
        o->pos_y = -0x60;
        set_sequence_from_table_5(o, 1, data_80034520_slot28);
        o->pos_x -= 1;
        o = data_80051994_slot28.p;
        o->field_01 = 0;
        o = data_80051984_slot28.p;
        o->pos_x = 0x80;
        o->pos_y = 0x20;
        set_sequence_from_table_5(o, 1, data_80034528_slot28);
        one = 1;
        o->field_01 = one;
        n = (Object *)pop_block_from_pool();
        if (n != 0) {
            func_80019130_slot28(n);
            n->pos_x = 0xc8;
            n->pos_y = 0xa0;
            n->spawn_x = x60;
            n->spawn_y = 0x1e0;
            n->image_row = 0;
            n->ot_depth = one;
            n->variant_index = 0;
            set_sequence_from_table_5(n, 9, data_80034530_slot28);
            data_80051954_slot28[0] = n;
        }
        start_fade_mode_1a01_and_wait();
        func_80019100_slot28(obj, 7);
    }
}
