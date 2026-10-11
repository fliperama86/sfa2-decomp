/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations, not original ones. */
extern HudState *data_8018f5a0;
extern TextBuf data_801b8638_slot04_sel;
extern TextBuf data_801b874c_slot04_sel;
extern Slot04SelRec9d38 data_801b9d38_slot04_sel[];
extern u8 data_801b9d68_slot04_sel;
extern u8 data_801b9d6c_slot04_sel;
extern void (*data_801b7df0_slot04_sel[])(Object *, Slot04SelRec9d38 *);

/* What it does (inferred, not an original name): the per-frame update of
   a two-player selection screen. It calls func_801519b4 for two text
   buffers, then runs for each player a handler picked from a table by the
   first byte of the player's selection record (records of 0x15 bytes at
   data_801b9d38_slot04_sel, one per player, the handler gets the player
   object and the record). Then, when game_state.field_07 is not 0 and the
   field_0d of either record is not 0, it clears both records' first byte,
   adds one to the hud state's field_4e and clears its field_50. Finally,
   when the byte data_801b9d68_slot04_sel differs from game_state.field_07
   or data_801b9d6c_slot04_sel from game_state.mode, it clears both records'
   first byte and the hud state's field_50. */
void func_801b3f38_slot04_sel(void) {
    Slot04SelRec9d38 *r;
    Slot04SelRec9d38 *q;
    Object *w;

    func_801519b4((Object *)&data_801b8638_slot04_sel);
    func_801519b4((Object *)&data_801b874c_slot04_sel);
    w = &player_left;
    r = data_801b9d38_slot04_sel;
    /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 3 instruction slots. */
    do {
        data_801b7df0_slot04_sel[r->field_00](w, r);
    } while (0);
    w += 1;
    q = r + 1;
    data_801b7df0_slot04_sel[q->field_00](w, q);
    /* Stand-in, not recovered source: the term that is ORed to game_state.field_07 in the test below, which is 0 for every value of w, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
    /* The original steps the player pointer in its own register and then copies it to the first argument register. With no later use of the pointer the compiler makes one instruction of the two; the term is such a use and leaves no instruction of its own. */
    /* The pointer goes to the widest unsigned integer first and from there to 32 bits: the first conversion holds every pointer of a compiler with wider pointers, and the second is the defined narrowing of an unsigned integer. A direct conversion of a wider pointer to 32 bits would not be defined. */
    if ((game_state.field_07 | (((u32)(unsigned long long)w << 24) & 0x100)) != 0) {
        if (r->field_0d != 0 || r[1].field_0d != 0) {
            r->field_00 = 0;
            q->field_00 = 0;
            data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
            data_8018f5a0->field_50 = 0;
        }
    }
    if (data_801b9d68_slot04_sel != game_state.field_07 || data_801b9d6c_slot04_sel != game_state.mode) {
        r->field_00 = 0;
        q->field_00 = 0;
        data_8018f5a0->field_50 = 0;
    }
}
