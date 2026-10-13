/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_80036690_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519a4_slot28;
extern ObjectRef data_800519d0_slot28;
void set_sequence_of_fourth_hud_object_6_slot28(Object *obj, int arg);
void init_object_data_pointers_a_6_slot28(Object *obj);

void advance_pass_and_spawn_sequence_7_object_slot28(Object *obj) {
    Object *o = data_800519d0_slot28.p;
    if ((s16)o->step_flags < 0) {
        HudState *h = screen_state;
        h->pass_index++;
        h->countdown_timer = 0xb4;
        o = data_8005199c_slot28.p;
        o->variant_index = 0xff;
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            init_object_data_pointers_a_6_slot28(o);
            o->pos_x = 0x98;
            o->pos_y = 0x48;
            o->spawn_x = 0x60;
            o->spawn_y = 0x1e0;
            o->image_row = 0;
            o->ot_depth = 2;
            set_sequence_from_table_5(o, 7, data_80036690_slot28 + 6);
            data_800519a4_slot28.p = o;
        }
        set_sequence_of_fourth_hud_object_6_slot28(obj, 2);
    }
}
