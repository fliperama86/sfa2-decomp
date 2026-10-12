/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void play_frame_sound(Object *object) {
    s16 index;
    s16 *table;
    index = object->frame_sound_index;
    if (index == 0) return;
    object->frame_sound_index = 0;
    if (data_801a8067 == 0xd && data_801a83fb == data_801a8067) {
        table = data_80171bf0;
    } else if (data_801a8067 == 0x10 && data_801a83fb == data_801a8067) {
        table = data_80171bf4;
    } else if (data_801a8067 == 0x11 &&
               (data_801a83fb == data_801a8067 || data_801a83fb == 0x13)) {
        table = data_80171bf8;
    } else if (data_801a8067 == 0x13 &&
               (data_801a83fb == data_801a8067 || data_801a83fb == 0x11)) {
        table = data_80171bf8;
    } else if (object->side == 0) {
        table = data_80171b9c[object->kind];
    } else {
        table = data_80171bfc[object->kind];
    }
    index = table[index];
    if (index & 0xff00) {
        play_sound_at(object, object->side, index);
    } else {
        play_side_sound_at(object, object->side, index);
    }
}
