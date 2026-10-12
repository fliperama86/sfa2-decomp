/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void read_stream_sequence_and_open(Ctl *ctl)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[8];
  u32 v;
  u32 n;
  u8 **t;
  u8 *p;
  if (((Stream *)ctl)->step_count == 0)
  {
    t = data_8017eb1c[((Stream *)ctl)->source_index];
    v = *(u32 *)((Stream *)ctl)->current_record;
    n = v & 0xff;
    n = n + ((v & 0xff00) >> 6);
    p = t[n];
    ((Stream *)ctl)->source_ptr = p;
    ((Stream *)ctl)->open_source_ptr = p;
  }
  read_stream_sector_to_source_ptr((Stream *)ctl);
  ((Stream *)ctl)->step_count++;
  if (((Stream *)ctl)->bytes_remaining == 0)
  {
    u8 *r = ((Stream *)ctl)->current_record;
    u32 k = r[0];
    n = r[1];
    if (n == 0)
    {
      data_80190a44[k + 4] = func_80168d4c(((Stream *)ctl)->open_source_ptr, data_80190a44[k]);
    }
    else
    {
      data_80190a44[k + 8] = func_80166144(((Stream *)ctl)->open_source_ptr, data_80190a44[k], ((s16 *)r)[6]);
    }
  }
}
