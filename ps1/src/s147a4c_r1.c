/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80147a4c(Block172 *block) {
    Object *object = (Object *)block;
    fns_cbd0[object->state_0](object);
}

void func_80147a8c(Object *object) {
    object->ot_depth = 0;
    object->state_0++;
    object->variant_index_2 = bytes_cbe0[object->table_index + object->variant_index];
    func_80130768(object, 0x13, (SequenceStep **)seqs_c7f8);
}

void func_80147ae4(Object *object) {
    fns_cbe8[object->table_index](object);
}

void func_80147b24(Object *object) {
    fns_cbf4[object->state_1](object);
    set_position_from_parent(object);
    func_80120028(object);
}

void func_80147b80(Object *object) {
    object->countdown = 0x1d;
    object->state_1++;
    advance_sequence_checked(object);
}

void func_80147bb0(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->state_1++;
        func_80130768(object, 0x14, (SequenceStep **)seqs_c7f8);
    } else {
        advance_sequence_checked(object);
    }
}

void func_80147c0c(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_1 = 0;
        object->state_0++;
    } else {
        advance_sequence_checked(object);
    }
}

void func_80147c50(Object *object) {
    fns_cc00[object->state_1](object);
    func_80120028(object);
}

void func_80147ca4(Object *object) {
    object->countdown = 5;
    object->state_1++;
    advance_sequence_checked(object);
    func_80147ce8(object);
}

void func_80147ce8(Object *object) {
    if (object->variant_index != 0) {
        object->vel_x = 0xffc60000;
        object->vel_y = 0x180000;
        object->accel_x = 0x74000;
        object->accel_y = 0xfffd0000;
    } else {
        object->vel_x = 0x3a0000;
        object->vel_y = 0xffe80000;
        object->accel_x = 0xfff8c000;
        object->accel_y = 0x30000;
    }
    func_80148194(object);
}

void func_80147d54(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->countdown = 7;
        object->state_1++;
    }
    advance_sequence_checked(object);
}

void func_80147da0(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->countdown = 7;
        object->state_1++;
        func_80147ce8(object);
    } else {
        func_80148194(object);
    }
}

void func_80147df8(Object *object) {
    func_80147da0(object);
}

void func_80147e18(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        if (object->active_state != 0) {
            func_80147ef0(object);
        } else {
            object->state_1++;
            set_position_from_parent(object);
            func_80130768(object, 0x14, (SequenceStep **)seqs_c7f8);
        }
    } else {
        func_80148194(object);
    }
}

void func_80147eb0(Object *object) {
    if ((s16)object->step_flags < 0) {
        func_80147ef0(object);
    } else {
        advance_sequence_checked(object);
    }
}

void func_80147ef0(Object *object) {
    object->state_0++;
    object->state_1 = 0;
}

void func_80147f04(Object *object) {
    fns_cc18[object->state_1](object);
    func_80120028(object);
}

void func_80147f58(Object *object) {
    object->countdown = 5;
    object->state_1++;
    func_80147f88(object);
}

void func_80147f88(Object *object) {
    func_80148220(object);
    advance_sequence_checked(object);
}

void func_80147fb8(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        object->countdown = 0x17;
        object->state_1++;
    }
    advance_sequence_checked(object);
}

void func_80148004(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v < 0) {
        if (object->active_state != 0) {
            func_80147ef0(object);
        } else {
            object->state_1++;
            func_80130768(object, 0x14, (SequenceStep **)seqs_c7f8);
        }
    } else if (object->variant_index_2 != 0 || object->variant_index != 0) {
        func_80148220(object);
    } else {
        advance_sequence_checked(object);
    }
}

void func_801480b0(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_1 = 0;
        object->state_0++;
    } else {
        func_80147f88(object);
    }
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fns_cbd0 holds this function and is declared with this parameter. */
void func_801480f4(Object *p) {
    func_8011f240((Slab172 *)p);
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table fns_cbd0 holds this function and is declared with this parameter. */
void func_80148114(Object *p) {
    func_8011f240((Slab172 *)p);
}

void set_position_from_parent(Object *object) {
    Object *parent = object->parent;
    object->facing_side = parent->facing_side;
    if (object->facing_side != 0) {
        object->pos_x = parent->pos_x - object->health;
    } else {
        object->pos_x = parent->pos_x + object->health;
    }
    object->pos_y = parent->pos_y - object->y_offset;
}

void func_80148194(Object *object) {
    Object *parent = object->parent;
    int a = object->vel_x;
    int b;
    object->facing_side = parent->facing_side;
    b = object->vel_y;
    if (object->facing_side != 0) {
        object->pos_x = parent->pos_x + (a >> 16) - object->health;
    } else {
        object->pos_x = parent->pos_x + (a >> 16) + object->health;
    }
    object->pos_y = parent->pos_y - ((b >> 16) + object->y_offset);
    object->vel_x += object->accel_x;
    object->vel_y += object->accel_y;
}
