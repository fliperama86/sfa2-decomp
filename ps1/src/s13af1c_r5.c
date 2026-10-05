/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

typedef unsigned char u8_;
u8_ func_8013d004(Object *object);
u8_ func_8013d038(Object *object);
u8_ func_8013d06c(Object *object);
u8_ func_8013d0a0(Object *object, u8_ a);

void func_8013ca3c(Object *a, Object *b) {
    b->field_04 = 1;
    b->field_06 = 7;
    b->field_15a = 11;
    b->field_05 = 0;
    b->field_07 = 0;
    a->field_29d = 1;
    a->field_159 = 0;
    b->field_159 = 0;
    a->field_225 = 0;
    b->field_225 = 0;
    a->field_6b = 16;
    b->field_6b = 12;
    func_80120554(b, b->side, 0x313);
    func_801465b0(a, 2, -0x40, 0x51);
}

u8_ func_8013cac8(Object *object, u8_ a, u8_ b) {
    return func_8013de2c(object, a, b);
}

u8_ func_8013caf0(Object *object, u8_ a, u8_ b) {
    return (func_8013d06c(object) && func_8013d038(object)) ? func_8013de2c(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cb70(Object *object, u8_ a, u8_ b) {
    return (func_8013d004(object)) ? func_8013de2c(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cbdc(Object *object, u8_ a, u8_ b) {
    return (func_8013d038(object)) ? func_8013de2c(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cc48(Object *object, u8_ a, u8_ b) {
    return (func_8013d06c(object)) ? func_8013de2c(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013ccb4(Object *object, u8_ a, u8_ b) {
    return func_8013d4cc(object, a, b);
}

u8_ func_8013ccdc(Object *object, u8_ a, u8_ b) {
    return (func_8013d06c(object) && func_8013d038(object)) ? func_8013d4cc(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cd5c(Object *object, u8_ a, u8_ b) {
    return (func_8013d004(object)) ? func_8013d4cc(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cdc8(Object *object, u8_ a, u8_ b) {
    return func_8013e574(object, a, b);
}

u8_ func_8013cdf0(Object *object, u8_ a, u8_ b) {
    return (func_8013d004(object)) ? func_8013e574(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013ce5c(Object *object, u8_ a, u8_ b) {
    return (func_8013d038(object)) ? func_8013e574(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cec8(Object *object, u8_ a, u8_ b) {
    return (func_8013d06c(object)) ? func_8013e574(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cf34(Object *object, u8_ a, u8_ b) {
    return (func_8013d038(object) && func_8013d06c(object)) ? func_8013e574(object, a, b) : func_8013d0a0(object, a);
}

u8_ func_8013cfb4(Object *object, u8_ a, u8_ b) {
    return func_8013ead4(object, a, b);
}

u8_ func_8013cfdc(Object *object, u8_ a, u8_ b) {
    return func_8013ee80(object, a, b);
}

u8_ func_8013d004(Object *object) {
    if (object->field_d8 != 0) {
        return ((object->field_134 | object->field_136) & 0xc0) != 0xc0;
    }
    return 1;
}

u8_ func_8013d038(Object *object) {
    if (object->field_d8 != 0) {
        return ((object->field_134 | object->field_136) & 0x30) != 0x30;
    }
    return 1;
}

u8_ func_8013d06c(Object *object) {
    if (object->field_d8 != 0) {
        return ((object->field_134 | object->field_136) & 0xc) != 0xc;
    }
    return 1;
}

u8_ func_8013d0a0(Object *object, u8_ a) {
    object->slots[a].field_00 = 0;
    object->field_25c = object->field_25c + 1;
    return 1;
}
