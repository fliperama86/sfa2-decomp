/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80130b10(Object *object) {
    int limit;
    int dist;
    s16 x;
    int d;
    int span;

    object->field_164 = 0;
    ref_other.p = object->other;
    limit = 0x100;
    if ((s16)object->field_5c >= 0 && (s16)ref_other.p->field_5c >= 0) limit = 0x150;
    x = object->pos_x;
    dist = ref_other.p->pos_x - x;
    if (dist < 0) dist = -dist;
    span = (s16)object->field_154;
    d = x - span - (s16)box_margin[0];
    if (d <= 0) {
        object->pos_x = x - d;
        object->field_164 = 1;
        if (*(u16 *)&object->field_04 == 0x101 && object->field_241 == 0 && object->field_45 == 0 && dist < limit) goto push;
    } else {
        d += span * 2;
        d -= 0x180;
        if (d >= 0) {
            object->pos_x = x - d;
            object->field_164 = 2;
            if (*(u16 *)&object->field_04 == 0x101 && object->field_241 == 0 && object->field_45 == 0 && dist < limit) {
push:
                if (object->field_166 == 0 && ref_other.p->field_45 == 0) {
                    ref_other.p->pos_x = ref_other.p->pos_x - d;
                }
            }
        }
    }
}

void func_80130c80(Object *object) {
    s16 d;
    int span;
    u16 x;

    ref_other.p = object->other;
    ref_first.p = object->other;
    ref_second.p = object;
    ref_other.p->field_164 = 0;
    if (!(ref_other.p->pos_x < object->pos_x)) {
        ref_first.p = object;
        ref_second.p = ref_other.p;
    }
    span = ref_first.p->field_154;
    x = ref_first.p->pos_x;
    d = x - span - box_margin[0];
    if (d < 0) {
        object->pos_x = object->pos_x - d;
        ref_other.p->pos_x = ref_other.p->pos_x - d;
        ref_other.p->field_164 = 1;
    } else {
        x = ref_second.p->pos_x;
        span = ref_second.p->field_154;
        d = x - box_margin[0] + span - 0x180;
        if (d >= 0) {
            object->pos_x = object->pos_x - d;
            ref_other.p->pos_x = ref_other.p->pos_x - d;
            ref_other.p->field_164 = 2;
        }
    }
}

void func_80130dc0(Object *object) {
    int half;
    s16 index;
    int bonus;

    func_80130ec0(object);
    half = object->field_12a >> 1;
    if (object->kind != 9) {
        func_80141f28(object, half);
    } else {
        object->field_225 = 1;
        if (object->field_246 == 0) {
            object->field_225 = 0;
            func_80141f28(object, half);
        }
    }
    index = 0;
    if (object->field_128 != 0) {
        index = 6;
        object->field_157 = object->field_157 + 1;
    }
    bonus = 0;
    if (object->field_129 != 0) bonus = 3;
    index = index + half;
    index = index + bonus;
    func_801307e0(object, (u16)index);
    object->field_248 = kind_table_248[object->kind * 16 + index];
    object->field_29b = kind_table_29b[object->kind * 16 + index];
}

void func_80130ec0(Object *object) {
    object->field_278 = 0;
    object->field_279 = 0;
    object->field_157 = 0;
    object->field_67 = 0;
    object->field_16a = 0;
    object->field_17d = 0xff;
    object->field_17c = 0xff;
    func_80142c04(object);
}

void func_80130efc(Object *object) {
    func_80130f2c(object);
    func_80131020(object);
}

void func_80130f2c(Object *object) {
    SequenceStep *next;

    if (--*(s16 *)&object->field_38 == 0) {
        if ((s16)object->field_3a < 0) {
            next = object->sequence + object->sequence->loop_offset;
            if (next != object->sequence) object->field_80 = 1;
            object->sequence = object->sequence + object->sequence->loop_offset;
        } else {
            next = object->sequence + 1;
            if (next != object->sequence) object->field_80 = 1;
            object->sequence = object->sequence + 1;
        }
        object->field_38 = object->sequence->duration;
        object->field_3a = object->sequence->flags;
        object->frame = object->frames + object->sequence->frame_index;
        object->field_4a = object->frame->field_0d;
    }
}

void func_80131020(Object *object) {
    u32 w;

    if (object->field_7e != 0) {
        w = *(u32 *)&object->field_04;
        if ((u16)w == 1) {
            if ((w & 0xffff0000) == 0x03030000 || object->field_06 == 5) {
                if (object->field_29a == 0) func_80130f2c(object);
            }
        }
    }
}

void func_80131094(Object *object) {
    SequenceStep *next;

    if (--*(s16 *)&object->field_38 == 0) {
        if ((s16)object->field_3a < 0) {
            next = object->sequence + object->sequence->loop_offset;
            if (next != object->sequence) object->field_80 = 1;
            object->sequence = object->sequence + object->sequence->loop_offset;
        } else {
            next = object->sequence + 1;
            if (next != object->sequence) object->field_80 = 1;
            object->sequence = object->sequence + 1;
        }
        object->field_38 = object->sequence->duration;
        object->field_3a = object->sequence->flags;
        if (object->field_08 == 0 || object->field_08 == 8) {
            object->frame = object->frames + object->sequence->frame_index;
            object->field_4a = object->frame->field_0d;
        } else {
            object->field_4a = 0;
        }
    }
}
