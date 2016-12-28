/*
  Copyright (C) 2003-2013 Paul Brossier <piem@aubio.org>

  This file is part of aubio.

  aubio is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  aubio is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with aubio.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifdef OPENME
#include <openme.h>
#endif
#ifdef XOPENME
#include <xopenme.h>
#endif

#include "utils.h"
#define PROG_HAS_PITCH 1
#define PROG_HAS_ONSET 1
#define PROG_HAS_SILENCE 1
#define PROG_HAS_JACK 1
// TODO add PROG_HAS_OUTPUT
#include "parse_args.h"

aubio_notes_t *notes;
smpl_t lastmidi = 0.;

long ct_repeat=0;
long ct_repeat_max=1;

void ck_send_noteon(smpl_t pitch, smpl_t velo) {
  // only do something the last time
  if (ct_repeat == ct_repeat_max-1) {
    send_noteon(pitch, velo);
  }
}

void process_block (fvec_t *ibuf, fvec_t *obuf)
{
  aubio_notes_do (notes, ibuf, obuf);
  // did we get a note off?
  if (obuf->data[2] != 0) {
    lastmidi = aubio_freqtomidi (obuf->data[2]) + .5;
    ck_send_noteon(lastmidi, 0);
  }
  // did we get a note on?
  if (obuf->data[0] != 0) {
    lastmidi = aubio_freqtomidi (obuf->data[0]) + .5;
    ck_send_noteon(lastmidi, obuf->data[1]);
  }
}

void process_print (void)
{
  //if (verbose) outmsg("%f\n",pitch_obuf->data[0]);
}

int main(int argc, char **argv) {
  int ret = 0;

#ifdef OPENME
  openme_init(NULL,NULL,NULL,0);
  openme_callback("PROGRAM_START", NULL);
#endif
#ifdef XOPENME
  xopenme_init(1,0);
#endif

  if (getenv("CT_REPEAT_MAIN")!=NULL) ct_repeat_max=atol(getenv("CT_REPEAT_MAIN"));
              
  /* parse command line arguments */
  parse_args (argc, argv);

#ifdef OPENME
  openme_callback("KERNEL_START", NULL);
#endif
#ifdef XOPENME
  xopenme_clock_start(0);
#endif

  for (ct_repeat=0; ct_repeat<ct_repeat_max; ct_repeat++) {

    examples_common_init(argc,argv);

    notes = new_aubio_notes ("default", buffer_size, hop_size, samplerate);
    if (notes == NULL) { ret = 1; goto beach; }

    examples_common_process((aubio_process_func_t)process_block, process_print);

    // send a last note off if required
    if (lastmidi) {
      ck_send_noteon (lastmidi, 0);
    }

    del_aubio_notes (notes);
    lastmidi = 0.;

    examples_common_del();

  }

beach:

#ifdef XOPENME
  xopenme_clock_end(0);
#endif
#ifdef OPENME
  openme_callback("KERNEL_END", NULL);
#endif

#ifdef XOPENME
  xopenme_dump_state();
  xopenme_finish();
#endif
#ifdef OPENME
  openme_callback("PROGRAM_END", NULL);
#endif

  return ret;
}
