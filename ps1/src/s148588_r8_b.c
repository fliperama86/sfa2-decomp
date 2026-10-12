/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80149238(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cf00[object->state_0](object);
}

void func_80149278(Object *object) {
    object->state_0 = object->state_0 + 1;
    table_8017cf10[object->table_index](object);
}

void func_801492c0(Object *object) {
    object->ot_depth = 6;
    func_80149888(object);
    func_80130768(object, 0x25, seqs_8017c7f8);
}

void func_80149304(Object *object) {
    object->ot_depth = 8;
    func_80149888(object);
    func_80130768(object, 0x26, seqs_8017c7f8);
}

void func_80149348(Object *object) {
    Object *other = object->parent;
    object->ot_depth = 6;
    play_sound_at(other, other->side, 0x326);
    func_80130768(object, 0x27, seqs_8017c7f8);
}

void func_80149398(Object *object) {
    int dx;
    int dy;
    object->ot_depth = 6;
    dx = (get_random_byte() & 0x1f) - 0x10;
    object->pos_x = object->pos_x + dx;
    dy = (get_random_byte() & 0x1f) + 0x10;
    object->pos_y = object->pos_y - dy;
    func_80130768(object, (get_random_byte() & 7) | 0x28, seqs_8017c7f8);
}

void func_80149410(Object *object) {
    table_8017cf20[object->table_index](object);
    func_80120028(object);
}

void func_80149464(Object *object) {
    ref_other.p = object->parent;
    if (ref_other.p->field_165 == 0) {
        object->state_0 = object->state_0 + 1;
    }
    advance_sequence_checked(object);
}

void func_801494c0(Object *object) {
    func_80149464(object);
}

void func_801494e0(Object *object) {
    ref_other.p = object->parent;
    if ((s16)object->step_flags < 0) {
        object->state_0 = object->state_0 + 1;
        play_sound_at(ref_other.p, ref_other.p->side, 0x328);
    }
    /* Byte load of the low half of step_flags. */
    if (*(u8 *)&object->step_flags != 0) {
        play_sound_at(ref_other.p, ref_other.p->side, 0x327);
    }
    advance_sequence_checked(object);
}

void func_80149584(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_0 = object->state_0 + 1;
    }
    advance_sequence_checked(object);
}
