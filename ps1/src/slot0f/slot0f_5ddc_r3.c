/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void *data_800ef978_slot0f[];

void func_800e61c4_slot0f(Object *obj);
void func_800e6410_slot0f(Object *obj, int a);

void func_800e6078_slot0f(Object *obj) {
    FrameRecord *rec;

    obj->field_09 = 4;
    obj->field_01 = 1;
    obj->field_50 = 0x70000;
    obj->field_54 = 0x500;
    obj->field_58 = -0x5000;
    obj->field_4c = 0xfffa8000;
    obj->field_04++;
    obj->field_0b = 0;
    rec = table_8016e614;
    if (obj->field_03 == 0) {
        rec = table_8016e5c4;
    }
    obj->field_7a = 0x60;
    obj->field_7c = 0x1f0;
    obj->field_0d = 0;
    obj->field_60 = 0;
    obj->field_60 = rec->field_08;
    obj->field_61 = rec->field_09;
    obj->field_0c = 0xff;
    obj->field_98 = data_800ef978_slot0f[obj->field_60 * 2];
    obj->field_9c = data_800ef978_slot0f[obj->field_60 * 2 + 1];
    func_800e61c4_slot0f(obj);
    if (game_state.config->field_01 == 0) {
        obj->pos_x += 0x10e;
        obj->pos_y += 0x40;
    } else {
        obj->field_05 = 7;
        obj->field_46 = 10;
        func_800e6410_slot0f(obj, 0);
    }
}

void func_800e61bc_slot0f(Object *obj) {
}
