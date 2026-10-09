/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80130678(Object *object, int arg);

void func_801b3ff8_slot04_06(Object *obj) {
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
