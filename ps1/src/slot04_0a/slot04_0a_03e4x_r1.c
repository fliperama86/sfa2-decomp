/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/*
 * What it does (inferred, not an original name): a state of a character
 * object that picks a random variant. When game_state.field_64 and
 * field_5c are both 0 it sets the object's field_46 to 0x3c00, adds one to
 * its state_2, sets game_state.field_76 to 0x1e, draws a number from the
 * random generator, divides it by 52, passes it to func_80125734 together
 * with the object, stores the result as a signed byte in field_48 and
 * starts the sequence numbered 0x23 plus that result with func_80130678.
 * Otherwise it only calls func_80130efc.
 */
void func_801b03e4_slot04_0a(Object *obj) {
    unsigned variant;

    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->field_46 = 0x3c00;
        obj->state_2 = obj->state_2 + 1;
        game_state.field_76 = 0x1e;
        /* One local holds the random number and then the callee's result: with a local for each, 14 instruction lines differ. */
        variant = func_80151184();
        variant = (s8)func_80125734(obj, (s8)(variant / 52));
        obj->field_48 = variant;
        func_80130678(obj, (s16)(variant + 0x23));
    } else {
        func_80130efc(obj);
    }
}
