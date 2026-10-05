/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


GlyphSet *func_8015a618(GlyphSet *g) {
    int r = func_8015ab7c(data_8018d3c8, g);
    if (r < 0) {
        return 0;
    }
    g->table1 = data_8018d3c0;
    g->table0 = data_8018d3c4;
    g->out_40 = data_8018d3c4[g->idx_70].a;
    g->out_42 = data_8018d3c4[g->idx_70].b;
    g->out_44 = data_8018d3c4[g->idx_70].c;
    g->out_48 = data_8018d3c4[g->idx_72].a;
    g->out_4a = data_8018d3c4[g->idx_72].b;
    g->out_4c = data_8018d3c4[g->idx_72].c;
    g->out_50 = data_8018d3c4[g->idx_74].a;
    g->out_52 = data_8018d3c4[g->idx_74].b;
    g->out_54 = data_8018d3c4[g->idx_74].c;
    data_8018d3c8 += r;
    g->out_58 = data_8018d3c4[g->idx_76].a;
    g->out_5a = data_8018d3c4[g->idx_76].b;
    g->out_5c = data_8018d3c4[g->idx_76].c;
    g->out_20 = data_8018d3c0[g->idx_68].a;
    g->out_22 = data_8018d3c0[g->idx_68].b;
    g->out_24 = data_8018d3c0[g->idx_68].c;
    g->out_28 = data_8018d3c0[g->idx_6a].a;
    g->out_2a = data_8018d3c0[g->idx_6a].b;
    g->out_2c = data_8018d3c0[g->idx_6a].c;
    g->out_30 = data_8018d3c0[g->idx_6c].a;
    g->out_32 = data_8018d3c0[g->idx_6c].b;
    g->out_34 = data_8018d3c0[g->idx_6c].c;
    g->out_38 = data_8018d3c0[g->idx_6e].a;
    g->out_3a = data_8018d3c0[g->idx_6e].b;
    g->out_3c = data_8018d3c0[g->idx_6e].c;
    return g;
}
