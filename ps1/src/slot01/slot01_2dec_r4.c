/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80024344_slot01[];
extern u8 data_80024a7c_slot01[];
extern SequenceStep *data_800299a8_slot01[];

void func_800131ec_slot01(Object *obj) {
    obj->field_01 = 1;
    obj->field_09 = 0;
    obj->field_0b = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_04++;
    obj->pos_x = box_margin[0] + 0xc0;
    obj->pos_y = 0x78;
    obj->field_98 = data_80024344_slot01;
    obj->field_9c = data_80024a7c_slot01;
    obj->field_90 = (void *)0x8007a744;
    obj->field_7a = 0x40;
    obj->field_7c = 0x1f0 + func_80151184() % 3;
    if (obj->field_03 == 0x81 || obj->field_03 == 0x83) {
        obj->field_7c = 0x1f4;
    }
    func_80130768(obj, (s16)(obj->field_03 - 0x80), data_800299a8_slot01);
}

void func_800132f0_slot01(Object *obj) {
    data_801ae066 = obj->field_3a;
    if (data_801ae066 & 0x8000) {
        obj->field_01 = 0;
        obj->field_04++;
        game_state.field_06 = 0;
        data_801ae02c = 0;
    }
    func_80131094(obj);
}
