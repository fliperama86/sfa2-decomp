/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code has the original's size but differs in which register holds
 * the loaded halfword and which the shifted copy. The exact owner of the
 * bytes in the PS1 build stays the raw bytes of the resident executable;
 * the build does not use this file. The differential test next to it
 * (func_8014a94c.py, run by difftest.py) compares the behavior of this C
 * with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): reads the next 16-bit word
 * of the object's script (the pointer data_80189460, which it advances) and
 * keeps it in field_210 (high byte) and field_211 (low byte). If the object's
 * partner is active (other->field_240) and has a linked object
 * (other->field_14c) whose field_04 is 1, it measures the horizontal
 * distance to the partner with func_8014c4a8 (result in data_80189464);
 * when the script word is smaller than that distance, it sets the object
 * to a new state (field_209 = 7, field_208 = 0, field_04 = 1, fields 05
 * to 07 cleared) and returns. In every other case it calls func_8014c914.
 *
 * Contract:
 *   Argument: a0 = object. No return value.
 *   Reads: data_80189460 and the word it points at; object->other and from
 *     it field_240, field_14c; the linked object's field_04; then what
 *     func_8014c4a8 reads (see its file); data_80189464.
 *   Writes: data_80189460 (+2), object->field_210 and field_211; in the
 *     state-change arm field_209, field_208, field_04 to field_07; the
 *     globals ref_other and ref_second (both set to the linked object, only
 *     when field_240 and field_14c are not 0, ref_second also only when
 *     the linked object's field_04 is 1); and the callees' writes.
 *   func_8014c4a8 runs as the original code. func_8014c914 is replaced by a
 *     recorder that takes the object (its own code reaches the script
 *     interpreter and is outside this test); it returns nothing the
 *     function uses.
 *   Watched at every recorded call: the whole object, data_80189460 and
 *     data_80189464, ref_other, ref_second.
 *   Aliasing: the object, its partner, the linked object, the frame record,
 *     the box table and the script words are distinct blocks.
 *   Exclusions: none. All instruction slots are reachable.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8014a94c(Object *object) {
    Object *other;
    u16 word;

    word = *data_80189460++;
    object->field_210 = word >> 8;
    object->field_211 = word;
    other = object->other;
    if (other->field_240 != 0 && other->field_14c != 0) {
        ref_other.p = (Object *)other->field_14c;
        if (ref_other.p->field_04 == 1) {
            ref_second.p = (Object *)other->field_14c;
            func_8014c4a8(object);
            if (word < data_80189464) {
                object->field_209 = 7;
                object->field_208 = 0;
                object->field_04 = 1;
                object->field_05 = 0;
                object->field_06 = 0;
                object->field_07 = 0;
                return;
            }
        }
    }
    func_8014c914(object);
}
