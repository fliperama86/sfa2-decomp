/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b4310_slot04_08(Object *obj);

void func_801b3828_slot04_08(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07++;
        func_801495f8(obj, -0xb, 0x6a);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b4310_slot04_08(obj);
    }
    func_80130efc(obj);
}
