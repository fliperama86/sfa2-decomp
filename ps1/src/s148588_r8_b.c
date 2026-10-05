/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8011a55c(void);

void func_80149238(Object *object) {
    table_8017cf00[object->field_04](object);
}

void func_80149278(Object *object) {
    object->field_04 = object->field_04 + 1;
    table_8017cf10[object->field_03](object);
}

void func_801492c0(Object *object) {
    object->field_09 = 6;
    func_80149888(object);
    func_80130768(object, 0x25, seqs_8017c7f8);
}

void func_80149304(Object *object) {
    object->field_09 = 8;
    func_80149888(object);
    func_80130768(object, 0x26, seqs_8017c7f8);
}

void func_80149348(Object *object) {
    Object *other = object->field_3c;
    object->field_09 = 6;
    func_80120554(other, other->side, 0x326);
    func_80130768(object, 0x27, seqs_8017c7f8);
}

void func_80149398(Object *object) {
    int dx;
    int dy;
    object->field_09 = 6;
    dx = (func_80151184() & 0x1f) - 0x10;
    object->pos_x = object->pos_x + dx;
    dy = (func_80151184() & 0x1f) + 0x10;
    object->pos_y = object->pos_y - dy;
    func_80130768(object, (func_80151184() & 7) | 0x28, seqs_8017c7f8);
}

void func_80149410(Object *object) {
    table_8017cf20[object->field_03](object);
    func_80120028(object);
}

void func_80149464(Object *object) {
    ref_other.p = object->field_3c;
    if (ref_other.p->field_165 == 0) {
        object->field_04 = object->field_04 + 1;
    }
    func_80131094(object);
}

void func_801494c0(Object *object) {
    func_80149464(object);
}

void func_801494e0(Object *object) {
    ref_other.p = object->field_3c;
    if ((s16)object->field_3a < 0) {
        object->field_04 = object->field_04 + 1;
        func_80120554(ref_other.p, ref_other.p->side, 0x328);
    }
    /* Byte load of the low half of field_3a. */
    if (*(u8 *)&object->field_3a != 0) {
        func_80120554(ref_other.p, ref_other.p->side, 0x327);
    }
    func_80131094(object);
}

void func_80149584(Object *object) {
    if ((s16)object->field_3a < 0) {
        object->field_04 = object->field_04 + 1;
    }
    func_80131094(object);
}
