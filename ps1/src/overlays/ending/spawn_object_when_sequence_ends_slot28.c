/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern SequenceStep *data_80036690_slot28[];
extern u8 data_80035430_slot28[];
extern u8 data_800357d8_slot28[];
extern ObjectRef data_8005199c_slot28;
extern ObjectRef data_800519a0_slot28;
extern ObjectRef data_800519d0_slot28;
void set_sequence_of_fourth_hud_object_6_slot28(Object *obj, int arg);

void spawn_object_when_sequence_ends_slot28(Object *obj) {
    Object *p;
    p = data_800519d0_slot28.p;
    if ((s16)p->step_flags < 0) {
        HudState *h = screen_state;
        h->pass_index++;
        h->countdown_timer = 0xb4;
        p = data_8005199c_slot28.p;
        p->variant_index = 0xff;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x33;
            p->field_90 = (void *)0x80060000;
            p->data_ptr_a = data_80035430_slot28;
            p->data_ptr_b = data_800357d8_slot28;
            p->pos_x = 0x68;
            p->pos_y = 0x58;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->ot_depth = 3;
            p->table_index = 1;
            p->field_01 = 0;
            p->image_row = 0;
            ((Slot28Obj *)p)->sequence_ptr = data_80036690_slot28 + 6;
            data_800519a0_slot28.p = p;
        }
        set_sequence_of_fourth_hud_object_6_slot28(obj, 1);
        set_menu_command_by_code(6, 1);
    }
}
