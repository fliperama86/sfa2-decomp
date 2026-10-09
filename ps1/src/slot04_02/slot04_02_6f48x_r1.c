/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7514_slot04_02[];
extern u16 data_801c7748_slot04_02[];
extern u16 data_801c7754_slot04_02[];
extern s32 data_801c7760_slot04_02[];
extern s32 data_801c779c_slot04_02[];
extern FrameRecord *data_1f8000a8;
extern FrameRecord *data_1f800158;

int func_801b772c_slot04_02(Object *object, Object *parent);
void func_801b7920_slot04_02(Object *object, u8 a_arg);

/* The local t holds the 0x7754 table value for the y offset; d holds the later x offsets. Written with one local for both, this function differs from the original in 16 instruction slots. */
void func_801b6f48_slot04_02(Object *object) {
    Object *parent;
    u16 d;
    int t;

    ((Slot04aObj *)object)->field_6c = data_801c7514_slot04_02;
    object->field_04 = object->field_04 + 1;
    ((Slot04aObj *)object)->field_b0 = object->field_0d;
    parent = object->field_3c;
    object->field_09 = 0;
    object->field_49 = parent->field_49;
    object->field_1c = parent->field_1c;
    if (object->field_03 != 0) {
        func_801b772c_slot04_02(object, parent);
    }
    t = data_801c7754_slot04_02[object->field_03 >> 1];
    object->field_50 = data_801c779c_slot04_02[object->field_ac];
    object->pos_y = object->pos_y - t;
    if (object->field_0b != 0) {
        d = data_801c7748_slot04_02[object->field_03 >> 1];
        object->field_4c = data_801c7760_slot04_02[object->field_ac];
    } else {
        d = data_801c7748_slot04_02[object->field_03 >> 1];
        object->field_4c = -data_801c7760_slot04_02[object->field_ac];
        d = -d;
    }
    object->pos_x = d + object->pos_x;
    if (parent->side == 0) {
        ((Slot04aObj *)object)->field_8c = (int)data_1f8000a8;
    } else {
        ((Slot04aObj *)object)->field_8c = (int)data_1f800158;
    }
    func_801b7920_slot04_02(object, object->field_ac);
    if (object->field_03 == 2 && ((Slot04aObj *)parent)->field_48 == -1) {
        if (parent->field_0b != 0) {
            object->field_4c = object->field_4c - 0x10000;
        } else {
            object->field_4c = object->field_4c + 0x10000;
        }
    }
}
