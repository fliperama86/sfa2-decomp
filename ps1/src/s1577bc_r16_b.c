/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern unsigned data_80181a50[];

unsigned *func_80158374(unsigned *env) {
    if (gpu_dbg_level[0] >= 2) {
        gpu_printf(&str_8016dc70, env);
    }
    func_80158a84(env + 7, env);
    env[7] |= 0xffffff;
    gpu_ops[2](gpu_ops[6], env + 7, 0x40, 0);
    func_8015a540(gpu_dbg_level + 0xe, env, 0x5c);
    return env;
}

void *func_80158438(void *env) {
    func_8015a540(env, gpu_dbg_level + 0xe, 0x5c);
    return env;
}
