/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_80137e38 (original stores field_46 through a0 in the delay slot). */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80137e94(Object *object) {
    *(s16 *)&object->field_46 -= 0x100;
    if ((object->field_46 & 0xff00) == 0) {
        object->field_00 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
        object->vel_y = 0;
        object->vel_x = -object->vel_x;
        object->accel_x = -object->accel_x;
    }
    func_80131094(object);
}

void func_80137f00(Object *object) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        handlers_2694[object->state_2](object);
    }
    func_8011ffdc(object);
}

void func_80137f70(Object *object) {
    object->field_80 = 1;
    object->state_2++;
    object->field_0b ^= 1;
    func_801380f0(object);
    object->field_46 = (u8)object->field_46 | 0x200;
    if ((object->field_a0 & 0x80) == 0) {
        func_80138070(object, object->field_a0);
    }
    func_80131094(object);
}

void func_80137fe4(Object *object) {
    *(s16 *)&object->field_46 -= 0x100;
    if ((s16)object->field_46 < 0) {
        object->field_00 = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
        object->vel_x = -object->vel_x;
        object->accel_x = -object->accel_x;
        object->vel_y = table_269c[object->field_ac >> 1];
    }
    func_80131094(object);
}

void func_80138070(Object *object, u8 index) {
    if (object->field_66 == 0) {
        object->sequence = seqs_a4_left[index];
    } else {
        object->sequence = seqs_154_right[index];
    }
    object->step_timer = object->sequence->duration;
    object->step_flags = object->sequence->flags;
    object->field_80 = 1;
    object->frame = object->frames + object->sequence->frame_index;
}

void func_801380f0(Object *object) {
    if (player_left.kind == 9) {
        func_80078e44(object);
    } else if (player_right.kind == 9) {
        func_8008ef44(object);
    }
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The tables table_801726bc, table_80172980 and table_801729e4 hold this function and are declared with this parameter. */
void func_80138144(Block172 *p) {
    func_8011f240((Slab172 *)p);
}
