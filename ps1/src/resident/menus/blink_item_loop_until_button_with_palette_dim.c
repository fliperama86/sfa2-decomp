/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void blink_item_loop_until_button_with_palette_dim(void)
{
  int n;
  int v;
  int new_var;
  scale_palette_rows_from_backup_bank(0, 0x20, 0xd, 0);
  scale_palette_rows_from_backup_bank(1, 0x20, 0xd, 0);
  scale_palette_rows_from_backup_bank(2, 0x20, 0xd, 0);
  scale_palette_rows_from_backup_bank(3, 0x20, 0xd, 0);
  queue_five_jobs();
  game_state.loop_mode = 0;
  v = 0x700;
  set_menu_command_by_code(1, v);
  while ((menu_active_flag == 0) || (game_state.wait_flag != 0))
  {
    select_buffers_and_run_first_setup();
    request_sound_command_and_switch_thread(1);
  }

  n = 0x77;
  do
  {
    v = 0x1a;
    if ((u8)is_any_enabled_pad_button_pressed() != 0)
    {
      set_menu_command_by_code(6, 2);
      break;
    }
    if (n & 2)
    {
      data_8016e863 = v;
    }
    else
    {
      data_8016e863 = 0x10;
    }
    add_item_to_table(&data_8016e858);
    select_buffers_and_run_first_setup();
    request_sound_command_and_switch_thread(1);
  }
  while (new_var = n--);
  copy_palette_rows_from_backup_bank(0, 0x20, 0);
  copy_palette_rows_from_backup_bank(1, 0x20, 0);
  ;
  copy_palette_rows_from_backup_bank(2, 0x20, 0);
  copy_palette_rows_from_backup_bank(3, 0x20, 0);
  queue_five_jobs();
}
