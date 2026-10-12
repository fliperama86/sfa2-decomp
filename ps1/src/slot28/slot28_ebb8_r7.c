/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051b00_slot28[];
extern Object *data_80051b04_slot28[];
extern ObjectRef data_80051b08_slot28;
extern Object *data_80051ad8_slot28[];
extern SequenceStep *data_80041498_slot28[];
extern SequenceStep *data_800414a4_slot28[];
extern SequenceStep *data_800414d4_slot28[];
extern u8 data_8003f9fc_slot28[];
extern u8 data_8003feb8_slot28[];
void func_800200ec_slot28(void);
void func_8002005c_slot28(Object *obj, int arg);

void func_8001f698_slot28(Object *obj) {
    HudState *h;
    Object *p;
    int x;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 0x258;
        h->pass_index++;
        func_800200ec_slot28();
        set_sequence_from_table_5(data_80051b00_slot28[0], 2, data_80041498_slot28);
        x = 0x38;
        p = data_80051b04_slot28[0];
        p->pos_x = x;
        p->pos_y = 0x20;
        set_sequence_from_table_5(p, 2, data_800414a4_slot28);
        p = data_80051b08_slot28.p;
        p->pos_x = x;
        p->pos_y = -0x60;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x71;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_8003f9fc_slot28;
            p->data_ptr_b = data_8003feb8_slot28;
            p->pos_x = 0x40;
            p->pos_y = 0xcc;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 2;
            p->table_index = 0;
            p->field_01 = 0;
            p->image_row = 0;
            data_80051ad8_slot28[0] = p;
            ((Slot28Obj *)p)->sequence_ptr = data_800414d4_slot28;
        }
        play_sound_at(0, 0, 0x304);
        start_fade_mode_1a01_and_wait();
        func_8002005c_slot28(obj, 1);
    }
}
