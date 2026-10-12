/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int is_all_states_one_and_healthy_slot04_06(Object *obj) {
    if (*(u32 *)&obj->state_0 == 0x1010101) {
        if (obj->active_state == 0) {
            if (obj->field_163 == 0) {
                if (*(s16 *)&obj->health >= 0) goto yes;
            }
        }
    }
    return 0;
yes:
    return 1;
}
