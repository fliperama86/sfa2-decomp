#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80128b28(void) {
    Object *left = &player_left;
    func_80128edc(left);
    if (player_left.field_73 != 0) {
        func_80128e7c();
    } else {
        func_80128edc(left + 1);
        if (player_right.field_73 != 0) {
            func_80128e1c();
        } else {
            func_80128d58();
            func_80128d08();
        }
    }
}
