/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b28_slot04_01(Object *object);

void func_801b2d0c_slot04_01(Object *obj) {
    int a = -0x24;
    int b = 0x55;

    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07++;
        if (obj->field_45 != 0) {
            a = -5;
            b = 0x45;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3b28_slot04_01(obj);
    }
    func_80130efc(obj);
}
