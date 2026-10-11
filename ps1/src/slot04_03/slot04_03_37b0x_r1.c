/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* What it does (inferred, not an original name): a state of a character that is struck or held by the other object. It stores the other object (field_40) in ref_other. When the low byte of field_3a is not 0 it clears that byte, makes a sound, calls func_80146478 and func_80140cd8 (with 8 the first time field_1cf is 0, which then becomes 0xff, otherwise 0). If func_80140cd8 returns a nonzero byte it calls func_80140770, sounds 0x340 for the other object and goes to the advance. Otherwise it adds 1 to field_46 and sounds 0x30b. Then (and when the low byte of field_3a was 0) it calls func_80140fe0; if field_46 is 0 it hands over to func_80130efc; otherwise it reloads ref_other from field_40 and advances when the other object's field_15b is 0 or func_801410c8 returns a nonzero byte, else hands over to func_80130efc. The advance adds 1 to field_07, sounds 0x30d for the other object and starts sequence 0x41. */
void func_801b37b0_slot04_03(Object *obj) {
    /* Stand-in, not recovered source: the local r, which holds the address of ref_other and is set again in three places (the original keeps it in a register of its own), reproduces the original's choice of registers. Without it this function differs from the original in 53 instruction slots. */
    ObjectRef *r;
    /* k is a short: as an int this function differs from the original in 6 instruction slots (the original widens the argument of func_80140cd8 with sll/srl 16). */
    s16 k;
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        r = &ref_other;
        r->p = obj->other;
    } while (0);
    if ((obj->step_flags & 0xff) != 0) {
        obj->step_flags &= 0xff00;
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
            /* Stand-in, not recovered source: a second do-while wrapper, around the reload of r and the call that sounds 0x340, reproduces the original's choice of registers. Without it this function differs from the original in 43 instruction slots. */
            do {
                r = &ref_other;
                func_80120554(r->p, ref_other.p->side, 0x340);
            } while (0);
            goto tail;
        }
        /* Stand-in, not recovered source: a third do-while wrapper, around the reload of r, the increment of field_46 and the call that sounds 0x30b, reproduces the original's choice of registers. Without it this function differs from the original in 43 instruction slots. */
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
    obj->state_3++;
    /* Stand-in, not recovered source: a fourth do-while wrapper, around the single statement that sounds 0x30d, reproduces the original's choice of registers. Without it this function differs from the original in 43 instruction slots. */
    do {
        func_80120554(r->p, ref_other.p->side, 0x30d);
    } while (0);
    func_801204f4(obj, obj->side, 7);
    func_801307e0(obj, 0x41);
    return;
idle:
    func_80130efc(obj);
}


