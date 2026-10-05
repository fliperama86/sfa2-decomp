/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801521a0(int a, u8 b, s16 c, s16 d) {
    if (b != 0) {
        Cmd *cmd = data_8018d13c;
        u8 lo = (b & 0xf) << 4;
        u8 hi = b & 0xf0;
        u16 e = data_8018d208;
        cmd->field_0c = lo;
        cmd->field_0e = e;
        cmd->field_0d = hi;
        cmd->field_08 = c;
        cmd->field_0a = d;
        cmd->field_10 = 0x10;
        cmd->field_12 = 0x10;
        func_8015bf34(a, cmd++);
        data_8018d13c = cmd;
    }
}

void func_80152220(Object *object) {
    table_8018023c[object->field_04](object);
}
