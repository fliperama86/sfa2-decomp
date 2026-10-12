/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* One local, s, holds the result of the first test and later the shifted
   halfword: with the test written in the if and s used for the shift only,
   this function differs from the original in 6 instruction slots. The
   halfword is read into t and copied to v: with v = *p; it differs in 5.
   The shifted value goes through t to the store: with
   object->script_opcode_high = s >> 24; it differs in 5. The pointer is read back
   from data_80189460: with p = (u16 *)object->script_ptr_1; it differs in 5. */
void enter_script_mode_1_and_read_opcode(Object *object) {
    u16 *p;
    int t;
    int v;
    int s;
    s = object->script_mode == 0;
    if (s) {
        init_script_from_table_index(object);
    }
    object->script_mode = 1;
    start_random_script_from_table_index(object);
    data_80189460 = (u16 *)object->script_ptr_1;
    object->script_ptr = (s32)data_80189460;
    p = data_80189460;
    data_80189460 = p + 1;
    t = *p;
    v = t;
    s = v << 16;
    t = s >> 24;
    object->script_opcode_high = t;
    object->script_arg = v;
}
