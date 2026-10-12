/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b66fc_slot04_06(Object *obj);

void func_801b6554_slot04_06(Object *obj) {
    GameState *g = &game_state;

    if ((func_801b66fc_slot04_06(obj) << 16) >= 0) {
        obj->state_0++;
    }
    if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
        ((Slot04aObj *)obj)->timer -= 1;
        if ((((Slot04aObj *)obj)->timer & 0x80) == 0) {
            func_8011ffdc(obj);
            return;
        }
    }
    if (g->frame_counter & 1) {
        func_8011ffdc(obj);
    }
}
