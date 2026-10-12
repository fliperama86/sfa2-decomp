/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_bar_entry_and_add_prim(Object *object) {
    s16 index = data_801a27d0;
    u8 *base = data_801ac6a8;

    if (object->variant_index_2 >= 0x90) {
        object->variant_index_2 = 0x90;
    }
    if (object->table_index == 0) {
        if (data_80198098 == 0) {
            *(u16 *)&base[(index << 4) + 0x18c] = 0xa6 - object->variant_index_2;
            *(u16 *)&base[(index << 4) + 0x190] = object->variant_index_2;
        } else {
            if (object->variant_index_2 >= 0x48) {
                object->variant_index_2 = 0x48;
            }
            *(u16 *)&base[(index << 4) + 0x18c] = 0xa6 - object->variant_index_2 * 2;
            *(u16 *)&base[(index << 4) + 0x190] = object->variant_index_2 * 2;
        }
        func_8015bf34(data_801987c8 + 0x1c, &base[(index << 4) + 0x184]);
    } else {
        if (data_8019842c == 0) {
            *(u16 *)&base[(index << 4) + 0x1b0] = object->variant_index_2;
        } else {
            if (object->variant_index_2 >= 0x48) {
                object->variant_index_2 = 0x48;
            }
            *(u16 *)&base[(index << 4) + 0x1b0] = object->variant_index_2 * 2;
        }
        func_8015bf34(data_801987c8 + 0x1c, &base[(index << 4) + 0x1a4]);
    }
    func_801347b4(7, object->sequence->data_ptr, object->pos_x, object->pos_y, 7, 0x19);
}
