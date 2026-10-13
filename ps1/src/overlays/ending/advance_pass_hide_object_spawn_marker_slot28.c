/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051b04_slot28[];
extern ObjectRef data_80051b08_slot28;
void set_sequence_of_fourth_hud_object_11_slot28(Object *obj, int arg);

void advance_pass_hide_object_spawn_marker_slot28(Object *obj) {
    HudState *h;
    Object *b;

    if (obj->field_f0 == 0) {
        h = screen_state;
        h->countdown_timer = 0x258;
        h->pass_index++;
        b = data_80051b04_slot28[0];
        b->field_01 = 0;
        b = data_80051b08_slot28.p;
        b->pos_x = 0x58;
        b->pos_y = -0x60;
        b = (Object *)pop_block_from_pool();
        if (b != 0) {
            b->active_count = 1;
            b->field_02 = 0xa6;
            b->table_index = 9;
            b->field_01 = 0;
        }
        set_sequence_of_fourth_hud_object_11_slot28(obj, 4);
        start_fade_mode_1a01_and_wait();
    }
}
