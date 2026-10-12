#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void close_sounds_and_load_command_4(int a)
{
  GameState *g = &game_state;
  g->loop_mode = 1;
  set_voice_thread_entry_and_open(2, 7);
  request_sound_command_and_switch_thread(1);
  close_vab_and_sep(0);
  close_vab_and_sep(1);
  close_vab_and_sep(2);
  if (a == 0)
  {
    start_cd_track_wait_with_thread_switch(4, 2);
  }
  else
  {
    start_cd_track_wait_with_thread_switch(4, 1);
  }
  g->loop_mode = 0;
  while (g->wait_flag)
  {
    request_sound_command_and_switch_thread(1);
  }
}
