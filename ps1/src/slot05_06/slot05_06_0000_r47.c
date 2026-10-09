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
void func_801cd1f4_slot05_06(Object *obj);
void func_80142fe8(Object *object);

void func_801cbff4_slot05_06(Object *obj) {
    int a;

    obj->field_07 = obj->field_07 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    a = 0x2f;
    if (obj->field_45 == 0) {
        a = 0x2b;
    }
    func_80130678(obj, a);
}

void func_801cc060_slot05_06(Object *obj) {
    s16 a;
    int b;

    if (((Slot04aObj *)obj)->field_3b != 0) {
        obj->field_165 = 0xff;
        ((Slot04aObj *)obj)->field_47 = 0x1e;
        ((Slot04aObj *)obj)->field_46 = 1;
        obj->field_07++;
        if (obj->field_45 == 0) {
            a = -8;
            b = 0x51;
        } else {
            a = -0xa;
            b = 0x6a;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801cd1f4_slot05_06(obj);
    }
    func_80130efc(obj);
}

void func_801cc108_slot05_06(Object *obj) {
    int a;
    ((Slot04aObj *)obj)->field_47--;
    if (((Slot04aObj *)obj)->field_47 == 0) {
        ((Slot04aObj *)obj)->field_47 = 0x14;
        obj->field_07++;
        obj->field_27c = 0;
        ((Slot04aObj *)obj)->field_27d = 0;
        if (obj->field_4b == 0) {
            a = ((Slot04aObj *)obj)->field_c6 + 0x1e;
        } else {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}
