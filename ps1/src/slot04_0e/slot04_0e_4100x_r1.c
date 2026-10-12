/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"
#include "../protos.h"

extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];
extern void func_801b420c_slot04_0e(Object *obj);

/* What it does (inferred, not an original name): a character action handler
   that asks for a free object slot and, if it gets one, fills it as an
   effect object tied to the object's partner (obj->other): the slot copies
   the partner's field_0e and field_0b and takes the partner's ground_y as
   its pos_y, its pos_x is the object's plus the high half of offset_x of
   the record that the object's side selects, and field_3c points at the
   partner; it then calls play_sound_at for the partner. In every case it
   ends by calling func_801b420c_slot04_0e on the object. */
void func_801b4100_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[obj->side];
    Object *s = (Object *)func_8011f1e0();
    Object *o;
    u8 side;

    if (s != 0) {
        o = obj->other;
        s->field_00 = 1;
        s->field_02 = 4;
        s->field_03 = 6;
        s->field_0e = o->field_0e;
        s->field_0b = o->field_0b;
        /* Stand-in, not recovered source: the cast that reads the record's offset_x as volatile reproduces the original's order of instructions. Without it this function differs from the original in 5 instruction slots. */
        /* The original loads the whole word and shifts it in a later instruction; with a plain read the compiler makes one load of the high half. */
        s->pos_x = obj->pos_x + (*(volatile s32 *)&rec->offset_x >> 16);
        s->pos_y = o->ground_y;
        s->field_48 = 1;
        side = obj->field_65;
        s->spawn_x = 0x60;
        s->field_3c = o;
        s->spawn_y = 0x1e0;
        s->field_65 = side;
        s->field_0d = obj->field_0d;
        s->field_90 = obj->field_90;
        s->field_98 = obj->field_98;
        s->field_9c = obj->field_9c;
        play_sound_at(o, o->side, 0x312);
    }
    func_801b420c_slot04_0e(obj);
}
