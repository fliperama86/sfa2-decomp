/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c4220_slot04_04[];
void func_80138ae8(GameState *state, Object *object);
void func_801b19bc_slot04_04(Object *obj);
void func_801b1a08_slot04_04(Object *obj);
void func_801b1a54_slot04_04(Object *obj);

void func_801b17c4_slot04_04(Object *obj) {
    s16 a;
    int s;
    u8 t;
    u8 g;
    u8 u;
    FrameRecord *fr;

    s = (obj->field_3a << 16) >> 24;
    if (obj->field_cd != 0) {
        func_801b1a54_slot04_04(obj);
    } else {
        if ((*(u32 *)&game_state.field_4c & 0xffff00) != 0 || game_state.field_04 != 0 || (u8)func_8012f56c(obj)) {
            func_801b1a08_slot04_04(obj);
            return;
        }
        if ((s & 0x7f) != 0) {
            obj->field_3a = obj->field_3a & 0x80ff;
            func_80142c04(obj);
            func_80138ae8(&game_state, obj);
            if (obj->field_254 == 0) {
                obj->field_254 = 0xff;
            }
        }
        if ((obj->field_134 & 0x68) == 0) {
            func_801b19bc_slot04_04(obj);
            return;
        }
        if (obj->field_134 & 0x40) {
            a = 0;
        } else if (obj->field_134 & 0x20) {
            a = 2;
        } else {
            a = 4;
        }
        if ((s16)obj->field_46 <= data_801c4220_slot04_04[a >> 1]) {
            func_801b19bc_slot04_04(obj);
            return;
        }
        obj->field_46 = 0xf;
        if (obj->field_12a == a) {
            func_80130efc(obj);
            return;
        }
        obj->field_12a = a;
        u = obj->field_3a;
        g = obj->field_49;
        if (g != 0) {
            a += 0x40;
        } else {
            a += 0x21;
        }
        func_801307e0(obj, a);
        obj->sequence = &obj->sequence[u];
        obj->field_3a = obj->sequence->flags;
        obj->field_38 = obj->sequence->duration;
        fr = obj->frames + obj->sequence->frame_index;
        obj->frame = fr;
        t = fr->field_0d;
        obj->field_80 = 1;
        obj->field_4a = t;
    }
}
