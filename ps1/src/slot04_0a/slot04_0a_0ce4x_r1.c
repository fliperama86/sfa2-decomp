/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The local b holds the byte at 0x129, then the value that is stored; a holds the byte at 0x21a. Written with one local for both, this function differs from the original in 7 instruction slots. */
int set_offset_index_from_direction_flags_slot04_0a(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int a = o->direction_flag;
    int b = o->input_dir_second;
    a = (a != 0) << 2;
    if (b != 0) a += 2;
    b = a;
    obj->offset_index = b;
    return 1;
}
