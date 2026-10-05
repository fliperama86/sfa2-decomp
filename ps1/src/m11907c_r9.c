/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern s8 data_801a6984;
extern u16 data_801a6966;
void func_8011f674(Object *o, void *p);

void func_8011f5a0(void) {
    Object *p;
    player_left.field_c4 = player_left.field_c2;
    player_right.field_c4 = player_right.field_c2;
    if (game_state.field_30 == 0 || data_801a6984 == 0) {
        player_left.field_c2 = data_801a6966;
        player_right.field_c2 = data_801a6972;
    }
    if (game_state.field_31 == 0) {
        p = &player_left;
        func_8011f774(p);
        func_8011f674(p, table_80185cf4);
        p++;
        func_8011f774(p);
        func_8011f674(p, table_80185e50);
    }
}
