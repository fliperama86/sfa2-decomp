/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800252b0_slot28(Object *obj) {
    obj->pos_x = 0xb8;
    obj->pos_y = 0x100;
    obj->field_0c = 0;
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_04++;
    func_80130768(obj, 1, (SequenceStep **)obj->box_tables);
}
