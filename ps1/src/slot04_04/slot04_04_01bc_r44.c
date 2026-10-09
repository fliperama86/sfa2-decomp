/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801c4550_slot04_04[];
extern u8 data_801c4554_slot04_04[];
extern u8 data_801c4590_slot04_04[];
void func_801b3e74_slot04_04(Object *obj);

void func_801b3de4_slot04_04(Object *obj) {
    Object *parent;

    obj->field_a0 = 0xff;
    obj->field_04 = obj->field_04 + 1;
    parent = obj->field_3c;
    obj->field_09 = 0;
    obj->field_49 = parent->field_49;
    obj->field_1c = parent->field_1c;
    ((Slot04bObj *)obj)->field_6c = data_801c4590_slot04_04;
    ((Slot04bObj *)obj)->field_8c = (int)data_801c4554_slot04_04;
    obj->field_45 = 0;
    obj->field_5c = 0xff;
    func_80130700(obj, data_801c4550_slot04_04[0]);
    func_801b3e74_slot04_04(obj);
}
