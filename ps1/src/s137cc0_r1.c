/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_80137e38 (original stores field_46 through a0 in the delay slot). */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80137cc0(Object *object) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        handlers_2684[object->field_06](object);
    }
    if ((object->field_46 >> 8) & 0x92) {
        func_8011ffdc(object);
    }
}

void func_80137d48(Object *object) {
    object->field_06++;
    object->field_46 = (u8)object->field_46 | 0x1800;
    func_80131094(object);
}

void func_80137d7c(Object *object) {
    *(s16 *)&object->field_46 -= 0x100;
    if ((s16)object->field_46 < 0) {
        object->field_04 = 3;
        object->field_05 = 0;
        object->field_06 = 0;
        object->field_07 = 0;
    }
    func_80131094(object);
}

void func_80137dc8(Object *object) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        handlers_268c[object->field_06](object);
    }
    func_8011ffdc(object);
}
