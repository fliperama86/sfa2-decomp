/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6500_slot04_02[];
extern u8 data_801c6508_slot04_02[];

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146478(Object *object, u8 a, int dx, int dy);

void func_801b4f0c_slot04_02(Object *obj) {
    Object *p;
    u16 t;

    p = obj->other;
    func_80130efc(obj);
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_17b = 0;
        p->field_15b = 1;
        func_80140770(obj, 0, 0xf, *(s16 *)(data_801c6500_slot04_02 + (obj->field_12a & 0xfe)), 0, 0, 1);
        func_801307e0(obj, 0x51);
        if (((Slot04aObj *)p)->field_5c < 0) {
            if (obj->field_49 != 0) {
                obj->field_167 = 0x12;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            } else {
                obj->field_167 = 2;
            }
        }
    }
    t = obj->field_3a;
    if ((u8)t != 0) {
        obj->field_3a = t & 0xff00;
        func_80146478(obj, 2, -0x17, 0x26);
        func_80120554(p, p->side, 0x306);
        if (game_state.field_5c == 0) {
            game_state.field_5c = 0xa;
            game_state.field_5d = 2;
            game_state.field_5e = 2;
        }
        game_state.field_63 = data_801c6508_slot04_02[obj->field_12a >> 1];
    }
}
