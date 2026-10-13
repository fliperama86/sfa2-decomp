/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80023170_slot12[])(Object *);
void call_handler_by_state_0_with_tile_update_slot12(Object *obj);

/* v is not set on every path: when the flag at single_object_active is already set,
   the call at the end passes the register as it is. Reading the unset local
   is undefined behaviour in C. It reproduces the original's instructions
   with this compiler and is not a defined implementation: a port has to
   give v a value on that path. */
int spawn_object_once_with_arg_slot12(u8 arg) {
    Object *v;
    int r;
    u8 *flag = &single_object_active;

    r = 0;
    if (*flag == 0) {
        *flag = 1;
        data_801ae02e = arg;
        data_801ae02d = 0;
        v = (Object *)pop_block_from_pool();
        if (v != 0) {
            v->active_count = 1;
            r = 1;
            v->field_02 = 0x7e;
            v->table_index = arg + 0x80;
        }
    }
    call_handler_by_state_0_with_tile_update_slot12(v);
    return r;
}
