/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code keeps the table base and the object's field_d4 in swapped
 * registers. The exact owner of the bytes in the PS1 build stays the raw
 * bytes of the module image; the build does not use this file. The
 * differential test next to it (difftest.py, with func_801b2270_slot04_0f.py)
 * compares the behavior of this C with the original code on random inputs of
 * the contract below.
 *
 * What it does (inferred, not an original name): a character object holds a
 * wanted state byte in field_3a and the state it last acted on in field_339.
 * When they differ it copies field_3a to field_339. If bit 7 of the new value
 * is set it calls func_801b22ec (one argument, the object). Otherwise it
 * calls func_801b2340 with the object and a row of 16 halfwords of a table,
 * data_801c5500_slot04_0f, chosen by field_339 plus 5 times the object's
 * field_d4. When the two bytes are equal it does nothing.
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's field_3a, field_339 and field_d4. The table is not
 *     read here, only its row address is passed on.
 *   Writes: the object's field_339.
 *   Callees (both replaced by recorders that return 0, because they reach
 *     the library and the job queue):
 *     func_801b22ec (object): 1 argument.
 *     func_801b2340 (object, row address): 2 arguments.
 *   Watched at every recorded call: the whole object (0x394 bytes). No
 *     pointee is recorded: the row address passed to func_801b2340 is a
 *     table address the function does not fill.
 *   Aliasing: none that matters; the object is one block.
 *   Excluded inputs: none. Slots no input can reach: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c5500_slot04_0f[][0x10];

void func_801b22ec_slot04_0f(Object *obj);
void func_801b2340_slot04_0f(Object *obj, u16 *src);

void func_801b2270_slot04_0f(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a != obj->field_339) {
        obj->field_339 = obj->field_3a;
        if (obj->field_339 & 0x80) {
            func_801b22ec_slot04_0f(o);
        } else {
            func_801b2340_slot04_0f(o, data_801c5500_slot04_0f[obj->field_339 + o->field_d4 * 5]);
        }
    }
}
