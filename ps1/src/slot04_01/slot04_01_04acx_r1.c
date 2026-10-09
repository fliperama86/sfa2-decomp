/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130dc0(Object *object);
u8 func_8013f8c4(Object *object, int a, int b);

/* The local u holds the constant 1 in the one block that calls; the local t holds the constant of the other blocks. Written with one local for both, this function differs from the original in 36 instruction slots. */
void func_801b04ac_slot04_01(Object *obj) {
    int t;
    int u;
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
                    u = 1;
                    obj->field_159 = u;
                    obj->field_07 = 2;
                    func_80141f28(obj, 1);
                    func_80130ec0(obj);
                    obj->field_278 = u;
                    obj->field_29a = u;
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
