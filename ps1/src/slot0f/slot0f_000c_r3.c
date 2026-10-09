/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e01e0_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    HudState *h = data_8018f5a0;
    Object *left = &player_left;
    Object *right = left + 1;
    h->field_4c += 1;
    obj->field_32 = 0;
    obj->field_33 = 0;
    obj->field_1b = 0;
    obj->field_09 = 0;
    obj->field_04 = 0;
    obj->field_05 = 0;
    obj->field_54 = 0;
    obj->field_66 = 0;
    obj->field_f0 = 0;
    obj->field_84 = 0;
    obj->field_12c = 0;
    obj->field_12e = 0;
    func_8011eb14();
    obj->field_42 = 1;
    obj->field_49 = 0x13;
    obj->field_4a = 0x3b;
    obj->field_9c = left;
    obj->field_a0 = right;
    left->field_01 = 1;
    left->field_00 = 1;
    right->field_01 = 1;
    right->field_00 = 1;
    left->other = right;
    right->other = left;
    left->field_d4 = 0;
    left->field_02 = 0;
    left->side = 0;
    right->field_d4 = 1;
    right->field_02 = 1;
    right->side = 1;
    right->field_cd = 1;
    left->field_cd = 1;
    left->field_07 = 0;
    left->field_06 = 0;
    left->field_05 = 0;
    left->field_04 = 0;
    right->field_07 = 0;
    right->field_06 = 0;
    right->field_05 = 0;
    right->field_04 = 0;
    right->field_c6 = 0x90;
    left->field_c6 = 0x90;
    right->kind = ((u8 *)&data_801ac624)[0];
    left->kind = ((u8 *)&data_801ac624)[0];
    obj->field_40 = ((u16 *)&data_801ac624)[1];
    func_80128978();
    data_80190568 = 1;
    data_801aa544[0].field_00 = 1;
    data_801aa5d4[0] = 1;
    cam_obj[0] = 1;
    func_80137220(0, 6);
    func_80137220(1, 0);
    func_80137220(2, 1);
    func_80137220(3, 2);
    func_80137220(4, 7);
    func_801192bc(1);
    func_80136dc4();
    func_801285e0();
    func_80136bc0();
    func_80135ef0((ObjectView *)obj);
    obj->field_2c = 0xff;
    func_8013245c();
    data_801a6938 = 0;
}
