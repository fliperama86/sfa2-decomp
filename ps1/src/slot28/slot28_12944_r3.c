/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8004897c_slot28[];

/* The parameter is not used: its caller spawn_ten_objects_and_advance_pass_slot28 passes the object, and with the call written without the argument that function differs from the original in 5 instruction slots (the HUD pointer lands in the first argument register where the original keeps the object). */
void copy_words_to_two_buffer_copies_slot28(Object *obj) {
    int i = 0;
    u16 *base = (u16 *)data_801a2fe4;
    u16 *b = base + 0xa00;
    u16 *s = data_8004897c_slot28;
    u16 *a = base;
    for (; i < 0x1a0; i++) {
        *a = *s;
        *b++ = *s;
        s++;
        a++;
    }
}
