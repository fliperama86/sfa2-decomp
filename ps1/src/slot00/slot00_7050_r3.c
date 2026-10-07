/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80079938_slot00[])(Object *);
extern BoxTables data_800798d0_slot00;
extern SequenceStep *data_800798fc_slot00[];
extern FrameRecord data_80079908_slot00[];
extern void func_800773b8_slot00(Object *obj);

void func_800772c0_slot00(Object *obj) {
    data_80079938_slot00[obj->field_04](obj);
}

void func_80077300_slot00(Object *obj) {
    Object *p = obj->field_3c;
    obj->field_04 = obj->field_04 + 1;
    obj->field_09 = 0;
    obj->field_1c = p->field_1c;
    obj->field_1e = p->field_1e;
    obj->box_tables = &data_800798d0_slot00;
    obj->field_45 = 0;
    obj->field_5c = 0;
    obj->pos_x = p->pos_x;
    obj->pos_y = p->pos_y;
    obj->field_0b = p->field_0b;
    obj->frames = data_80079908_slot00;
    func_80130768(obj, obj->field_ac >> 1, data_800798fc_slot00);
    obj->frame = obj->frames + obj->sequence->frame_index;
    func_800773b8_slot00(obj);
}
