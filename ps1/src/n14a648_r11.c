/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void clear_menu_fade_mode(Menu *menu) {
    menu->fade_mode = 0;
    menu->field_01 = 0;
}
