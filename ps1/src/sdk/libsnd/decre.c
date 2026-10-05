// SPDX-License-Identifier: MIT
#include "common.h"
#include "libsnd_i.h"

/* Adapted: this SDK version's routine is larger than the reference's. It
   steps the volume of a sequence down or up (by unk3E sign and unk42 sign)
   and clears the 0x20 flag when the fade ends. */
void _SsSndDecrescendo(s16 arg0, s16 arg1) {
    struct SeqStruct* score = &_ss_score[arg0][arg1];
    u16 voll, volr;

    score->unk98--;

    if (score->unk42 > 0) {
        if ((score->unk98 % score->unk42) == 0) {
            if (score->unk3E > 0) {
                score->unk40--;
                if (score->unk40 >= 0) {
                    SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
                    if (voll != 0 && volr != 0) {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), voll - 1, volr - 1, 0);
                    } else {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                        _ss_score[arg0][arg1].unk90 &= ~0x20;
                    }
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x20;
                }
            } else if (score->unk3E < 0) {
                score->unk40++;
                if (score->unk40 <= 0) {
                    SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
                    if (voll + 1 < 0x80 && volr + 1 < 0x80) {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), voll + 1, volr + 1, 0);
                    } else {
                        SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                        _ss_score[arg0][arg1].unk90 &= ~0x20;
                    }
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x20;
                }
            }
            if ((score->unk98 == 0) || (score->unk40 == 0)) {
                _ss_score[arg0][arg1].unk90 &= ~0x20;
            }
        }
    } else {
        if (score->unk3E > 0) {
            score->unk40 += score->unk42;
            if (score->unk40 >= 0) {
                SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
                if (voll >= -score->unk42 && volr >= -score->unk42) {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), voll + score->unk42,
                                   volr + score->unk42, 0);
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x20;
                }
            } else {
                SpuVmSetSeqVol(arg0 | (arg1 << 8), 0, 0, 0);
                _ss_score[arg0][arg1].unk90 &= ~0x20;
            }
        } else if (score->unk3E < 0) {
            score->unk40 -= score->unk42;
            if (score->unk40 <= 0) {
                SpuVmGetSeqVol(arg0 | (arg1 << 8), &voll, &volr);
                if (voll - score->unk42 < 0x80 && volr - score->unk42 < 0x80) {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), voll - score->unk42,
                                   volr - score->unk42, 0);
                } else {
                    SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                    _ss_score[arg0][arg1].unk90 &= ~0x20;
                }
            } else {
                SpuVmSetSeqVol(arg0 | (arg1 << 8), 0x7F, 0x7F, 0);
                _ss_score[arg0][arg1].unk90 &= ~0x20;
            }
        }
        if ((score->unk98 == 0) || (score->unk40 == 0)) {
            _ss_score[arg0][arg1].unk90 &= ~0x20;
        }
    }
    SpuVmGetSeqVol(arg0 | (arg1 << 8), &score->unk78, &score->unk7A);
}
