/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

typedef void (*SlotFn)(Slot0bObj *);

extern SlotFn table_801e4ce0_slot0b[];
extern u8 data_801e7a2c_slot0b[];
void build_sprite_set_quads_into_ot_slot0b(Object *obj, u8 *p);

void update_object_by_state_1_and_draw_slot0b(Object *obj) {
    if (ref_game_state.p->state_0 == 0) {
        table_801e4ce0_slot0b[obj->state_1]((Slot0bObj *)obj);
        if (data_80190a40 != 1) {
            build_sprite_set_quads_into_ot_slot0b(obj, data_801e7a2c_slot0b + obj->table_index * 0x50 + draw_buffer_index * 0x28);
        }
    } else {
        obj->state_0 = 2;
    }
}
