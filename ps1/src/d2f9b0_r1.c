/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

u8 func_8012faf8(Object *object);

/* What it does (inferred, not original names): looks through the 16 units of the table units_2c20 for one that is near the given object and belongs to the other side. A unit qualifies when its field_00 is not 0, bit 0x80 of its field_74 is clear, its field_65 differs from the object's, and the 16-bit horizontal distance between its field_12 and the object's pos_x is less than 0x50. The cursor ref_other.p walks the table, 0xc0 bytes per step. For a qualifying unit: if the object's field_150 has bit 0x2000 set, field_24e and field_251 of the object are set to 1 when its field_d8 is not 0; if the bit is clear, the unit is passed over when field_d8 is 0 and field_24e and field_251 are set to 1 otherwise. Then data_80186000 is set to 1 and the function returns. After 16 units without a hit data_80186000 is set to the result of func_8012faf8(object). */
void func_8012f9b0(Object *object)
{
    s16 i;
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. With 15 written at its use this function differs from the original in 5 instruction slots. */
    s16 limit = 15;
    /* Stand-in, not recovered source: this local, which holds the address of the cursor ref_other.p, reproduces the original's choice of registers. Without it this function differs from the original in 5 instruction slots. */
    /* The original keeps that address in a register of its own and reads and writes the cursor through it at the end of the loop. The 5 slots are with ref_other.p written in the local's place and the wrappers kept; without the local and without the wrappers: 33. */
    Object **pp;
    Unit *unit;
    int diff;
    ref_other.p = (Object *) units_2c20;
    i = 0;
    pp = &ref_other.p;
    while (1)
    {
        unit = (Unit *) ref_other.p;
        if (unit->field_00 != 0 && (unit->field_74 & 0x80) == 0 && unit->field_65 != object->field_65)
        {
            diff = unit->field_12 - (u16) object->pos_x;
            /* The distance is made positive with a conditional expression; as an if statement this function differs from the original in 4 instruction slots. */
            diff = ((s16) diff < 0) ? -diff : diff;
            if ((s16) (diff + 0x50) < 0xa0)
            {
                if (object->field_150 & 0x2000)
                {
                    if (object->field_d8 != 0)
                    {
                        object->field_24e = 1;
                        object->field_251 = 1;
                    }
                }
                else
                {
                    if (object->field_d8 == 0)
                        goto next;
                    object->field_24e = 1;
                    object->field_251 = 1;
                }
                data_80186000 = 1;
                return;
            }
        }
next:
        /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        do {
            /* Stand-in, not recovered source: a nested pair of further do-while wrappers inside the first (three levels in all), around the one statement, reproduces the original's choice of registers. Without it this function differs from the original in 11 instruction slots. */
            /* The pair raises the compiler's count of references to pp. With one of the two wrappers: 8 slots. */
            do {
                do {
                    *pp = (Object *) ((u8 *) *pp + 0xc0);
                } while (0);
            } while (0);
        } while (0);
        i++;
        if (i > limit)
            break;
    }
    data_80186000 = func_8012faf8(object);
}
