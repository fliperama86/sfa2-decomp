/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80137b10(void) {
    func_80137220(0, 6);
    func_80137220(1, 0);
    func_80137220(2, 1);
    func_80137220(3, 2);
    func_80137220(4, 7);
}

void func_80137b64(Object *object) {
    if ((game_state.config->field_65 | game_state.config->field_a8) != 0) {
        func_8011ffdc(object);
    } else {
        *(s32 *)&object->field_10 += object->field_4c;
        *(u32 *)&object->field_14 -= object->field_50;
        func_80131094(object);
        func_8011ff74(object);
        func_8011ffdc(object);
    }
}

void func_80137be0(Object *object) {
    if ((game_state.config->field_65 | game_state.config->field_a8) != 0) {
        func_8011ffdc(object);
    } else {
        handlers_267c[object->field_06](object);
        func_8011ffdc(object);
    }
}

void func_80137c54(Object *object) {
    object->field_06++;
    object->field_46 = (object->field_46 & 0xff) | 0x200;
    func_80131094(object);
}
