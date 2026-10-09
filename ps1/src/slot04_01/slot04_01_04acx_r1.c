/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013f8c4(Object *object, int a, int b);

void func_801b04ac_slot04_01(Object *obj) {
    int t;
    int x;

    x = obj->field_07;
    x++;
    obj->field_07 = x;
    x = obj->field_12a;
    t = 0x18;
    if (x != 0) {
        if (obj->field_25f == 0) {
            if ((obj->field_130 & 0xa000) != 0) {
                if (func_8013f8c4(obj, -0x14, 0x14) != 0) {
                    obj->field_04 = 1;
                    obj->field_05 = 2;
                    obj->field_06 = 0;
                    obj->field_07 = 0;
                    return;
                }
                if (obj->field_12a == 2 && (obj->field_130 & 0x8000) != 0 && obj->field_262 != 0) {
                    obj->field_159 = 1;
                    obj->field_07 = 2;
                    func_80141f28(obj, 1);
                    func_80130ec0(obj);
                    obj->field_278 = 1;
                    obj->field_29a = 1;
                    func_801307e0(obj, 0x1c);
                    return;
                }
            }
            if (obj->field_0b == 0) {
                obj->pos_x -= t;
            } else {
                obj->pos_x += t;
            }
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}
