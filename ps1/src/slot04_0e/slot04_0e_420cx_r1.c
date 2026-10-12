/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../externs.h"
#include "../protos.h"

extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

/* What it does (inferred, not an original name): a character action handler
   that sets the object's state_3 to 4, asks for a free object slot and, if
   it gets one, fills it as an effect object that follows this one: field_3c
   points back at the object, pos_x is the object's plus the high half of
   offset_x of the record that the object's side selects, pos_y is the
   object's ground_y. It ends by calling func_801307e0 on the object with
   0x49. */
void func_801b420c_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *rec;
    Object *p;

    obj->state_3 = 4;
    rec = &data_801c62b4_slot04_0e[obj->side];
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x3b;
        p->field_03 = 1;
        p->ot_depth = 0;
        p->field_02 = 0x14;
        /* Stand-in, not recovered source: the cast that reads the record's offset_x as volatile reproduces the original's order of instructions. Without it this function differs from the original in 5 instruction slots. */
        /* The original loads the whole word and shifts it in a later instruction; with a plain read the compiler makes one load of the high half. */
        p->pos_x = obj->pos_x + (*(volatile s32 *)&rec->offset_x >> 16);
        p->pos_y = obj->ground_y;
        p->field_48 = 0x4b;
        p->field_3c = obj;
        p->spawn_x = 0x60;
        p->spawn_y = 0x1e0;
        p->field_0d = obj->field_0d;
        p->field_08 = 0x20;
        p->field_90 = obj->field_90;
        p->field_66 = obj->field_66;
        p->field_98 = obj->field_98;
        p->field_9c = obj->field_9c;
    }
    func_801307e0(obj, 0x49);
}
