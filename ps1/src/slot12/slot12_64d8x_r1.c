/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800287b4_slot12[])(Object *);
extern u8 data_8002bfe8_slot12[];
extern u8 data_8002c128_slot12[];
extern u8 data_8002c268_slot12[];
void build_sprite_from_sequence_data_and_link_slot12(Object *obj, Slot12Sprite *cell);

void call_state_1_handler_and_link_sprite_slot12(Object *obj) {
    u8 *base;
    Object *o;
    int t;

    data_800287b4_slot12[obj->state_1](obj);
    switch (obj->variant_index) {
    case 0:
        base = data_8002bfe8_slot12;
        break;
    case 1:
        base = data_8002c128_slot12;
        break;
    case 2:
        base = data_8002c268_slot12;
        break;
    default:
        return;
    }
    o = obj;
    t = o->table_index * 64;
    build_sprite_from_sequence_data_and_link_slot12(o, (Slot12Sprite *)((u8 *)t + (data_801a27d0 * 32 + (int)base)));
}
