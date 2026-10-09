/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
extern u16 data_801a6984;
void func_801257d0(Attacker *unused);

void func_80122630(GameState *state) {
    Attacker *a = (Attacker *)state;
    int mode;
    if (a->field_04 != 0) {
        a->field_6d = 0;
        a->field_2c = 0xff;
        a->field_4e = 0xff;
        return;
    }
    if ((a->field_30 == 0 || data_801a6984 != 0) && (a->field_49 & 0x80) == 0 &&
        *(u16 *)&a->field_64 == 0 && a->field_4c == 0) {
        a->field_4a--;
        if (a->field_4a == 0) {
            a->field_4a = 0x3c;
            if (a->field_49 == 0) {
                a->field_49 = 0;
                a->field_4d = 0xff;
                func_80123968(a);
            } else {
                a->field_49--;
            }
        }
    }
    mode = a->field_4c;
    if (mode != 0) {
        if (mode == 3) {
            a->field_64 = 0;
            a->field_a6 = 1;
        } else {
            func_801227d4(a, mode);
            func_801229b4(a);
        }
        if (a->field_4d == 0) {
            a->field_5c = 0x3c;
            a->field_5d = 3;
            a->field_5e = 3;
        }
        data_8018f5a0->field_4e++;
        data_8018f5a0->field_50 = 0;
        a->field_ca = 0;
        a->field_09 = 1;
        a->field_47 = 1;
        a->field_4e = 1;
        a->field_6d = 0;
        a->field_76 = 0;
        player_left.field_165 = 0;
        player_right.field_165 = 0;
        func_80138964(a);
        func_801257d0(a);
    }
}
