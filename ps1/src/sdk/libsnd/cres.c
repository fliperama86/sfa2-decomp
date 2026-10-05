// SPDX-License-Identifier: MIT
#include "common.h"
#include "libsnd_i.h"

/* Adapted: this SDK version's body. unk3E selects the direction (up or
   down), unk42 is the step (a positive value means one unit every unk42
   ticks, otherwise unk42 per tick). Not the small reference form. */
void _SsSndCrescendo(s16 arg0, s16 arg1) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];
    u16 voll, volr;

    score->unk98--;

    if (score->unk42 > 0) {
        if ((score->unk98 % score->unk42) == 0) {
            if (score->unk3E > 0) {
                score->unk40--;
                if (score->unk40 >= 0) {
                    SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
                    if ((voll + 1) < 0x80 && (volr + 1) < 0x80) {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), voll + 1, volr + 1, 0);
                    } else {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                        _ss_score[arg0][arg1].unk90 &= ~0x10;
                    }
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x10;
                }
            } else if (score->unk3E < 0) {
                score->unk40++;
                if (score->unk40 <= 0) {
                    SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
                    if ((voll - 1) >= 0 && (volr - 1) >= 0) {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), voll - 1, volr - 1, 0);
                    } else {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                        _ss_score[arg0][arg1].unk90 &= ~0x10;
                    }
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x10;
                }
            }
            if ((score->unk98 == 0) || (score->unk40 == 0)) {
                _ss_score[arg0][arg1].unk90 &= ~0x10;
            }
        }
    } else {
        if (score->unk3E > 0) {
            score->unk40 += score->unk42;
            SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
            if (score->unk40 >= 0) {
                if ((voll - score->unk42) < 0x80 &&
                    (volr - score->unk42) < 0x80) {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), voll - score->unk42,
                                   volr - score->unk42, 0);
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x10;
                }
            } else {
                SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                _ss_score[arg0][arg1].unk90 &= ~0x10;
            }
        } else if (score->unk3E < 0) {
            score->unk40 -= score->unk42;
            SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
            if (score->unk40 <= 0) {
                if (voll >= -score->unk42 && volr >= -score->unk42) {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), voll + score->unk42,
                                   volr + score->unk42, 0);
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x10;
                }
            } else {
                SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                _ss_score[arg0][arg1].unk90 &= ~0x10;
            }
        }
        if ((score->unk98 == 0) || (score->unk40 == 0)) {
            _ss_score[arg0][arg1].unk90 &= ~0x10;
        }
    }
    SpuVmGetSeqVol(arg0 | (arg1 << 8), &score->unk78, &score->unk7A);
}
