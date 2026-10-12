/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_80137e38 (original stores countdown through a0 in the delay slot). */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80137cc0(Object *object) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        handlers_2684[object->state_2](object);
    }
    if ((object->countdown >> 8) & 0x92) {
        func_8011ffdc(object);
    }
}

void func_80137d48(Object *object) {
    object->state_2++;
    object->countdown = (u8)object->countdown | 0x1800;
    advance_sequence_checked(object);
}

void func_80137d7c(Object *object) {
    *(s16 *)&object->countdown -= 0x100;
    if ((s16)object->countdown < 0) {
        object->state_0 = 3;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
    advance_sequence_checked(object);
}

void func_80137dc8(Object *object) {
    Config *config = game_state.config;

    if ((config->pause_flag_a | config->pause_flag_b) == 0) {
        handlers_268c[object->state_2](object);
    }
    func_8011ffdc(object);
}
