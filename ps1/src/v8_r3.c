/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80128bb8(void) {
    Object *right = &player_right;
    func_80128edc(right);
    if (player_right.field_73 != 0) {
        func_80128e1c();
    } else {
        func_80128edc(right - 1);
        if (player_left.field_73 != 0) {
            func_80128e7c();
        } else {
            func_80128d58();
            func_80128d08();
        }
    }
}
