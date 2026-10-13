/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_800519d8_slot28[];
extern Object *data_80051a04_slot28[];
extern ObjectRef data_80051a08_slot28;
void set_sequence_of_fourth_hud_object_7_slot28(Object *obj, int arg);

void next_pass_and_set_sequence_if_field_set_slot28(Object *obj) {
    if (obj->field_f0 != 0) {
        screen_state->pass_index = screen_state->pass_index + 1;
        set_sequence_of_fourth_hud_object_7_slot28(obj, 1);
    }
}

void scroll_objects_up_until_limit_slot28(Object *obj) {
    Object *o;
    int i;
    Object **q;
    i = 0;
    q = data_800519d8_slot28;
    while (i < 6) {
        o = *q++;
        ((Slot28Obj *)o)->pos_y -= 0x4000;
        i++;
    }
    o = data_80051a08_slot28.p;
    ((Slot28Obj *)o)->pos_y -= 0x4000;
    o = data_80051a04_slot28[0];
    ((Slot28Obj *)o)->pos_y -= 0x4000;
    if (o->pos_y < -0x6f) {
        screen_state->countdown_timer = 0x258;
        screen_state->pass_index = screen_state->pass_index + 1;
        o->pos_y = -0x70;
        set_sequence_of_fourth_hud_object_7_slot28(obj, 2);
    }
}
