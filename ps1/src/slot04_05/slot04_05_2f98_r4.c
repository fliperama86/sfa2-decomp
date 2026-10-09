/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3b14_slot04_05(Object *obj);

void func_801b32c8_slot04_05(Object *obj) {
    int a;
    if ((s16)obj->field_3a & 0xff00) {
        a = 2;
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = -1;
        }
        func_801495f8(obj, a, 0x50);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3b14_slot04_05(obj);
    }
    func_80130efc(obj);
}
