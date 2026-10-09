/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80130678(Object *object, int index);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_801b3530_slot04_04(Object *obj);

void func_801b3294_slot04_04(Object *obj) {
    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        func_80130678(obj, 0x2f);
    } else {
        func_801204f4(obj, obj->side, 8);
        func_80130678(obj, 0x2b);
    }
}

void func_801b3318_slot04_04(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->field_3a & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = -3;
            b = 0x50;
        } else {
            a = -7;
            b = 0x47;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3530_slot04_04(obj);
    }
    func_80130efc(obj);
}
