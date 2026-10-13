/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

int try_spawn_pooled_object_once_with_arg_slot01(u8 arg) {
    int ret = 0;
    Object *p;
    u8 *flag = &single_object_active;

    if (*flag == 0) {
        *flag = 1;
        data_801ae02e = arg;
        data_801ae02d = 0;
        p = (Object *)pop_block_from_pool();
        if (p != 0) {
            ret = 1;
            p->active_count = 1;
            p->field_02 = 0x13;
            p->table_index = arg + 0x80;
        }
    }
    return ret;
}
