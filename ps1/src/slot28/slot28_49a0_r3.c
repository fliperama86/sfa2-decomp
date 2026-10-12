/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_800518c8_slot28;
extern Object *data_800518bc_slot28[];
extern SequenceStep *data_8002bddc_slot28[];

void set_sequence_of_fourth_hud_object_2_slot28(Object *obj, int arg);
void release_four_stored_objects_slot28(Object *obj);

void start_countdown_when_object_field_negative_slot28(Object *obj) {
    if (((Slot28Obj *)data_800518c8_slot28.p)->field_3a < 0) {
        data_8018f5a0->countdown_timer = 0xc0;
        data_8018f5a0->pass_index = data_8018f5a0->pass_index + 1;
        set_menu_command_by_code(6, 1);
    }
}

void count_down_then_advance_pass_and_call_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}

void func_80015028_slot28(Object *obj) {
    Object *o;
    HudState *h;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->countdown_timer = 0xf0;
        h->pass_index = h->pass_index + 1;
        release_four_stored_objects_slot28(obj);
        o = data_800518bc_slot28[0];
        o->pos_x = 0x50;
        o->pos_y = -0x5f;
        o->spawn_x = 0x10;
        o->spawn_y = 0x1e0;
        o->ot_depth = 2;
        o->image_row = 0;
        o->field_01 = 1;
        set_sequence_from_table_5(o, 0, data_8002bddc_slot28);
        set_sequence_of_fourth_hud_object_2_slot28(obj, 4);
        set_menu_command_by_code(1, 0x402);
        start_fade_mode_1a01_and_wait();
    }
}
