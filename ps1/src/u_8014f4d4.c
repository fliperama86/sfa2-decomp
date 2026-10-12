/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f4d4(int a, int arg) {
    s16 b = arg;
    Menu *m = &data_80190948;
    switch (a) {
    case 1:
        m->fade_mode = 0;
        m->active_flag = 0;
        m->flag_a = 0;
        m->flag_b = 1;
        func_80150b80(m, b);
        break;
    case 2:
        m->flag_a = 1;
        set_cd_volume(0);
        break;
    case 3:
        m->flag_a = 0;
        set_cd_volume(0x55);
        break;
    case 4:
        m->command_state = 4;
        m->fade_mode = 0;
        break;
    case 5:
        m->fade_mode = 1;
        m->fade_step = b;
        break;
    case 6:
        m->fade_mode = 2;
        m->fade_step = b;
        break;
    }
}
