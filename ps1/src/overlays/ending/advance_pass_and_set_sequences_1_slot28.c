/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051878_slot28;
extern Object *data_8005187c_slot28[];
extern SequenceStep *data_80029ed0_slot28[];
extern SequenceStep *data_80029ed8_slot28[];
extern SequenceStep *data_80029ee4_slot28[];
void set_sequence_of_fourth_hud_object_slot28(Object *obj, int arg);

void advance_pass_and_set_sequences_1_slot28(Object *obj) {
    HudState *h;
    Object *o;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        h->countdown_timer = 0x258;
        set_sequence_from_table_5(data_80051878_slot28.p, 0, data_80029ed0_slot28);
        o = data_8005187c_slot28[0];
        o->pos_y = 0x20;
        set_sequence_from_table_5(o, 1, data_80029ed8_slot28);
        o = data_8005187c_slot28[2];
        o->pos_x = 0xc0;
        o->pos_y = 0x90;
        set_sequence_from_table_5(o, 1, data_80029ee4_slot28);
        set_sequence_of_fourth_hud_object_slot28(obj, 1);
        start_fade_mode_1a01_and_wait();
    }
}
