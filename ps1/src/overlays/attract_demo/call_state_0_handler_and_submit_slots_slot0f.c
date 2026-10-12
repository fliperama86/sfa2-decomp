/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800f012c_slot0f[])(Object *);
extern u8 data_800f7d34_slot0f;
extern u8 data_800f7d38_slot0f[];
extern u8 data_800f7d48_slot0f[];
extern Slot12Quad data_800f7d50_slot0f[];
extern Slot12Cell data_800f7e90_slot0f[];
void fill_sprite_from_sequence_and_link_slot0f(Object *obj, Slot12Sprite *cell);

void call_state_0_handler_and_submit_slots_slot0f(Object *obj) {
    int i;
    int k;
    data_80190468.p = (Object *)&game_state;
    data_800f012c_slot0f[obj->state_0](obj);
    k = obj->table_index;
    if (k == 5) {
        i = 0;
        if (data_800f7d34_slot0f == 1) {
            add_item_to_table(data_800f7d38_slot0f);
        }
        for (i = 0; i < 5; i++) {
            if (data_800f7d48_slot0f[i] == 1) {
                add_item_to_table(&data_800f7d50_slot0f[i].cells[0]);
                add_item_to_table(&data_800f7d50_slot0f[i].cells[1]);
                add_item_to_table(&data_800f7d50_slot0f[i].cells[2]);
                if (game_state.mode_flag_2 == 1) {
                    add_item_to_table(&data_800f7d50_slot0f[i].cells[3]);
                }
            }
        }
    } else if (data_800f7d48_slot0f[k] == 1) {
        fill_sprite_from_sequence_and_link_slot0f(obj, (Slot12Sprite *)(data_800f7e90_slot0f + k * 4 + data_801a27d0 * 2));
    }
}
