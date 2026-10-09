/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140cd8(Object *obj, int a, int b);
void func_80146478(Object *object, u8 a, int dx, int dy);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80140fe0(Object *object);
int func_801410c8(Object *object);

void func_801b3edc_slot04_08(Object *obj, Object *unused) {
    int a;
    Object **q;
    Object **r = &ref_other.p;
    u16 t;
    *r = obj->other;
    t = obj->field_3a;
    if ((u8)t != 0) {
        obj->field_3a = t & 0xff00;
        func_80146478(obj, 1, -0x1d, 0x52);
        a = 1;
        if (((Slot04aObj *)obj)->field_1c7 == 0) {
            ((Slot04aObj *)obj)->field_1c7 = 0xff;
            a = 0xa;
        }
        if ((u8)func_80140cd8(obj, a, 0) != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 1);
            func_80120554(game_state.field_358, ref_other.p->side, 0x336);
            goto tail;
        }
        obj->field_46++;
        func_801204f4(obj, obj->side, 0xc);
        q = &game_state.field_358;
        func_80120554(*q, ref_other.p->side, 0x304);
        func_80140fe0(obj);
        if ((s16)obj->field_46 != 0) {
            *q = obj->other;
            if (game_state.field_358->field_15b == 0 || (u8)func_801410c8(obj) != 0) {
                goto act;
            }
        }
        func_80130efc(obj);
        return;
    }
    func_80140fe0(obj);
    if ((s16)obj->field_46 != 0) {
        *r = obj->other;
        if (ref_other.p->field_15b == 0 || (u8)func_801410c8(obj) != 0) {
act:
            func_80140770(obj, 0, 5, 0, 0, 0, 1);
            func_801204f4(obj, obj->side, 0xd);
tail:
            obj->field_07++;
            func_801307e0(obj, 0x1b);
            return;
        }
    }
    func_80130efc(obj);
}
