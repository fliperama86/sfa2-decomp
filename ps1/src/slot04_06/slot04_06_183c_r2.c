/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c535c_slot04_06[];
extern u8 data_801c5368_slot04_06[];
void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);
void func_80146960(Object *object);
int func_80140cd8(Object *object, int a, int b);

void func_801b1944_slot04_06(Object *o) {
    u8 t;
    GameState *g;
    func_801b4850_slot04_06(o);
    g = &game_state;
    if (o->pos_y < o->field_70) {
        func_801b4818_slot04_06(o);
        func_80130efc(o);
    } else {
        o->field_07++;
        func_80146960(o);
        t = o->field_12a >> 1;
        if (o->field_49 != 0) {
            t += 3;
        }
        if ((u8)func_80140cd8(o, data_801c535c_slot04_06[t], 0) != 0) {
            func_80120554(o, o->side, 0x319);
            if (o->field_49 != 0) {
                o->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(o);
            }
        } else {
            func_80120554(o, o->side, 0x319);
        }
        o->pos_y = o->field_70;
        o->field_45 = 0;
        ((Slot04aObj *)o)->field_1c2 = 0x10;
        g->field_63 = data_801c5368_slot04_06[o->field_12a >> 1];
        func_801307e0(o, 0x21);
    }
}

void func_801b1a88_slot04_06(Object *o) {
    ((Slot04aObj *)o)->field_1c2 += 0xff;
    if (((Slot04aObj *)o)->field_1c2 & 0x80) {
        o->field_07++;
    }
    func_80130efc(o);
}
