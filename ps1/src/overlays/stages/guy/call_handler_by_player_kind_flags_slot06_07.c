/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 is_kind_in_special_set_slot06_07(Object *obj);

extern Object *(*data_801ee1e8_slot06_07[])(void);

Object *call_handler_by_player_kind_flags_slot06_07(void) {
    Object *l = &player_left;
    u8 i;
    i = is_kind_in_special_set_slot06_07(l) != 0;
    if (is_kind_in_special_set_slot06_07(l + 1) != 0) {
        i |= 2;
    }
    return data_801ee1e8_slot06_07[i]();
}
