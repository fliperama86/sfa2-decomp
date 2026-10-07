/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146478(Object *object, u8 a, int dx, int dy);
int func_80140cd8(Object *object, int a, int b);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80140fe0(Object *object);
int func_801410c8(Object *object);

void func_801b3280_slot04_01(Object *obj) {
    u16 t = obj->field_3a;
    Object *p = obj->other;

    if ((u8)t != 0) {
        obj->field_3a = t & 0xff00;
        obj->field_07++;
        func_80146478(obj, 1, -0x3e, 0x39);
        func_801204f4(obj, ((Slot04aObj *)obj)->field_a6, 8);
        if ((u8)func_80140cd8(obj, 8, 0) != 0) {
            func_80140770(obj, 1, 5, 0, 0, 0, 0);
            func_80120554(p, ((Slot04aObj *)p)->field_a6, 0x30d);
            goto done;
        }
        obj->field_46++;
        func_80120554(p, ((Slot04aObj *)p)->field_a6, 0x30b);
    }
    func_80140fe0(obj);
    if ((s16)obj->field_46 == 0 || (p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
        func_80130efc(obj);
        return;
    }
    func_80146478(obj, 1, -0x3e, 0x39);
    func_80140770(obj, 1, 5, 8, 0, 0, 1);
    func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x30d);
done:
    obj->field_07 = 3;
    func_801204f4(obj, ((Slot04aObj *)obj)->field_a6, 7);
    func_801307e0(obj, 0x2f);
}
