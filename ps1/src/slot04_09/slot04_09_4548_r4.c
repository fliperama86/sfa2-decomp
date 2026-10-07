/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80138b38(GameState *state, Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80130678(Object *object, int arg);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);

void func_801b5348_slot04_09(Object *obj);

void func_801b484c_slot04_09(Object *obj) {
    int a = 0x2b;
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    func_801204f4(obj, obj->side, 7);
    if (obj->field_45 != 0) {
        a = 0x2f;
    }
    func_80130678(obj, a);
}

void func_801b48d4_slot04_09(Object *obj) {
    int a;
    if ((s16)obj->field_3a & 0xff00) {
        a = 0xa;
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = -7;
        }
        func_801495f8(obj, a, 0x50);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b5348_slot04_09(obj);
    }
    func_80130efc(obj);
}
