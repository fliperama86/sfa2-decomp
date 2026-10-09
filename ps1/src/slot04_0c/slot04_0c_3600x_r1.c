/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801bdb28_slot04_0c[];
extern s8 data_801bdb48_slot04_0c[];
extern s8 data_801bdb68_slot04_0c[];

u8 func_80125734(Object *object, s8 a);
void func_80130678(Object *object, int arg);

void func_801b3600_slot04_0c(Object *object) {
    int a;

    object->field_06 = object->field_06 + 1;
    object->field_46 = 0x3c;
    if (game_state.field_76 == 0) {
        game_state.field_76 = 0x1e;
    }
    if (object->other->kind == 0xb) {
        if (game_state.field_90 != 0) {
            func_80130678(object, 0x26);
        } else {
            func_80130678(object, (s8)func_80125734(object, data_801bdb68_slot04_0c[func_80151184() & 0x1f]) + 0x23);
        }
    } else if (((Slot04bObj *)object)->field_5c >= 0x90 && (func_80151184() & 0x3f) != 0) {
        func_80130678(object, (s8)func_80125734(object, 2) + 0x23);
    } else {
        a = func_80151184() & 0x1f;
        if (((Slot04bObj *)object)->field_5c >= 0x80) a = data_801bdb48_slot04_0c[a]; else a = data_801bdb28_slot04_0c[a];
        func_80130678(object, (s8)func_80125734(object, a) + 0x23);
    }
}
