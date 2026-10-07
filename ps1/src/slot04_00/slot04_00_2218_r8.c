/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b2ed4_slot04_00(Object *obj);

void func_801b2a90_slot04_00(Object *obj) {
    int a = 0x13;
    int b = 0x33;

    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07++;
        if (obj->field_45 != 0) {
            a = -3;
            b = 0x41;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b2ed4_slot04_00(obj);
    }
    func_80130efc(obj);
}
