/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e4b40_slot0b[];
void func_801e1598_slot0b(Slot0bObj *obj, u8 index);
void func_801e1624_slot0b(Slot0bObj *obj);

void func_801e1144_slot0b(Object *obj) {
    obj->field_0a = 1;
    obj->field_0e = 0;
    obj->field_09 = 0;
    obj->field_0b = 0;
    obj->field_0c = 0;
    obj->field_24 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->box_tables = (BoxTables *)data_801e4b40_slot0b;
    obj->field_04 = obj->field_04 + 1;
    obj->field_70 = data_801e4b40_slot0b[0];
    obj->field_46 = 0x1f;
    obj->pos_x = 0x180;
    obj->pos_y = 0x30;
    obj->field_5c = 0xc0;
    obj->field_5e = 0x70;
    obj->field_4c = 0xffe80005;
    obj->field_54 = 0x1294a;
    obj->field_50 = 0x100003;
    obj->field_58 = 0xffff18c6;
    if (obj->field_03 != 0) {
        obj->pos_y = 0xb0;
        obj->field_4c = -obj->field_4c;
        obj->pos_x = 0;
        obj->field_54 = -obj->field_54;
        obj->field_50 = -obj->field_50;
        obj->field_58 = -obj->field_58;
    }
    func_801e1598_slot0b((Slot0bObj *)obj, obj->field_03);
    func_801e1624_slot0b((Slot0bObj *)obj);
}
