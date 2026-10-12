/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c6568_slot04_02[];
extern u16 data_801c6578_slot04_02[];

void func_801b5f50_slot04_02(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b5f84_slot04_02(Object *obj) {
    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void func_801b5fd4_slot04_02(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->field_46 = 0x3c;
    game_state.field_76 = 0x1e;
    func_80130678(obj, data_801c6578_slot04_02[(u8)func_80125734(obj, data_801c6568_slot04_02[get_random_byte() & 0xf])]);
}
