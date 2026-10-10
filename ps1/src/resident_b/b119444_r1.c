/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Counted up here once a frame and read by the frame loop of the game's main function (inferred). */
extern volatile u16 data_801ac310;
void func_80119718(void);
void func_80150638(void);

/* The handler that the vertical-blank event calls once a frame (inferred). */
void func_80119444(void) {
    func_80119718();
    data_801ac310++;
    func_80150638();
}
