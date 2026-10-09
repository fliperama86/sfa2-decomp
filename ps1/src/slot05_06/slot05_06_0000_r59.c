/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);
void func_801cd258_slot05_06(Object *obj);
void func_801cd6d8_slot05_06(Object *obj);

void func_801cd0e4_slot05_06(Object *obj) {
    GameState *g = &game_state;
    Object *p;

    if (((Slot04aObj *)obj)->field_3a != 0) {
        ((Slot04aObj *)obj)->field_3a = 0;
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->field_00 = 1;
            p->field_03 = 5;
            p->field_3c = obj;
            p->field_7a = obj->field_7a;
            p->field_7c = obj->field_7c;
            p->field_0d = obj->field_0d;
            p->field_02 = 0x11;
            p->field_08 = 0x20;
            p->field_90 = obj->field_90;
            p->field_66 = obj->field_66;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
        }
    }
    if (((Slot04aObj *)obj)->field_47 != 0) {
        ((Slot04aObj *)obj)->field_47--;
        if (((Slot04aObj *)obj)->field_47 == 0) {
            g->field_4b |= 1 << ((Slot04aObj *)obj)->field_a6;
        }
    }
    func_80130efc(obj);
}

void func_801cd1f4_slot05_06(Object *obj) {
    int i;
    u8 z = 0;
    u8 *p = (u8 *)&obj->slots;

    for (i = 0x47; i >= 0; i--) {
        *p++ = z;
    }
}

void func_801cd218_slot05_06(Object *obj) {
    if (obj->field_128 != 0) {
        func_801cd6d8_slot05_06(obj);
    } else {
        func_801cd258_slot05_06(obj);
    }
}
