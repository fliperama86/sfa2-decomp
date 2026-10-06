/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801ae02e;
extern void (*data_801e4d5c_slot0b[])(u8 *);
extern void (*data_801e4d78_slot0b[])(void);

void func_801e1b20_slot0b(void) {
    func_8011f240();
}

void func_801e1b40_slot0b(Object *obj) {
    *(int *)&obj->field_10 = *(int *)&obj->field_10 + obj->field_4c;
    *(int *)&obj->field_14 = *(int *)&obj->field_14 + obj->field_54;
}

void func_801e1b64_slot0b(void) {
    u8 *p = &data_801ae02c;
    if (*p != 0) {
        data_801e4d5c_slot0b[data_801ae02e](p);
    }
}

void func_801e1bc0_slot0b(Object *obj) {
    data_801e4d78_slot0b[obj->field_04]();
}
