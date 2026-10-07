/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c60e8_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];
extern u16 data_801aa618[];

Block172 *func_8011f1e0(void);
void func_80138ae8(GameState *state, Object *object);
void func_801b5088_slot04_0e(Object *obj);
void func_801b6d7c_slot04_0e(Slot04bObj *obj);

void func_801b4b40_slot04_0e(Object *obj) {
    Slot04_0eRec62b4 *r = &data_801c62b4_slot04_0e[obj->side];
    Object *c;
    s16 x;
    s16 lim;

    c = (Object *)func_8011f1e0();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x14;
        c->field_03 = 0;
        c->field_09 = 0;
        c->field_0b = obj->field_0b;
        if (obj->field_0b != 0) {
            x = obj->pos_x + 0x40;
            lim = data_801aa618[1] + 0x160;
            if (lim <= x) {
                x = lim;
            }
            c->pos_y = -8;
        } else {
            x = obj->pos_x - 0x40;
            lim = data_801aa618[0] + 0x20;
            if (lim >= x) {
                x = lim;
            }
            c->pos_y = -8;
        }
        c->field_48 = 0x56;
        c->pos_x = x;
        c->field_3c = obj;
        r->field_14 = c;
        c->field_7a = 0x60;
        c->field_7c = 0x1e0;
        c->field_0d = obj->field_0d;
        c->field_08 = 0x20;
        c->field_90 = obj->field_90;
        c->field_66 = obj->field_66;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}

void func_801b4cb0_slot04_0e(Object *obj) {
    data_801c60e8_slot04_0e[obj->field_07](obj);
}

void func_801b4cf0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;

    obj->field_47 = 0x3c;
    o->field_17b = 1;
    obj->field_1de = 0;
    o->field_07 = o->field_07 + 1;
    func_801b5088_slot04_0e(o);
    func_80141f28(o, 4);
    func_80138ae8(&game_state, o);
    func_801b6d7c_slot04_0e((Slot04bObj *)o);
    a = 0x62;
    if (o->field_49 == 0) {
        a = 0x4c;
    }
    func_801307e0(o, a);
}
