/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_80029ed8_slot28[];
extern SequenceStep *data_80029ee4_slot28[];
extern Object *data_8005187c_slot28[];
void set_sequence_of_fourth_hud_object_slot28(Object *obj, int arg);

void advance_pass_and_set_sequences_2_slot28(Object *obj) {
    HudState *h;
    Object *o;
    Object *q;
    if (obj->field_f0 == 0) {
        h = screen_state;
        h->countdown_timer = 0x258;
        h->pass_index += 1;
        o = data_8005187c_slot28[0];
        set_sequence_from_table_5(o, 0, data_80029ed8_slot28);
        o->pos_y = -0x60;
        o = data_8005187c_slot28[1];
        o->pos_y = -0x60;
        o = data_8005187c_slot28[2];
        q = o;
        q->pos_x = 0xc0;
        q->pos_y = 0x90;
        set_sequence_from_table_5(q, 2, data_80029ee4_slot28);
        set_sequence_of_fourth_hud_object_slot28(obj, 2);
        start_fade_mode_1a01_and_wait();
    }
}

void advance_pass_and_set_sequences_3_slot28(Object *obj) {
    if (obj->field_f0 == 0 && (s16)data_8005187c_slot28[3]->step_flags < 0) {
        screen_state->pass_index += 1;
        set_sequence_from_table_5(data_8005187c_slot28[2], 3, data_80029ee4_slot28);
        set_sequence_of_fourth_hud_object_slot28(obj, 3);
    }
}
