/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b50e8_slot04_06(Object *obj) {
    GameState *g = &game_state;
    Object *p;

    if (((Slot04aObj *)obj)->mode != 0) {
        ((Slot04aObj *)obj)->mode = 0;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->active_count = 1;
            p->table_index = 5;
            p->parent = obj;
            p->spawn_x = obj->spawn_x;
            p->spawn_y = obj->spawn_y;
            p->image_row = obj->image_row;
            p->field_02 = 0x11;
            p->mode_id = 0x20;
            p->field_90 = obj->field_90;
            p->sound_side = obj->sound_side;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
    if (((Slot04aObj *)obj)->timer != 0) {
        ((Slot04aObj *)obj)->timer--;
        if (((Slot04aObj *)obj)->timer == 0) {
            g->side_bits_2 |= 1 << ((Slot04aObj *)obj)->side;
        }
    }
    update_sequence(obj);
}

void func_801b51f8_slot04_06(Object *obj) {
    int i;
    u8 z = 0;
    u8 *p = (u8 *)&obj->slots;

    for (i = 0x47; i >= 0; i--) {
        *p++ = z;
    }
}
