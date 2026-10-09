/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b3a5c_slot04_03(Object *obj);

void func_801b3010_slot04_03(Object *obj) {
    int a = -6;
    int b = 0x4c;

    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07++;
        if (obj->field_45 != 0) {
            a = -2;
            b = 0x4e;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3a5c_slot04_03(obj);
    }
    func_80130efc(obj);
}
