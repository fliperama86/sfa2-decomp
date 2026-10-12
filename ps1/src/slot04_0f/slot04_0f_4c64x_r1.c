/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801716b4[];

u16 *func_801b4c64_slot04_0f(Object *obj);

u16 *func_801b4c64_slot04_0f(Object *obj) {
    u8 a = obj->input_dir_0 ? 12 : 0;
    if (obj->input_dir_second != 0) {
        a += 6;
    }
    return (u16 *)(data_801716b4 + ((a + obj->direction_index) & 0xfe));
}
