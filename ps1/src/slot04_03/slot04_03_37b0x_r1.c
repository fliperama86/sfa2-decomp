/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* What it does (inferred, not an original name): a state of a character that is struck or held by the other object. It stores the other object (field_40) in ref_other. When the low byte of field_3a is not 0 it clears that byte, makes a sound, calls func_80146478 and func_80140cd8 (with 8 the first time field_1cf is 0, which then becomes 0xff, otherwise 0). If func_80140cd8 returns a nonzero byte it calls func_80140770, sounds 0x340 for the other object and goes to the advance. Otherwise it adds 1 to field_46 and sounds 0x30b. Then (and when the low byte of field_3a was 0) it calls func_80140fe0; if field_46 is 0 it hands over to func_80130efc; otherwise it reloads ref_other from field_40 and advances when the other object's field_15b is 0 or func_801410c8 returns a nonzero byte, else hands over to func_80130efc. The advance adds 1 to field_07, sounds 0x30d for the other object and starts sequence 0x41. */
void func_801b37b0_slot04_03(Object *obj) {
    /* The local r holds the address of ref_other, which the original keeps in a register of its own and sets again in three places (below). */
    ObjectRef *r;
    /* k is a short: as an int or an unsigned short this function differs from the original in 13 instruction lines (the original widens the argument of func_80140cd8 with sll/srl 16). */
    s16 k;
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        r = &ref_other;
        r->p = obj->other;
    } while (0);
    if ((obj->field_3a & 0xff) != 0) {
        obj->field_3a &= 0xff00;
        func_801204f4(obj, obj->side, 7);
        func_80146478(obj, 1, -0x2d, 0x2b);
        if (((Slot04aObj *)obj)->field_1cf == 0) {
            ((Slot04aObj *)obj)->field_1cf = 0xff;
            k = 8;
        } else {
            k = 0;
        }
        if ((func_80140cd8(obj, k, 0) & 0xff) != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 1);
            /* Exception, not recovered source: a further do-while wrapper, around the reload of r and the call that sounds 0x340. It adds no instruction; the compiler counts a reference inside it twice, which the original's register choice for the pointer r and the object needs. Without it this function differs from the original in 43 instruction lines (each of the four wrappers is needed). */
            do {
                r = &ref_other;
                func_80120554(r->p, ref_other.p->side, 0x340);
            } while (0);
            goto tail;
        }
        /* Exception, not recovered source: a further do-while wrapper, around the reload of r, the increment of field_46 and the call that sounds 0x30b. It adds no instruction; the compiler counts a reference inside it twice, which the original's register choice for the pointer r and the object needs. Without it this function differs from the original in 43 instruction lines (each of the four wrappers is needed). */
        do {
            r = &ref_other;
            obj->field_46++;
            func_80120554(r->p, ref_other.p->side, 0x30b);
        } while (0);
        func_80140fe0(obj);
        if ((s16)obj->field_46 == 0) {
            goto idle;
        }
        r->p = obj->other;
        if (ref_other.p->field_15b == 0) {
            goto tail;
        }
        if ((func_801410c8(obj) & 0xff) != 0) {
            goto tail;
        }
        goto idle;
    } else {
        func_80140fe0(obj);
        if ((s16)obj->field_46 == 0) {
            goto idle;
        }
        r->p = obj->other;
        if (ref_other.p->field_15b == 0) {
            goto tail;
        }
        if ((func_801410c8(obj) & 0xff) == 0) {
            goto idle;
        }
    }
tail:
    obj->field_07++;
    /* Exception, not recovered source: a further do-while wrapper, around the single statement that sounds 0x30d. It adds no instruction; the compiler counts a reference inside it twice, which the original's register choice for the pointer r and the object needs. Without it this function differs from the original in 43 instruction lines (each of the four wrappers is needed). */
    do {
        func_80120554(r->p, ref_other.p->side, 0x30d);
    } while (0);
    func_801204f4(obj, obj->side, 7);
    func_801307e0(obj, 0x41);
    return;
idle:
    func_80130efc(obj);
}


