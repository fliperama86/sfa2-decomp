/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800518d0_slot28[];
extern ObjectRef data_80051908_slot28;
extern SequenceStep *data_8002fca8_slot28[];
void set_sequence_of_fourth_hud_object_3_slot28(Object *obj, int arg);

void advance_pass_set_final_sequences_on_object_done_slot28(Object *o) {
    Object *p = data_80051908_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        p = (Object *)data_8018f5a0;
        ((HudState *)p)->countdown_timer = 0x12c;
        ((HudState *)p)->pass_index++;
        p = data_800518d0_slot28[0];
        p->variant_index = 0xff;
        if ((((Slot28Obj *)o)->state_index & 0x7f) == 4) {
            set_sequence_from_table_5(p, 0xc, data_8002fca8_slot28);
        } else {
            set_sequence_from_table_5(p, 0xe, data_8002fca8_slot28);
        }
        p = data_800518d0_slot28[1];
        p->variant_index = 0xff;
        if ((((Slot28Obj *)o)->state_index & 0x7f) == 4) {
            set_sequence_from_table_5(p, 0xd, data_8002fca8_slot28);
        } else {
            set_sequence_from_table_5(p, 0xf, data_8002fca8_slot28);
        }
        set_sequence_of_fourth_hud_object_3_slot28(o, 5);
    }
}

void fade_and_advance_pass_when_object_done_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051908_slot28.p)->field_3a < 0) {
        set_menu_command_by_code(6, 3);
        start_fade_mode_1_and_wait();
        data_8018f5a0->pass_index++;
    }
}
