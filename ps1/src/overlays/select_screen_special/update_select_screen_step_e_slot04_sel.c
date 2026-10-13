/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* Inferred declarations, not original ones. */
extern HudState *screen_state;
extern TextBuf data_801b8638_slot04_sel;
extern TextBuf data_801b874c_slot04_sel;
extern Slot04SelRec9d38 data_801b9d38_slot04_sel[];
extern u8 data_801b9d68_slot04_sel;
extern u8 data_801b9d6c_slot04_sel;
extern void (*data_801b7df0_slot04_sel[])(Object *, Slot04SelRec9d38 *);

/* What it does (inferred, not an original name): the per-frame update of
   a two-player selection screen. It calls add_item_to_table for two text
   buffers, then runs for each player a handler picked from a table by the
   first byte of the player's selection record (records of 0x15 bytes at
   data_801b9d38_slot04_sel, one per player, the handler gets the player
   object and the record). Then, when game_state.side_mask is not 0 and the
   active_flag of either record is not 0, it clears both records' first byte,
   adds one to the hud state's substep_index and clears its step_index_2. Finally,
   when the byte data_801b9d68_slot04_sel differs from game_state.side_mask
   or data_801b9d6c_slot04_sel from game_state.mode, it clears both records'
   first byte and the hud state's step_index_2. */
void update_select_screen_step_e_slot04_sel(void) {
    Slot04SelRec9d38 *r;
    Slot04SelRec9d38 *q;
    Object *w;

    add_item_to_table((Object *)&data_801b8638_slot04_sel);
    add_item_to_table((Object *)&data_801b874c_slot04_sel);
    w = &player_left;
    r = data_801b9d38_slot04_sel;
    /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    /* Without it this function differs from the original in 3 instruction slots. */
    do {
        data_801b7df0_slot04_sel[r->count](w, r);
    } while (0);
    w += 1;
    q = r + 1;
    data_801b7df0_slot04_sel[q->count](w, q);
    /* Stand-in, not recovered source: the term that is ORed to game_state.side_mask in the test below, which is 0 for every value of w, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
    /* The original steps the player pointer in its own register and then copies it to the first argument register. With no later use of the pointer the compiler makes one instruction of the two; the term is such a use and leaves no instruction of its own. */
    /* The pointer goes to an unsigned integer of 64 bits first and from there to 32 bits: the first conversion holds every pointer of the 32-bit and 64-bit compilers this project builds with, and the second is the defined narrowing of an unsigned integer. A direct conversion of a wider pointer to 32 bits would not be defined. */
    if ((game_state.side_mask | (((u32)(unsigned long long)w << 24) & 0x100)) != 0) {
        if (r->active_flag != 0 || r[1].active_flag != 0) {
            r->count = 0;
            q->count = 0;
            screen_state->substep_index = screen_state->substep_index + 1;
            screen_state->step_index_2 = 0;
        }
    }
    if (data_801b9d68_slot04_sel != game_state.side_mask || data_801b9d6c_slot04_sel != game_state.mode) {
        r->count = 0;
        q->count = 0;
        screen_state->step_index_2 = 0;
    }
}
