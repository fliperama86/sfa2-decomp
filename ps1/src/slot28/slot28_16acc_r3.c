/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051ccc_slot28[];
extern Object *data_80051cd0_slot28[];
extern ObjectRef data_80051cdc_slot28;
extern SequenceStep *data_800502c8_slot28[];
extern SequenceStep *data_800502d4_slot28[];
void func_800272ec_slot28(void);
void func_8002725c_slot28(Object *o, int arg);

/* data_80051ccc_slot28[1] is the word that other units name data_80051cd0_slot28. */
void advance_pass_reposition_slot28_objects_slot28(Object *obj) {
    HudState *h;
    Object *o;
    Object *o2;
    int v;
    if (obj->field_f0 == 0) {
        v = 0x1e0;
        h = data_8018f5a0;
        h->countdown_timer = v;
        h->pass_index++;
        func_800272ec_slot28();
        set_sequence_from_table_5(data_80051ccc_slot28[0], 0, data_800502c8_slot28);
        o = data_80051cd0_slot28[0];
        o->pos_x = 0x38;
        o->pos_y = 0x20;
        o->ot_depth = 4;
        o->field_01 = 1;
        set_sequence_from_table_5(o, 0, data_800502d4_slot28);
        o2 = data_80051ccc_slot28[1];
        o2->pos_x = 0x58;
        o2->pos_y = -0x60;
        o2->spawn_x = 0x10;
        o2->spawn_y = v;
        o2->image_row = 0;
        o2->ot_depth = 5;
        set_sequence_from_table_5(o2, 0, data_800502d4_slot28);
        func_8002725c_slot28(obj, 3);
        start_fade_mode_1a01_and_wait();
    }
}

void advance_pass_on_step_flag_start_music_slot28(Object *obj) {
    Object *o = data_80051cdc_slot28.p;
    if ((s16)o->step_flags < 0) {
        data_8018f5a0->pass_index++;
        set_menu_command_by_code(1, 0x601);
        start_fade_mode_1a01_and_wait();
    }
}
