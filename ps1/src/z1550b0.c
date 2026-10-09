/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801550b0(void) {
    u8 *tab;
    u16 *pad, *pad2;
    u8 sel, k, found, i;

    tab = table_8016e664;
    func_801519b4(&data_801812b4);
    func_801519b4(&data_801812c4);
    func_801519b4(&data_801812d4);
    func_801519b4(&data_80181324);
    func_801519b4(&data_80181334);
    if (game_state.field_31 != 0) {
        if ((data_801a696a & 0x800) || (data_801a6976 & 0x800)) {
            ((HudBig *)data_8018f5a0)->field_52 = 0;
        }
    }

    pad = &data_801a696a;
    if (*pad & 0x4000) {
        data_8018125e[data_8018d250 * 5 + 1] = *(tab + data_8018d250 + 0x10);
        data_80181094[data_8018d250 * 14 + 1] = 0x1b;
        data_8018d250 = (data_8018d250 + 1) & 7;
        data_80181094[data_8018d250 * 14 + 1] = 0x1a;
        func_80120554(0, 0, 0x34d);
    }
    if (*pad & 0x1000) {
        data_8018125e[data_8018d250 * 5 + 1] = *(tab + data_8018d250 + 0x10);
        data_80181094[data_8018d250 * 14 + 1] = 0x1b;
        data_8018d250 = (data_8018d250 - 1) & 7;
        data_80181094[data_8018d250 * 14 + 1] = 0x1a;
        func_80120554(0, 0, 0x34d);
    }
    if (*pad & 0xff) {
        if (*pad & 0x80) { sel = 0x90; k = 0; }
        if (*pad & 0x10) { sel = 0x92; k = 1; }
        if (*pad & 0x04) { sel = 0x95; k = 2; }
        if (*pad & 0x40) { sel = 0x93; k = 3; }
        if (*pad & 0x20) { sel = 0x91; k = 4; }
        if (*pad & 0x08) { sel = 0x94; k = 5; }
        if (*pad & 0x01) { sel = 0x97; k = 6; }
        if (*pad & 0x02) { sel = 0x96; k = 7; }
        i = 0;
        do {
            if (sel == *(tab + i + 0x10)) data_8018d258 = i;
            if (data_80181344[data_8018d250] == tab[i + 0]) found = i;
            i++;
        } while (i < 8);
        if (sel == 0x94 || sel == 0x95 || sel == 0x96 || sel == 0x97) {
            data_8018125e[data_8018d250 * 5] = 0x1a;
        } else {
            data_8018125e[data_8018d250 * 5] = 0x1d;
        }
        if (*(tab + data_8018d250 + 0x10) == 0x94 || *(tab + data_8018d250 + 0x10) == 0x95 || *(tab + data_8018d250 + 0x10) == 0x96 || *(tab + data_8018d250 + 0x10) == 0x97) {
            data_8018125e[data_8018d258 * 5] = 0x1a;
        } else {
            data_8018125e[data_8018d258 * 5] = 0x1d;
        }
        *(tab + data_8018d258 + 0x10) = *(tab + data_8018d250 + 0x10);
        *(tab + data_8018d250 + 0x10) = sel;
        sel = tab[k + 0];
        tab[k + 0] = data_80181344[data_8018d250];
        tab[found + 0] = sel;
        data_8018125e[data_8018d258 * 5 + 1] = *(tab + data_8018d258 + 0x10);
        data_8018125e[data_8018d250 * 5 + 1] = *(tab + data_8018d250 + 0x10);
        data_8018d258 = data_8018d250;
        func_80120554(0, 0, 0x34c);
    }

    pad2 = &data_801a6976;
    if (*pad2 & 0x4000) {
        data_8018128a[data_8018d254 * 5 + 1] = *(tab + data_8018d254 + 0x18);
        data_80181138[data_8018d254 * 14 + 1] = 0x1b;
        data_8018d254 = (data_8018d254 + 1) & 7;
        data_80181138[data_8018d254 * 14 + 1] = 0x1a;
        func_80120554(0, 0, 0x34d);
    }
    if (*pad2 & 0x1000) {
        data_8018128a[data_8018d254 * 5 + 1] = *(tab + data_8018d254 + 0x18);
        data_80181138[data_8018d254 * 14 + 1] = 0x1b;
        data_8018d254 = (data_8018d254 - 1) & 7;
        data_80181138[data_8018d254 * 14 + 1] = 0x1a;
        func_80120554(0, 0, 0x34d);
    }
    if (*pad2 & 0xff) {
        if (*pad2 & 0x80) { sel = 0x90; k = 0; }
        if (*pad2 & 0x10) { sel = 0x92; k = 1; }
        if (*pad2 & 0x04) { sel = 0x95; k = 2; }
        if (*pad2 & 0x40) { sel = 0x93; k = 3; }
        if (*pad2 & 0x20) { sel = 0x91; k = 4; }
        if (*pad2 & 0x08) { sel = 0x94; k = 5; }
        if (*pad2 & 0x01) { sel = 0x97; k = 6; }
        if (*pad2 & 0x02) { sel = 0x96; k = 7; }
        i = 0;
        do {
            if (sel == *(tab + i + 0x18)) data_8018d25c = i;
            if (data_80181344[data_8018d254] == tab[i + 8]) found = i;
            i++;
        } while (i < 8);
        if (sel == 0x94 || sel == 0x95 || sel == 0x96 || sel == 0x97) {
            data_8018128a[data_8018d254 * 5] = 0x1a;
        } else {
            data_8018128a[data_8018d254 * 5] = 0x1d;
        }
        if (*(tab + data_8018d254 + 0x18) == 0x94 || *(tab + data_8018d254 + 0x18) == 0x95 || *(tab + data_8018d254 + 0x18) == 0x96 || *(tab + data_8018d254 + 0x18) == 0x97) {
            data_8018128a[data_8018d25c * 5] = 0x1a;
        } else {
            data_8018128a[data_8018d25c * 5] = 0x1d;
        }
        *(tab + data_8018d25c + 0x18) = *(tab + data_8018d254 + 0x18);
        *(tab + data_8018d254 + 0x18) = sel;
        sel = *(tab + k + 8);
        *(tab + k + 8) = data_80181344[data_8018d254];
        *(tab + found + 8) = sel;
        data_8018128a[data_8018d25c * 5 + 1] = *(tab + data_8018d25c + 0x18);
        data_8018128a[data_8018d254 * 5 + 1] = *(tab + data_8018d254 + 0x18);
        data_8018d25c = data_8018d254;
        func_80120554(0, 0, 0x34c);
    }
}
