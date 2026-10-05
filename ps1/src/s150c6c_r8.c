/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80151f64(int a, u8 b, s16 c, s16 d) {
    if (b != 0x20) {
        Cmd *cmd = data_8018d13c;
        u8 lo = (b & 0xf) << 3;
        u8 hi = (b & 0xf0) >> 1;
        u16 e = data_8018d208;
        cmd->field_0d = hi;
        cmd->field_0e = e;
        cmd->field_0c = lo;
        cmd->field_08 = c;
        cmd->field_0a = d;
        cmd->field_10 = 8;
        cmd->field_12 = 8;
        func_8015bf34(a, cmd++);
        data_8018d13c = cmd;
    }
}

void func_80151fec(int a, int b, s16 c, s16 d) {
    if ((u8)b != 0x20) {
        int v = b + 0xe0;
        b = v;
        if ((u8)b < 0xe0) {
            Cmd *cmd = data_8018d13c;
            u8 lo = (b & 0xf) << 3;
            u8 hi = ((u8)b & 0xf0) + 0x40;
            u16 e = data_8018d208;
            cmd->field_0d = hi;
            cmd->field_10 = 8;
            cmd->field_0e = e;
            cmd->field_0c = lo;
            cmd->field_08 = c;
            cmd->field_0a = d;
            cmd->field_12 = 0x10;
            func_8015bf34(a, cmd++);
            data_8018d13c = cmd;
        }
    }
}

void func_80152088(int a, int b, s16 c, s16 d) {
    if ((u8)b != 0x20) {
        int v = b + 0xe0;
        if ((u8)v < 0xe0) {
            u8 lo = ((v & 0x7) << 4) | 0x80;
            u8 hi = (v & 0x78) << 1;
            Cmd *cmd = data_8018d13c;
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
}
