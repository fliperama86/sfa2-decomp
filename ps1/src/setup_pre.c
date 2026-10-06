/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_8013060c(Object *object) {
    func_8013f3e8(object);
    object->field_262 = 0x10;
    if (object->field_267 == 0) {
        object->field_25c = 0;
        if (object->side == 0) scratch_fn_left(object);
        else scratch_fn_right(object);
    }
}

void func_80130678(Object *object, u16 index) {
    if (object->side == 0) object->sequence = seqs_60_left[index];
    else object->sequence = seqs_110_right[index];
    object->field_38 = object->sequence->duration;
    object->field_3a = object->sequence->flags;
    object->field_80 = 1;
    object->frame = object->frames + object->sequence->frame_index;
    object->field_4a = object->frame->field_0d;
}

void func_80130700(Object *object, SequenceStep *entry) {
    object->sequence = entry;
    object->field_38 = entry->duration;
    object->field_3a = object->sequence->flags;
    object->field_80 = 1;
    if (object->field_08 == 0 || object->field_08 == 8) {
        object->frame = object->frames + object->sequence->frame_index;
        object->field_4a = object->frame->field_0d;
    } else {
        object->field_4a = 0;
    }
}

void func_80130768(Object *object, s16 index, SequenceStep **table) {
    object->sequence = table[index];
    object->field_38 = object->sequence->duration;
    object->field_3a = object->sequence->flags;
    object->field_80 = 1;
    if (object->field_08 == 0 || object->field_08 == 8) {
        object->frame = object->frames + object->sequence->frame_index;
        object->field_4a = object->frame->field_0d;
    } else {
        object->field_4a = 0;
    }
}

void func_801307e0(Object *object, int arg) {
    u16 index = arg;
    if (object->field_cd == 0) {
        if (object->side == 0) object->sequence = sequences_left[index];
        else object->sequence = sequences_right[index];
    } else {
        if (object->side == 0) object->sequence = seqs_9c_left[index];
        else object->sequence = seqs_14c_right[index];
    }
    object->field_38 = object->sequence->duration;
    object->field_3a = object->sequence->flags;
    object->field_80 = 1;
    object->frame = object->frames + object->sequence->frame_index;
    object->field_4a = object->frame->field_0d;
    func_80131020(object);
}

void func_801308c4(Object *object, u16 index) {
    SequenceStep *entry;
    if (object->field_295 != 0) {
        if (object->side == 0) entry = seqs_6c_left[index];
        else entry = seqs_11c_right[index];
    } else {
        if (object->side == 0) entry = seqs_68_left[index];
        else entry = seqs_118_right[index];
    }
    if (object->sequence != entry) object->field_80 = 1;
    object->sequence = entry;
    object->field_38 = entry->duration;
    object->field_3a = object->sequence->flags;
    object->frame = object->frames + object->sequence->frame_index;
    object->field_4a = object->frame->field_0d;
}
