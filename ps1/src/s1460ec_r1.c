/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

void func_801460ec(u32 *p) {
    p[0] = (u32)func_8011f1e0();
    p[1] = (u32)func_8011f1e0();
    p[2] = (u32)func_8011f1e0();
    if (p[0] > p[1]) {
        u32 t = p[0];
        p[0] = p[1];
        p[1] = t;
    }
    if (p[0] > p[2]) {
        u32 t = p[0];
        p[0] = p[2];
        p[2] = t;
    }
    if (p[1] > p[2]) {
        u32 t = p[1];
        p[1] = p[2];
        p[2] = t;
    }
}

void func_80146184(Block172 *block) {
    Object *object = (Object *)block;
    fn_table_8017c9e0[object->field_04](object);
}
