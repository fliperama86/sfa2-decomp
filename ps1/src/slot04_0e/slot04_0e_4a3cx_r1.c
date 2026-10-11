/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"
#include "../protos.h"

extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

/* What it does (inferred, not an original name): a character action handler
   that, when the low three bits of game_state.field_1d are 0, asks for a
   free object slot and, if it gets one, fills it as an effect object that
   follows this one: field_3c points back at the object, pos_x is the
   object's plus the high half of field_10 of the record that the object's
   side selects, pos_y is the object's field_70. It fills the slot as
   func_801b420c_slot04_0e does, with field_48 0x4e where that one has 0x4b,
   and writes nothing to the object itself. */
void func_801b4a3c_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];
    Object *c;

    if ((game_state.field_1d & 7) == 0) {
        c = (Object *)func_8011f1e0();
        if (c != 0) {
            c->field_00 = 1;
            c->field_02 = 0x3b;
            c->field_03 = 1;
            c->field_09 = 0;
            c->field_02 = 0x14;
            /* Stand-in, not recovered source: the cast that reads the record's field_10 as volatile reproduces the original's order of instructions. Without it this function differs from the original in 5 instruction slots. */
            /* The original loads the whole word and shifts it in a later instruction; with a plain read the compiler makes one load of the high half. */
            c->pos_x = obj->pos_x + (*(volatile s32 *)&r->field_10 >> 16);
            c->pos_y = obj->field_70;
            c->field_48 = 0x4e;
            c->field_7a = 0x60;
            c->field_3c = obj;
            c->field_7c = 0x1e0;
            c->field_0d = obj->field_0d;
            c->field_08 = 0x20;
            c->field_90 = obj->field_90;
            c->field_66 = obj->field_66;
            c->field_98 = obj->field_98;
            c->field_9c = obj->field_9c;
        }
    }
}
