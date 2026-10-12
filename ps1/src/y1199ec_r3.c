/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80119e74(AnimObj *obj, u16 n) {
    void *p90 = obj->compressed_src;
    u16 idx = obj->stream_index;
    int col = data_80183910[idx];
    u16 row = data_801839f0[idx];
    u16 cnt = data_80183ad0[idx];
    Rect r;
    Job *job;
    int prev;
    unsigned int i;

    r.w = 4;
    r.h = 0x10;
    for (i = 0; i < n; i++) {
        job = (Job *) func_8011f430();
        if (job == 0) {
            break;
        }
        prev = col & 0xffff;
        decode_compressed_block(p90, data_80184704[prev][row], job->src);
        r.x = col * 4 + 0x180;
        r.y = row << 4;
        job->kind = 1;
        job->index = idx;
        job->rect = r;
        row = (row + 1) & 0xf;
        if (row == 0) {
            col = data_80183c90[prev];
        }
        cnt++;
    }
    data_80183910[idx] = col;
    data_801839f0[idx] = row;
    data_80183ad0[idx] = cnt;
}
