/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80141f28(Object *object, short delta);
void func_80146478(Object *object, u8 a, int dx, int dy);
int func_80140cd8(Object *object, int a, int b);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b51a8_slot04_11(Object *obj);
Object *func_801b5914_slot04_11(Object *obj);

extern ObjectFn data_801c7804_slot04_11[];
extern ObjectFn data_801c780c_slot04_11[];
extern u8 data_801c7818_slot04_11[];

void func_801b51ec_slot04_11(Object *obj) {
    func_801b51a8_slot04_11(obj);
}

void func_801b520c_slot04_11(Object *obj) {
    data_801c7804_slot04_11[obj->field_129 >> 1](obj);
}

void func_801b5250_slot04_11(Object *obj) {
    data_801c780c_slot04_11[obj->field_07](obj);
}

void func_801b5290_slot04_11(Object *obj) {
    obj->field_07++;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    func_801204f4(obj, obj->side, 8);
    obj->field_0b = 0;
    if (!(obj->field_cd != 0 ? (s16)((u16)func_801b5914_slot04_11(obj)->pos_x + 0xc0) < obj->pos_x : (obj->field_c2 & 0x8000) != 0)) {
        obj->field_0b++;
    }
    func_801307e0(obj, 0x1a);
}

void func_801b535c_slot04_11(Object *obj) {
    int i;
    s16 c;
    s16 d;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[6];

    ref_other.p = obj->other;
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_07++;
        func_80140770(obj, 0, 5, -0x200, 0, 0, 1);
    } else if (*(u8 *)&obj->field_3a != 0) {
        i = ((*(u8 *)&obj->field_3a + 0xff) << 4) & 0x3f0;
        obj->field_3a = 0;
        c = *(u16 *)(data_801c7818_slot04_11 + i + 8);
        d = *(u16 *)(data_801c7818_slot04_11 + i + 12);
        func_80146478(obj, obj->field_12a >> 1, *(s16 *)(data_801c7818_slot04_11 + i), *(s16 *)(data_801c7818_slot04_11 + i + 4));
        if (func_80140cd8(obj, c, d)) {
            func_80120554(obj, obj->side, 0x336);
            obj->field_07++;
            func_80140770(obj, 0, 5, -0x200, 0, 0, 1);
        } else {
            func_80120554(obj, obj->side, 0x304);
            ref_other.p = obj->other;
            if (ref_other.p->field_15b == 0) {
                obj->field_07++;
                func_80140770(obj, 0, 5, -0x200, 0, 0, 1);
            }
        }
    }
    func_80130efc(obj);
}
