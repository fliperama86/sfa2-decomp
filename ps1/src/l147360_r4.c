/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80148b60(Object *object) {
    int side;
    u16 i;
    Object *o;
    if ((s16)object->health >= 0) {
        if ((get_random_byte() & 0xff) == 0x25) side = 2;
        else side = get_random_byte() & 1;
        i = 0;
        do {
            o = (Object *)func_8011f1e0();
            if (o != 0) {
                o->active_count = 1;
                o->field_02 = 9;
                o->table_index = side;
                o->field_0e = object->field_0e;
                o->countdown = object->kind;
                o->parent = object;
                o->variant_index = data_8017ceb4[i];
                o->active_state = data_8017ceb8[i];
                o->spawn_x = 0x60;
                o->spawn_y = 0x1e0;
                o->data_ptr_a = &data_80172a48;
                o->field_90 = (void *)0x800fb100;
                o->data_ptr_b = &data_80173c9c;
            }
            i++;
        } while (i < 3);
        object->field_29e = 1;
        play_sound_at(object, object->side, 0x322);
    }
}
