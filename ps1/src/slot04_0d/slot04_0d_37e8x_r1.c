/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2828_slot04_0d[];
extern u8 data_801c2848_slot04_0d[];

void func_80130678(Object *object, int arg);

void func_801b37e8_slot04_0d(Object *object) {
    u16 a;
    ((Slot04bObj *)object)->field_46 = 0x3c;
    object->field_06 = object->field_06 + 1;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    a = object->field_c2 & 0xf000;
    if (((Slot04bObj *)object)->field_5c >= 0x90 && a == 0x1000) {
        a = 1;
    } else {
        a = func_80151184();
        a &= 0x1f;
        if (((Slot04bObj *)object)->field_5c < 0x80) {
            a = data_801c2828_slot04_0d[a];
        } else {
            a = data_801c2848_slot04_0d[a];
        }
    }
    a = (s8)func_80125734(object, (s8)a); object->field_12c = a; a += 0x23; func_80130678(object, (s16)a);
}
