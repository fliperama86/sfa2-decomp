/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80141618(Object *object);
int func_801412a4(Object *object);

void func_801b0768_slot04_07(Object *obj) {
    s16 t = obj->field_3a;

    if ((t & 0x8000) != 0) {
        ((Slot04aObj *)obj)->field_1ca = 0;
        func_801312b8(obj);
    } else {
        if ((t & 0x80) != 0 && func_80141618(obj) != 0) {
            goto reset;
        } else if (((Slot04aObj *)obj)->field_1ca == 7 && obj->field_12a == 4 && (obj->field_130 & 0x4000) == 0 && (obj->field_134 & 8) != 0) {
            obj->field_07 = 2;
            obj->field_129 = 2;
            func_80142c04(obj);
            if (obj->field_0b == 0) {
                obj->pos_x -= 0x20;
            } else {
                obj->pos_x += 0x20;
            }
            func_80130ec0(obj);
            func_801307e0(obj, 0x2f);
            return;
        } else if ((u8)func_801412a4(obj) == 0) {
            func_80130efc(obj);
        } else {
            ((Slot04aObj *)obj)->field_1ca |= 1 << (obj->field_12a >> 1);
            if (obj->field_0b == 0) {
                obj->pos_x -= 0x18;
            } else {
                obj->pos_x += 0x18;
            }
        reset:
            obj->field_25f = 0xff;
            obj->field_07 = 0;
            func_80130efc(obj);
        }
    }
}
