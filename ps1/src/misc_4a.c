/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80130678(Object *object, int arg);

u8 func_80149f14(Object *object) {
    return object->field_254 == 0;
}

int func_80149f20(Object *object, int a1, int a2, int x) {
    Object *other = object->other;
    s16 *flags = &other->sequence->flags;
    do {
        if (other->frames[((u16 *)flags)[4]].active != 0) {
            x = x + (u16)other->pos_x - (u16)object->pos_x + 0x60;
            if ((u16)x <= 0xc0) {
                return func_80149f14(other) != 0;
            }
            break;
        }
        flags += 6;
    } while (*flags > 0);
    return func_80149ec8(object, other, flags, x) != 0;
}

void func_80149fc4(Object *object) {
    Object *other = object->other;
    int arg;
    object->field_04 = 1;
    object->field_05 = 0;
    object->field_06 = 6;
    if (other->field_45 == 0 && other->field_157 != 0) {
        arg = 0x18;
        object->field_157 = 1;
        object->field_07 = 2;
    } else {
        arg = 0x15;
        object->field_157 = 0;
        object->field_07 = 0;
    }
    func_80130678(object, arg);
}

void func_8014a038(Object *object) {
    object->field_257 = data_8017cf7c[object->field_cf];
}

int func_8014a058(Object *object) {
    if ((object->field_217 & 1) == 0) return 0;
    if (object->kind != 4 && object->kind != 7) return 0;
    if (!(object->pos_y < (s16)(object->field_70 - 0x20))) return 0;
    if (object->field_48 == 0) return 0;
    if (object->field_164 == 0) return 0;
    if (object->field_4c == 0) return 0;
    if (object->field_4c < 0) {
        return object->field_164 == 1;
    }
    return object->field_164 == 2;
}

int func_8014a0fc(Object *object) {
    if (object->field_242 != 0) return 0;
    if (func_8014a170(object, data_8017d698) == 0) return 0;
    if (func_80149d48(object) == 0) return 0;
    object->field_242 = 1;
    return 1;
}
