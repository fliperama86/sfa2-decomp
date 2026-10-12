/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void spawn_effect_at_object_offset(Object *object, u8 a, int dx, int dy);

/* s and d are s16 locals and t an s8 local: with t as an int two instruction
   slots differ, with s as an int the function is 4 bytes shorter, with d
   as an int 8 bytes shorter. */
u8 tick_countdown_spawn_random_effect_slot04_14(Object *obj) {
    Object *o = obj->other;
    u8 r = 0;
    int x = obj->countdown + 0x100;
    s16 s;
    s8 t;
    s16 d;
    x &= 0x3ff;
    obj->countdown = x;
    if ((x & 0x300) == 0) {
        s = get_random_byte() & 0x7f;
        t = get_random_byte();
        d = (t & 0x3f) - 0x60;
        spawn_effect_at_object_offset(obj, 2, d, s);
        if ((get_random_byte() & 1) == 0) {
            play_sound_at(o, o->side, 0x306);
        } else {
            play_sound_at(o, o->side, 0x30f);
        }
        r = 1;
    }
    return r;
}
