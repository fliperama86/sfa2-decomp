/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801e4b40_slot0b[];
void func_801e1598_slot0b(Slot0bObj *obj, int arg);
void func_801e1624_slot0b(Slot0bObj *obj);

void func_801e1144_slot0b(Object *obj) {
    obj->field_0a = 1;
    obj->field_0e = 0;
    obj->ot_depth = 0;
    obj->field_0b = 0;
    obj->field_0c = 0;
    obj->field_24 = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->box_tables = (BoxTables *)data_801e4b40_slot0b;
    obj->state_0 = obj->state_0 + 1;
    obj->ground_y = data_801e4b40_slot0b[0];
    obj->field_46 = 0x1f;
    obj->pos_x = 0x180;
    obj->pos_y = 0x30;
    obj->health = 0xc0;
    obj->field_5e = 0x70;
    obj->vel_x = 0xffe80005;
    obj->accel_x = 0x1294a;
    obj->vel_y = 0x100003;
    obj->accel_y = 0xffff18c6;
    if (obj->field_03 != 0) {
        obj->pos_y = 0xb0;
        obj->vel_x = -obj->vel_x;
        obj->pos_x = 0;
        obj->accel_x = -obj->accel_x;
        obj->vel_y = -obj->vel_y;
        obj->accel_y = -obj->accel_y;
    }
    func_801e1598_slot0b((Slot0bObj *)obj, obj->field_03);
    func_801e1624_slot0b((Slot0bObj *)obj);
}
