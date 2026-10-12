/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern Object *data_80188f20;
extern s16 table_8017736c[];
extern s16 table_80177390[];
extern s16 table_80197ef8[];

/*
 * What it does (inferred, not an original name): `object` is the object
 * that triggered a hit, `other` the object that was hit, `g` the Box32 record that the callers pass (inferred; the role is not known).
 * If the pool of spawnable objects (data_801a27d4, a byte counter) is not 0
 * and the pool hands out an object, it fills that object in as a hit effect
 * (kind 4, copies of fields of object and other, a link back to other,
 * a rectangle size, a draw hook, and a position at the centre of the
 * rectangle data_80188ed0) and counts the pool down. If other's field_61
 * has bit 0x80 the effect's field_48 is 1. Otherwise, when g->field_17 has
 * bit 0x80 and the pool is still not empty, a second effect is made the
 * same way, without the position and without masking field_03. Afterwards,
 * if other's field_61 has bit 0x80 the function starts an animation on
 * other (play_sound_at, or play_side_sound_at when other's kind is 6) from the
 * tables at 0x80177390; else, unless g->field_10 has bit 0x80, it starts
 * one chosen by an index from g->field_10 (doubled, with bit 0 set when
 * other's health is negative): for an index below 16 one animation
 * (play_sound_at with other's side and table_8017736c[index]), otherwise
 * two (play_sound_at with other's side and 0x32e, then play_side_sound_at with
 * the other side value, side ^ 1, and table_8017736c[index]); then a
 * follow-up step from a table by other's field_66 (func_80120444), left
 * out when the table entry is 3.
 */
void func_8013b0c4(Object *object, Object *other, Box32 *g)
{
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    /* Without it this function differs from the original in 22 instruction slots. */
    u8 unused[8];
    s16 idx;
    u8 k;
    s16 v;
    int x0;

    data_80188f20 = ref_other.p;
    if ((data_801a27d4 != 0) && ((ref_other.p = (Object *)func_8011f1e0()) != 0)) {
        ref_other.p->field_00++;
        ref_other.p->field_02 = 4;
        ref_other.p->field_65 = object->field_65;
        ref_other.p->field_0b = other->facing_flag;
        ref_other.p->field_0e = other->field_0e;
        ref_other.p->field_3c = other;
        ref_other.p->field_03 = g->field_17 & 0x7f;
        other->field_17f = g->field_17 & 0x7f;
        ref_other.p->field_76 = 0x240;
        ref_other.p->field_78 = 0;
        ref_other.p->spawn_x = 0x60;
        ref_other.p->spawn_y = 0x1e0;
        ref_other.p->field_90 = (void *)0x800fb100;
        data_801a27d4--;
        /* out_40 of the rectangle data_80188ed0 (offset 0x40, inside that record) is read through a cast; the plain form data_80188ed0.out_40 differs from the original in 38 instruction slots. */
        x0 = *(s16 *)((char *)&data_80188ed0 + 0x40);
        /* out_3c of the rectangle data_80188ed0 (offset 0x3c, inside that record) is read through a cast; the plain form data_80188ed0.out_3c differs from the original in 36 instruction slots. */
        /* The pos_x sum is kept in idx, the variable that holds the pos_y sum below; as a plain expression without idx it differs from the original in 36 instruction slots. */
        ref_other.p->pos_x = (idx = (((*(s16 *)((char *)&data_80188ed0 + 0x3c)) - x0) >> 1) + x0);
        /* The pos_y sum is kept in idx as well; as a plain expression without idx it differs from the original in 2 instruction slots. */
        ref_other.p->pos_y = (idx = ((((s16)data_80188ed0.out_44) - ((s16)data_80188ed0.out_48)) >> 1) + ((s16)data_80188ed0.out_48));
        ref_other.p->field_48 = 0;
        if (other->field_61 & 0x80) {
            ref_other.p->field_48 = 1;
        } else if (((g->field_17 & 0x80) && (data_801a27d4 != 0)) && ((ref_other.p = (Object *)func_8011f1e0()) != 0)) {
            ref_other.p->field_00++;
            ref_other.p->field_02 = 4;
            ref_other.p->field_03 = g->field_17;
            ref_other.p->field_65 = object->field_65;
            ref_other.p->field_0b = other->facing_flag;
            ref_other.p->field_0e = other->field_0e;
            ref_other.p->field_3c = other;
            ref_other.p->field_48 = 0;
            ref_other.p->field_76 = 0x240;
            ref_other.p->field_78 = 0;
            ref_other.p->spawn_x = 0x60;
            ref_other.p->spawn_y = 0x1e0;
            data_801a27d4--;
            ref_other.p->field_90 = (void *)0x800fb100;
        }
    }
    ref_other.p = data_80188f20;
    if (other->field_61 & 0x80) {
        if (other->kind != 6) {
            play_sound_at(other, other->field_66, table_80177390[other->kind]);
        } else {
            play_side_sound_at(other, other->field_66, table_80177390[6]);
        }
    } else if (!(g->field_10 & 0x80)) {
        idx = g->field_10 * 2;
        if (((s16)other->health) < 0) {
            idx |= 1;
        }
        if (idx >= 16) {
            play_sound_at(other, other->side, 0x32e);
            play_side_sound_at(other, other->side ^ 1, table_8017736c[idx]);
        } else {
            play_sound_at(other, other->side, table_8017736c[idx]);
        }
        k = other->field_66;
        v = table_80197ef8[k];
        if (v != 3) {
            /* The second argument is the halfword v, passed whole: the original does not mask it here; the callee takes its low byte. */
            func_80120444(k, v);
        }
    }
}
