## Data symbols, first batch: 37 named (2026-10-13)

The 40 resident data symbols with the most units using them were read by two Sonnet agents (20 each, read-only, from the uses in the source) and their names reviewed here. 37 were named with `ps1/tools/renamedata.py` (new, with its controls in `test_renamedata.py`); 3 were held because other symbols share their address (a second view of the same bytes). Data symbols named: 369 to 406 of 4,423.

Review changes to the agents' proposals: the two pad words of each port are named together (`pad_1_held`, `pad_2_held` with `pad_1_pressed`, `pad_2_pressed`); the third scratch word follows the first two (`scratch_word_c`); one flag was named for what the uses show, not for the CD (`menu_active_flag`); one gate was named for the handlers it gates, not for a stage (`kind_handlers_enabled`).

Evidence: 963 files rewritten, 3,868 places; every changed file reads back to its earlier text when the 37 names are put back as whole words (0 of 963 differ). 49 of the 56 images that this Mac's compiler can build were rebuilt and have the same image hashes and exact counts as before; the other 7 hit the compiler crash recorded in the layout record and are covered by the reverse check only. The renamer's 7 controls and the port's host build controls pass.

| Old | New | Confidence |
| --- | --- | --- |
| `data_8018f5a0` | `screen_state` | medium |
| `data_801a27d0` | `draw_buffer_index` | high |
| `data_801987c8` | `current_ordering_table` | high |
| `data_801aa544` | `background_layers` | high |
| `data_801aa5d4` | `main_layer` | high |
| `data_80190568` | `kind_handlers_enabled` | medium |
| `data_80190468` | `ref_game_state` | medium |
| `data_8019045c` | `scratch_word_a` | medium |
| `data_80190464` | `scratch_word_b` | medium |
| `data_80189460` | `script_cursor` | high |
| `data_1f8000b4` | `seqs_b4_left` | medium |
| `data_1f800164` | `seqs_164_right` | medium |
| `data_801a696a` | `pad_1_pressed` | high |
| `data_801a6976` | `pad_2_pressed` | high |
| `data_80172a48` | `spawned_object_data_a` | medium |
| `data_80173c9c` | `spawned_object_data_b` | medium |
| `data_801ad398` | `script_condition_result` | high |
| `data_801904d8` | `fourth_layer` | medium |
| `data_801adfe4` | `tile_sprite_count` | high |
| `data_80190542` | `fourth_layer_offset_x` | medium |
| `data_80190458` | `ref_aux` | medium |
| `data_801a6966` | `pad_1_held` | high |
| `data_80189464` | `script_work_value` | medium |
| `data_801a6984` | `input_block` | medium |
| `data_801a6972` | `pad_2_held` | high |
| `data_80197f10` | `free_block_count` | high |
| `table_8016e5c4` | `score_rank_table` | medium |
| `data_8019046c` | `scratch_word_c` | medium |
| `data_80190949` | `menu_active_flag` | medium |
| `data_801ae02c` | `single_object_active` | medium |
| `table_8017a8cc` | `slot_input_table` | medium |
| `table_8016e614` | `streak_rank_table` | medium |
| `data_8018f598` | `white_fill_active` | high |
| `data_8018db10` | `palette_altered_flag` | medium |
| `data_80188f44` | `slot_result_flag` | high |
| `data_801aa5ea` | `view_scroll_y` | medium |
| `data_801aa4dc` | `palette_fade_level` | high |

Held for the next batch: three symbols that share an address with another symbol (the palette banks, the box overlap record, the button configuration). Not rewritten: records and history, which keep the names of their day.
