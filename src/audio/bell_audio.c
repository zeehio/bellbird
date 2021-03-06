/*************************************************************************/
/*                This code has been created for Bellbird.               */
/*                See COPYING for more copyright details.                */
/*************************************************************************/
#include "cst_alloc.h"
#include "cst_error.h"
#include "bell_audio.h"

bell_boolean play_wave(cst_wave *w, cst_audiodev *ad)
{
    int retval = TRUE;

    if (!w || w->num_samples == 0) return FALSE;

    if (AUDIO_WRITE_NATIVE(ad,&w->samples[0],w->num_samples) <= 0)
    {
        cst_errmsg("failed to write %d samples\n",w->num_samples);
        retval = FALSE;
    }

    return retval;
}
