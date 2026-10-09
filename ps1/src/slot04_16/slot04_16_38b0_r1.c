/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void func_801b38b0_slot04_16(Object *obj) {
    s16 a[12] = { 0x17, 0, 0x1b, 0, 0x1f, 0x12, 0x14, 0xf, 0x16, 0xf, 0x18, 0xf };
    s16 b[3] = { 0x20, 0x21, 0x22 };
    s32 c[12] = { 0x38000, 0x60000, 0, -0x8000, 0x30000, 0x70000, 0, -0x8400, 0x48000, 0x80000, 0, -0x8800 };
    u16 t;
    s16 *p;
    int k;
    u8 i;

    t = obj->field_3a;
    if ((u8)t == 0) {
        func_80130efc(obj);
        return;
    }
    if ((u8)t != 0xf) {
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_3a &= 0xff00;
        game_state.field_358 = obj->other;
        game_state.field_358->field_15b = 1;
        func_80140770(obj, ((u8 *)b)[obj->field_12a & 0xfe], 0xf, -0x200, 0, 0, 1);
        game_state.field_358 = obj->other;
        if ((s16)game_state.field_358->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
            }
        }
        obj->field_4c = c[(obj->field_12a >> 1) * 4 + 0];
        obj->field_50 = c[(obj->field_12a >> 1) * 4 + 1];
        obj->field_58 = c[(obj->field_12a >> 1) * 4 + 3];
        obj->field_54 = 0;
        if (obj->field_0b != 0) {
            obj->field_4c = -obj->field_4c;
        }
    } else {
        obj->field_3a = t & 0xff00;
        game_state.config->field_63 = (obj->field_12a << 2) + 0x3c;
        func_80146960(obj);
        func_80130efc(obj);
        k = obj->field_49 != 0 ? 6 : 0; p = (s16 *)((obj->field_12a + k) * 2 + (u32)a);
        if ((u8)func_80140cd8(obj, p[0], p[1]) != 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x20;
                obj->field_255 = 6;
                game_state.config->field_6b = 0;
                func_80147000(obj);
            }
        }
        func_80120554(obj, obj->side, 0x319);
    }
}
