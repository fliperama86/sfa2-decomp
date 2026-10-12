/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_800f013c_slot0f[];
extern u16 data_800f0164_slot0f[];

void func_800e72bc_slot0f(Object *obj) {
    int i = obj->table_index;
    if (i != 5) {
        i <<= 2;
        if (data_80190468.p->field_01 == 0) {
            obj->base_x = data_800f013c_slot0f[i];
            obj->base_y = data_800f013c_slot0f[i + 1];
            obj->spawn_x = data_800f013c_slot0f[i + 2];
            obj->spawn_y = data_800f013c_slot0f[i + 3];
        } else {
            obj->base_x = data_800f0164_slot0f[i];
            obj->base_y = data_800f0164_slot0f[i + 1];
            obj->spawn_x = data_800f0164_slot0f[i + 2];
            obj->spawn_y = data_800f0164_slot0f[i + 3];
        }
    }
}
