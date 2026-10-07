/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80130678(Object *object, int arg);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b4128_slot04_07(Object *obj);

void func_801b3408_slot04_07(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        func_80130678(obj, 0x2f);
    } else {
        func_80130678(obj, 0x2b);
    }
}

void func_801b347c_slot04_07(Object *obj) {
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            func_801495f8(obj, -2, 0x4a);
        } else {
            func_801495f8(obj, -0x1b, 0x69);
        }
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b4128_slot04_07(obj);
    }
    func_80130efc(obj);
}
