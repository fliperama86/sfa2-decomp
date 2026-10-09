/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_80190464[];
extern int data_8019046c[];

void func_80011358_slot12(void) {
    while (game_state.field_ee == 0) {
        game_state.field_f0 = 0x1f;
        *(u16 *)data_80190464 = 0x2404;
        *(u16 *)data_8019046c = 0x1f00;
        func_80119144(2, 4);
    }
}
