/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_80149f14(Object *object) {
    return object->input_matched_flag == 0;
}

/* x is read before it is set: the original reads the fourth argument register as it is at entry, and the one caller passes two arguments. Reading the unset local is undefined behaviour in C. It reproduces the original's instructions with this compiler and is not a defined implementation: a port has to give x a value. */
int func_80149f20(Object *object, Object *unused) {
    int x;
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
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 6;
    if (other->active_state == 0 && other->field_157 != 0) {
        arg = 0x18;
        object->field_157 = 1;
        object->state_3 = 2;
    } else {
        arg = 0x15;
        object->field_157 = 0;
        object->state_3 = 0;
    }
    set_sequence_by_side(object, arg);
}

void func_8014a038(Object *object) {
    object->table_flag = data_8017cf7c[object->table_column];
}

int func_8014a058(Object *object) {
    if ((object->saved_mode_bits & 1) == 0) return 0;
    if (object->kind != 4 && object->kind != 7) return 0;
    if (!(object->pos_y < (s16)(object->ground_y - 0x20))) return 0;
    if (object->variant_index == 0) return 0;
    if (object->bound_push_flag == 0) return 0;
    if (object->vel_x == 0) return 0;
    if (object->vel_x < 0) {
        return object->bound_push_flag == 1;
    }
    return object->bound_push_flag == 2;
}

int func_8014a0fc(Object *object) {
    if (object->once_flag != 0) return 0;
    if (func_8014a170(object, data_8017d698) == 0) return 0;
    if (func_80149d48(object) == 0) return 0;
    object->once_flag = 1;
    return 1;
}
