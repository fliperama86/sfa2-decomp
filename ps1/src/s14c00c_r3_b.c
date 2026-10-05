/* Reconstruction. Names/roles inferred, not original symbols.
 * Unresolved: the 20e/20f split in func_8014c914/9f4/a7c/b1c. The original does
 * sll 16 / sra 24 (and a move) where this compiles to srl 8. Calls without an
 * argument (nop in the delay slot) are written as unprototyped declarations. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8014de94(Object *object);

void func_8014cba4(Object *object) {
    if (func_8014a170(object, table_8017d450) != 0) {
        func_8014ca7c(object);
    } else {
        func_8014c9f4(object);
    }
}

void func_8014cbf8(Object *object) {
    if (object->field_20c == 0) {
        func_8014d99c();
    } else if (object->field_20c == 2) {
        func_8014cc80(object);
    } else if (object->field_20c == 4) {
        if (func_8014a170(object, table_8017d450) != 0) {
            func_8014d99c(object);
        } else {
            func_8014cc80(object);
        }
    }
}

void func_8014cc80(Object *object) {
    object->field_20a = 0;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = object->field_222;
    func_8014cb1c(object);
}

void func_8014ccb0(Object *object) {
    table_8017d28c[object->field_208]();
}

void func_8014ccf0(Object *object) {
    table_8017d2a8[object->field_209]();
}

void func_8014cd30(Object *object) {
    if (func_8014de94(object) != 0) {
        func_8014e080(object);
    } else if (object->field_21c == 0 || --object->field_21c == 0) {
        func_8014d99c(object);
    }
}

void func_8014cda0(Object *object) {
    if (func_8014de94(object) != 0) {
        func_8014e080(object);
    } else if (object->field_06 == 0 || object->field_157 != 0) {
        func_8014d99c(object);
    }
}

void func_8014ce0c(Object *object) {
    if (func_8014de94(object) != 0) {
        func_8014e080(object);
    } else if (object->field_06 == 0 || object->field_157 == 0) {
        func_8014d99c(object);
    }
}

void func_8014ce78(Object *object) {
    if (func_8014de94(object) != 0) {
        func_8014e080(object);
    } else if (object->other->field_45 != 1) {
        func_8014d99c(object);
    }
}

void func_8014cedc(Object *object) {
    Object *other;
    if (func_8014de94(object) != 0) {
        func_8014e080(object);
    } else {
        other = object->other;
        if (other->field_45 != 1
            || ((s16)other->pos_y >= (s16)(other->field_70 - (object->field_211 + (object->field_210 << 8)))
                && other->field_50 > 0)) {
            func_8014d99c(object);
        }
    }
}

void func_8014cf80(Object *object) {
    if (func_8014de94(object) != 0) {
        func_8014e080(object);
    } else if (object->field_21c == 0 || --object->field_21c == 0) {
        func_8014d99c(object);
    } else if (func_80142378(object) != 0) {
        object->field_208 = 3;
        object->field_209 = 0;
        object->field_04 = 1;
        object->field_05 = 0;
        object->field_06 = 5;
        object->field_07 = 0;
        object->field_159 = 1;
        object->field_21b = 0;
        object->field_0b = object->field_158;
    }
}
