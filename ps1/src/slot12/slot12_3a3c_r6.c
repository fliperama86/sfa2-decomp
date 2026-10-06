/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);
void func_80014268_slot12(Object *obj);

void func_80013ed8_slot12(Object *obj) {
    Block172 *b;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_02 = 0xa0;
    b->field_03 = game_state.field_2bd;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 0;
    b->field_02 = 0xa1;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 1;
    b->field_02 = 0xa1;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 2;
    b->field_02 = 0xa1;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 3;
    b->field_02 = 0xa1;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 4;
    b->field_02 = 0xa1;
    b = func_8011f1e0();
    if (b == 0) goto fail;
    b->field_00 = 1;
    b->field_03 = 5;
    b->field_02 = 0xa1;
    if (game_state.field_2bd == 0) {
        b = func_8011f1e0();
        if (b == 0) goto fail;
        b->field_00 = 1;
        b->field_03 = 0;
        b->field_02 = 0xa3;
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
    }
    return;
fail:
    func_80014268_slot12(obj);
}
