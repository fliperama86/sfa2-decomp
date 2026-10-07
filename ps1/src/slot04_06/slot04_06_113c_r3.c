/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1368_slot04_06(Object *obj) {
    if (*(u32 *)&obj->field_04 == 0x1010101) {
        if (obj->field_45 == 0) {
            if (obj->field_163 == 0) {
                if (*(s16 *)&obj->field_5c >= 0) goto yes;
            }
        }
    }
    return 0;
yes:
    return 1;
}
