/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801e4614_slot0b[];
void func_801e09f4_slot0b(Object *obj);
void func_801e0578_slot0b(Object *obj, u8 a);
u8 func_80125b18(u8 a);

/* The local x holds the position 0x48 that two fields receive; the second store comes after the store of field_46. Written with the literal at both stores, this function differs from the original in 7 instruction slots. */
void func_801e0480_slot0b(Object *obj) {
    u8 k;
    s16 x;

    ref_other.p = obj->field_3c;
    x = 0x48;
    obj->pos_x = x;
    obj->field_46 = 0xf;
    obj->field_5c = x;
    obj->pos_y = 0x68;
    obj->field_5e = 0x88;
    obj->field_4c = 0;
    obj->field_54 = 0;
    obj->field_50 = 0;
    obj->field_58 = 0;
    obj->field_0b = obj->field_48;
    if (obj->field_48 != 0) {
        obj->pos_x = 0x128;
        obj->field_5c = 0x128;
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
    k = ref_other.p->kind;
    if (k == 0x14) {
        func_801e09f4_slot0b(obj);
    } else {
        func_80125b18(k);
    }
    func_801e0578_slot0b(obj, func_80125b18(ref_other.p->kind));
}
