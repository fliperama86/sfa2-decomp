/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801903c5;
void func_800e585c_slot0f(Object *obj);

void func_800e5470_slot0f(Object *obj) {
    Block172 *b;
    u8 *flag;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    flag = &data_801903c5;
    b->field_02 = 0x90;
    b->field_03 = *flag;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 0;
    b->field_02 = 0x96;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 1;
    b->field_02 = 0x96;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 2;
    b->field_02 = 0x96;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 3;
    b->field_02 = 0x96;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 4;
    b->field_02 = 0x96;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 5;
    b->field_02 = 0x96;
    if (*flag != 0) return;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 0;
    b->field_02 = 0x98;
    if (obj->field_01 != 0) return;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 0;
    b->field_02 = 0x92;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 1;
    b->field_02 = 0x92;
    return;
fail:
    func_800e585c_slot0f(obj);
}
