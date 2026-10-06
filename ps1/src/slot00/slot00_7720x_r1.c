/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80079fec_slot00[];
extern u8 data_8007a008_slot00[];
void func_8011a55c(Object *obj);

void func_80077720_slot00(Object *obj) {
    Object *p;
    obj->field_09 = 2;
    obj->field_50 = 0;
    obj->field_5c = 0xfff;
    obj->field_50 = 0x20;
    obj->field_58 = 0xff;
    obj->field_a0 = 0xff;
    obj->field_04++;
    obj->field_54 = obj->field_0d;
    p = obj->field_3c;
    obj->field_67 = 0;
    obj->field_60 = 0;
    obj->field_6b = 0;
    obj->field_1c = p->field_1c;
    obj->field_1e = p->field_1e;
    ((Slot00Obj *)obj)->field_a4 += *(s16 *)(data_8007a008_slot00 + (obj->field_ac & 0xfe));
    obj->box_tables = (BoxTables *)data_80079fec_slot00;
    if (p->side == 0) {
        obj->frames = frames_left;
    } else {
        obj->frames = frames_right;
    }
    func_8011a55c(obj);
}
