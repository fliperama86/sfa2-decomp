/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801483a4(Object *object, int a_arg, int b_arg);

void func_801b5d3c_slot04_09(Object *obj);

void func_801b4548_slot04_09(Object *obj) {
    obj->field_07++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801b5d3c_slot04_09(obj);
    func_801307e0(obj, 0x2f);
}

void func_801b45a4_slot04_09(Object *obj) {
    u16 t;
    func_80130efc(obj);
    t = obj->field_3a;
    if ((u8)t == 0) {
        obj->field_165 = 0;
        obj->field_07++;
        func_801312b8(obj);
    } else if ((u8)t != 2) {
        obj->field_3a = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0xe, 0x50);
    }
}
