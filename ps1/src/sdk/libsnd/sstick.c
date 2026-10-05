// SPDX-License-Identifier: MIT
#include "libsnd_i.h"

/* Adapted: separate variables instead of struct SndSeqTickEnv, and the video
   mode is a variable set by _SsInit (see ssstart.c); names are not original. */
extern s32 _snd_tick_mode;
extern s32 _snd_tick_flag;
extern s32 _snd_video_mode;

void SsSetTickMode(long tick_mode) {
    if (tick_mode & 0x1000) {
        _snd_tick_flag = 1;
        _snd_tick_mode = tick_mode & 0xFFF;
    } else {
        _snd_tick_flag = 0;
        _snd_tick_mode = tick_mode;
    }
    if (_snd_tick_mode < 6) {
        switch (_snd_tick_mode) {
        case 4:
            VBLANK_MINUS = 50;
            if (_snd_video_mode != 1) {
                _snd_tick_mode = 50;
            } else {
                _snd_tick_mode = 5;
            }
            return;
        case 1:
            VBLANK_MINUS = 60;
            if (_snd_video_mode == 0) {
                _snd_tick_mode = 5;
            } else {
                _snd_tick_mode = 60;
            }
            return;
        case 3:
            VBLANK_MINUS = 120;
            return;
        case 2:
            VBLANK_MINUS = 240;
            return;
        case 5:
            if (_snd_video_mode == 0) {
                VBLANK_MINUS = 60;
            } else if (_snd_video_mode == 1) {
                VBLANK_MINUS = 50;
            } else {
                VBLANK_MINUS = 60;
            }
            break;
        case 0:
            if (_snd_video_mode == 0) {
                VBLANK_MINUS = 60;
            } else if (_snd_video_mode == 1) {
                VBLANK_MINUS = 50;
            } else {
                VBLANK_MINUS = 60;
            }
            return;
        default:
            VBLANK_MINUS = 60;
            return;
        }
    } else {
        VBLANK_MINUS = _snd_tick_mode;
    }
}
