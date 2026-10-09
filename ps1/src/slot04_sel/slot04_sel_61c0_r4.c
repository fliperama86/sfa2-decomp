/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b9db8_slot04_sel[];
extern u8 data_801b9c20_slot04_sel;
extern void (*data_801b9c24_slot04_sel[])(Object *, u8 *);
extern u8 *data_801b9cd8_slot04_sel[];

void func_801b6654_slot04_sel(void) {
    data_801b9db8_slot04_sel[0] = 0;
    data_801b9db8_slot04_sel[1] = 0;
    data_801b9c20_slot04_sel = 0;
}

void func_801b6674_slot04_sel(void) {
    Object *l = &player_left;
    Object *r = l + 1;
    u8 *b = data_801b9db8_slot04_sel;
    data_801b9c24_slot04_sel[0](l, b);
    data_801b9c24_slot04_sel[0](r, b + 1);
    if (game_state.field_07 != 0) {
        if (b[0] != 0 || b[1] != 0) {
            data_8018f5a0->field_4a = 1;
        }
    }
    func_801519b4(data_801b9cd8_slot04_sel);
}
