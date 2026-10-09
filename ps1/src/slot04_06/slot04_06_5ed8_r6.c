/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b66fc_slot04_06(Object *obj);

void func_801b6554_slot04_06(Object *obj) {
    GameState *g = &game_state;

    if ((func_801b66fc_slot04_06(obj) << 16) >= 0) {
        obj->field_04++;
    }
    if ((((Slot04aObj *)obj)->field_47 & 0x80) == 0) {
        ((Slot04aObj *)obj)->field_47 -= 1;
        if ((((Slot04aObj *)obj)->field_47 & 0x80) == 0) {
            func_8011ffdc(obj);
            return;
        }
    }
    if (g->field_1d & 1) {
        func_8011ffdc(obj);
    }
}
