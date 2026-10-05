/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by a scripted enumeration of local types and statement order (3,600 variants), then finished by hand. */
/* The header word is loaded into one local and copied into a second; the
   first local is then reused for the high half. Written that way the
   compiler keeps both registers, as the original does: the copy is stored as
   the low half and the loaded register is shifted in place. */
void func_8011f994(Object *object, SeqRec *rec, u8 *base) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int offset, p, q;
    int t;
    int v;
    object->sequence = (SequenceStep *)rec;
    t = rec->header;
    v = t;
    t = v >> 16;
    object->field_38 = t;
    object->field_3a = v;
    q = (object->pos_x & 0xe00) * 4;
    p = (object->pos_x & 0x1f0) / 16 * 4;
    offset = p + q + (((object->pos_y / 16) << 23) >> 16);
    func_8011fae0(rec->data, (u16 *)(base + ((offset << 15) >> 15)));
}
