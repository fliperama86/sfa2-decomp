/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Direction tables of the module: bytes, then two tables of words. Inferred
   from the code; not original declarations. */
extern u8 data_801c6190_slot04_0e[];
extern s32 data_801c6194_slot04_0e[];
extern s32 data_801c6198_slot04_0e[];

/* What it does (inferred, not an original name): the start of a character
   action. It advances the object's state byte, takes 2 points off a
   counter (func_80141f28), plays two sounds (play_side_sound_at,
   play_sound_at), clears two velocity words, and sets a launch velocity
   from tables indexed by the object's direction byte field_48: a byte
   table (index field_48 / 2) gives field_47, two word tables (index
   field_48) give the velocity words vel_x and accel_x, both negated
   when field_0b is not 0. It then selects the sequence 0x27 + field_48 / 2. */
void func_801b5e34_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 i;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int t;

    o->state_3++;
    func_80141f28(o, 2);
    /* The local t holds the side here and the halved direction at the end: one local for two values. With the halved direction written at the last call this function differs from the original in 3 instruction slots: the value is then tied to the argument register that the sum goes to. */
    t = o->side;
    play_side_sound_at(o, t, 6);
    play_sound_at(o, o->field_02, 0x324);
    o->vel_y = 0;
    o->accel_y = 0;
    obj->field_46 = 7;
    obj->field_47 = data_801c6190_slot04_0e[o->field_48 >> 1];
    i = o->field_48;
    o->vel_x = data_801c6194_slot04_0e[i];
    o->accel_x = data_801c6198_slot04_0e[i];
    if (o->field_0b != 0) {
        o->vel_x = -o->vel_x;
        o->accel_x = -o->accel_x;
    }
    t = o->field_48 >> 1;
    func_801307e0(o, t + 0x27);
}
