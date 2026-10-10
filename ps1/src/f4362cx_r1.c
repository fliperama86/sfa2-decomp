/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* The difference is copied into the local that held the negated extent, and
   the sign test reads that copy. With the test reading d itself, this
   function differs from the original in 7 instruction slots; with one local
   for the difference, the test and both values of r, in 6; with r an int,
   in 3. In the mirrored case the extent is negated a second time, as the
   original does. */
void func_8014362c(Object *object, Object *other) {
    Box6 *box = other->unknown_148;
    int neg;
    int ext;
    int org;
    int flip;
    int d;
    u16 r;
    box++;
    ext = box->extent;
    org = (u16)box->origin;
    neg = -ext;
    flip = object->field_158 ^ 1;
    ext = neg;
    if (flip) {
        org = -org;
        ext = -neg;
    }
    d = (u16)object->pos_x - (org + ext + (u16)other->pos_x);
    r = d;
    neg = d;
    if ((s16)neg < 0) {
        r = -d;
    }
    object->field_21e = r;
}
