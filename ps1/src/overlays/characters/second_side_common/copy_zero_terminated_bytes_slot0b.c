/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern HudSlot data_801e7648_slot0b[];
extern void (*data_801e4d50_slot0b[])(Object *);
void set_hud_slot_position_from_object_slot0b(Object *obj, Object *other);

void copy_zero_terminated_bytes_slot0b(u8 *src, u8 *dst) {
    if (*src != 0) {
        do {
            *dst = *src;
            src++;
            dst++;
        } while (*src != 0);
        *dst = 0;
    }
}

void set_hud_slot_position_from_object_slot0b(Object *obj, Object *other) {
    HudSlot *s = &data_801e7648_slot0b[other->side];
    s->pos_x = obj->pos_x - 0x38;
    s->pos_y = obj->pos_y - 8;
}

void update_hud_slot_by_state_1_slot0b(Object *obj) {
    Object *o = obj->parent;
    if (ref_game_state.p->state_0 == 0) {
        data_801e4d50_slot0b[obj->state_1](obj);
        set_hud_slot_position_from_object_slot0b(obj, o);
        if (data_80190a40 != 1) {
            add_item_to_table(&data_801e7648_slot0b[o->side]);
        }
    } else {
        obj->state_0 = 2;
    }
}
