/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_8007ee64_slot2b[];

/*
 * What it does (inferred, not an original name): spawns a child effect of
 * an object. It takes a free object from the pool and, when it got one,
 * fills it in with fixed values and copies of the spawning object's fields.
 * The child's position is the spawning object's position plus an offset pair
 * from data_8007ee64_slot2b, chosen by a random even index below 32. Then it
 * calls play_side_sound_at with the spawning object, the parent's side and 0xf.
 */
void func_800796e4_slot2b(Object *obj) {
    Object *parent = obj->field_3c;
    /* func_8011f1e0 is declared in protos.h as returning Block172 *; without the cast the build warns (incompatible pointer type), same code. */
    Object *child = (Object *)func_8011f1e0();
    if (child != 0) {
        int index;
        u16 x;
        u16 y;
        s16 px;
        s16 py;
        child->field_00 = 1;
        /* The original stores 0x3b to field_02 and then 0x14; without the first store the build differs in 3 instruction lines. */
        child->field_02 = 0x3b;
        child->field_03 = 1;
        child->field_09 = 0;
        child->field_02 = 0x14;
        child->field_65 = obj->field_65;
        child->field_44 = 1;
        index = get_random_byte() & 0x1e;
        x = data_8007ee64_slot2b[index];
        y = data_8007ee64_slot2b[index + 1];
        px = obj->pos_x;
        py = obj->pos_y;
        child->field_48 = 0x52;
        child->field_7a = 0x60;
        child->field_3c = parent;
        child->field_7c = 0x1e0;
        x += px;
        /* Written as `py -= y;` with `child->pos_y = py;` the build differs in 8 instruction lines. */
        y = py - y;
        child->pos_x = x;
        child->pos_y = y;
        child->field_0d = obj->field_0d;
        child->field_08 = 0x20;
        child->field_90 = obj->field_90;
        child->field_66 = obj->field_66;
        child->field_98 = obj->field_98;
        child->field_9c = obj->field_9c;
        play_side_sound_at(obj, parent->side, 0xf);
    }
}
