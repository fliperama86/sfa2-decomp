/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Counted up here once a frame and read by the frame loop of the game's main function (inferred). */
extern volatile u16 data_801ac310;
void update_pad_state(void);
void check_cd_position_and_set_command_state(void);

/* The handler that the vertical-blank event calls once a frame (inferred). */
void update_pad_and_count_frame(void) {
    update_pad_state();
    data_801ac310++;
    check_cd_position_and_set_command_state();
}
