/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_801b9c38_slot04_sel[];
extern u8 data_801b9db0_slot04_sel;
extern u8 data_801b9db4_slot04_sel;

void move_selection_left_right_with_sound_slot04_sel(Object *obj, u8 *rec, int n) {
    if ((*data_801b9c38_slot04_sel[obj->side] & 0xf0) != 0) {
        data_801b9db4_slot04_sel = 1 << obj->side;
    }
    if (data_801b9db4_slot04_sel == 0) {
        if ((*data_801b9c38_slot04_sel[obj->side] & 0x8000) != 0) {
            if (data_801b9db0_slot04_sel == 0) {
                play_sound_at(0, obj->side, 0);
                data_801b9db0_slot04_sel = n;
            } else {
                play_sound_at(0, obj->side, 0);
                data_801b9db0_slot04_sel--;
            }
        }
        if ((*data_801b9c38_slot04_sel[obj->side] & 0x2000) != 0) {
            if (data_801b9db0_slot04_sel == (u16)n) {
                play_sound_at(0, obj->side, 0);
                data_801b9db0_slot04_sel = 0;
            } else {
                play_sound_at(0, obj->side, 0);
                data_801b9db0_slot04_sel = data_801b9db0_slot04_sel + 1;
            }
        }
    }
}
