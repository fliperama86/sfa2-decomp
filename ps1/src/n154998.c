/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;
extern u16 data_801a6984;

#define PRESSED(m) ((g->field_31 == 1 && (data_801a696a & (m))) || \
                    (g->field_31 == 2 && (data_801a6976 & (m))))

void func_80154998(void) {
    GameState *g = &game_state;
    if (g->field_30 == 0) {
        func_801519b4(&data_801812e4);
        if (PRESSED(0x20)) {
            if (data_8018d260 == 0) {
                data_8018f5a0->field_52 = 0;
            } else if (data_8018d260 == 1) {
                data_8018f5a0->field_52 = 3;
            } else if (data_8018d260 == 2) {
                data_8018f5a0->field_52 = 4;
            }
            func_80120554(0, 0, 0x34c);
        }
        if (PRESSED(0x4000)) {
            data_801811aa[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 1;
            if (data_8018d260 >= 3) {
                data_8018d260 = 0;
            }
            data_801811aa[data_8018d260 * 16] = 0x14;
            func_80120554(0, 0, 0x34d);
        }
        if (PRESSED(0x1000)) {
            data_801811aa[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 255;
            if (data_8018d260 & 0x80) {
                data_8018d260 = 2;
            }
            data_801811aa[data_8018d260 * 16] = 0x14;
            func_80120554(0, 0, 0x34d);
        }
    } else if (data_801a6984 == 0) {
        func_801519b4(&data_80181304);
        if (PRESSED(0x40)) {
            func_80155088();
            func_80120554(0, 0, 0x34c);
        } else if (PRESSED(0x20)) {
            if (data_8018d260 == 0) {
                func_80155088();
            } else if (data_8018d260 == 1) {
                data_8018f5a0->field_52 = 3;
            } else if (data_8018d260 == 2) {
                func_80155088();
                g->field_65 = 0xff;
                data_80197f1c = 0xff;
                func_80122e54(&game_state);
            } else if (data_8018d260 == 3) {
                data_8018f5a0->field_52 = 4;
            }
            func_80120554(0, 0, 0x34c);
        } else if (PRESSED(0x4000)) {
            data_80181206[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 1;
            if (data_8018d260 >= 4) {
                data_8018d260 = 0;
            }
            data_80181206[data_8018d260 * 16] = 0x14;
            func_80120554(0, 0, 0x34d);
        } else if (PRESSED(0x1000)) {
            data_80181206[data_8018d260 * 16] = 0x1a;
            data_8018d260 = data_8018d260 + 255;
            if (data_8018d260 & 0x80) {
                data_8018d260 = 3;
            }
            data_80181206[data_8018d260 * 16] = 0x14;
            func_80120554(0, 0, 0x34d);
        }
    } else {
        func_801519b4(&data_80181314);
        if (PRESSED(0x20)) {
            if (data_8018d260 == 0) {
                func_80155088();
                g->field_65 = 0xff;
                data_80197f1c = 0xff;
                func_80122e54(&game_state);
            } else if (data_8018d260 == 1) {
                data_8018f5a0->field_52 = 4;
            }
            func_80120554(0, 0, 0x34c);
        } else if (PRESSED(0x5000)) {
            data_8018124a[data_8018d260 * 9] = 0x1a;
            data_8018d260 = (data_8018d260 + 1) & 1;
            data_8018124a[data_8018d260 * 9] = 0x14;
            func_80120554(0, 0, 0x34d);
        }
    }
}
