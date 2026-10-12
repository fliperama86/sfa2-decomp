/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void add_16x16_sprite_prim_from_byte(int a, u8 b, s16 c, s16 d) {
    if (b != 0) {
        Cmd *cmd = data_8018d13c;
        u8 lo = (b & 0xf) << 4;
        u8 hi = b & 0xf0;
        u16 e = data_8018d208;
        cmd->tex_u = lo;
        cmd->clut_word = e;
        cmd->tex_v = hi;
        cmd->pos_x = c;
        cmd->pos_y = d;
        cmd->width = 0x10;
        cmd->height = 0x10;
        func_8015bf34(a, cmd++);
        data_8018d13c = cmd;
    }
}

void func_80152220(Block172 *block) {
    Object *object = (Object *)block;
    table_8018023c[object->state_0](object);
}
