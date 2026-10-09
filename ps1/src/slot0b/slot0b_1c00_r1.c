/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801e7630_slot0b[];

void func_801e1c00_slot0b(Object *obj) {
    obj->field_01 = 1;
    obj->field_09 = 0;
    obj->field_0b = 0;
    obj->field_0e = 0;
    obj->field_0f = 0;
    obj->field_26 = 0;
    obj->field_04++;
    obj->pos_x = box_margin[0];
    obj->pos_y = data_801aa5ea[0] + 8;
    func_80130768(obj, obj->field_02, data_801e7630_slot0b);
}

void func_801e1c6c_slot0b(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_01 = 0;
        obj->field_04++;
        data_80190468.p->field_06 = 0;
    }
    obj->pos_x = box_margin[0];
    obj->pos_y = data_801aa5ea[0] + 8;
    func_80131094(obj);
}

void func_801e1cdc_slot0b(Object *obj) {
    u32 *p = (u32 *)obj;
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    ((u8 *)p)[8] = 0x18;
    data_801ae02c = 0;
    game_state.field_06 = 0;
}
