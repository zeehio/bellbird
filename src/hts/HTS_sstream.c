/*************************************************************************/
/*                This code has been modified for Bellbird.              */
/*                See COPYING for more copyright details.                */
/*                The unmodified source code copyright notice            */
/*                is included below.                                     */
/*************************************************************************/
/* ----------------------------------------------------------------- */
/*           The HMM-Based Speech Synthesis Engine "hts_engine API"  */
/*           developed by HTS Working Group                          */
/*           http://hts-engine.sourceforge.net/                      */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2001-2013  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/*                2001-2008  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the HTS working group nor the names of its  */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */
#include <stdlib.h>
#include <math.h>

#include "cst_alloc.h"
#include "cst_error.h"
/* hts_engine libraries */
#include "HTS_hidden.h"

/* HTS_set_default_duration: set default duration from state duration probability distribution */
static void HTS_set_default_duration(size_t * duration, double *mean, size_t size)
{
   size_t i;
   double temp;

   for (i = 0; i < size; i++) {
      temp = mean[i] + 0.5;
      if (temp < 1.0)
         duration[i] = 1;
      else
         duration[i] = (size_t) temp;
   }
}

/* HTS_set_specified_duration: set duration from state duration probability distribution and specified frame length */
static double HTS_set_specified_duration(size_t * duration, double *mean, double *vari, size_t size, double frame_length)
{
   size_t i;
   int j;
   double temp1, temp2;
   double rho = 0.0;
   size_t sum = 0;
   size_t target_length;

   /* get the target frame length */
   if (frame_length + 0.5 < 1.0)
      target_length = 1;
   else
      target_length = (size_t) (frame_length + 0.5);

   /* check the specified duration */
   if (target_length <= size) {
      if (target_length < size)
         cst_errmsg("Warning: HTS_set_specified_duration: Specified frame length is too short.\n");
      for (i = 0; i < size; i++)
         duration[i] = 1;
      return (double) size;
   }

   /* RHO calculation */
   temp1 = 0.0;
   temp2 = 0.0;
   for (i = 0; i < size; i++) {
      temp1 += mean[i];
      temp2 += vari[i];
   }
   rho = ((double) target_length - temp1) / temp2;

   /* first estimation */
   for (i = 0; i < size; i++) {
      temp1 = mean[i] + rho * vari[i] + 0.5;
      if (temp1 < 1.0)
         duration[i] = 1;
      else
         duration[i] = (size_t) temp1;
      sum += duration[i];
   }

   /* loop estimation */
   while (target_length != sum) {
      /* sarch flexible state and modify its duration */
      if (target_length > sum) {
         j = -1;
         for (i = 0; i < size; i++) {
            temp2 = fabs(rho - ((double) duration[i] + 1 - mean[i]) / vari[i]);
            if (j < 0 || temp1 > temp2) {
               j = i;
               temp1 = temp2;
            }
         }
         sum++;
         duration[j]++;
      } else {
         j = -1;
         for (i = 0; i < size; i++) {
            if (duration[i] > 1) {
               temp2 = fabs(rho - ((double) duration[i] - 1 - mean[i]) / vari[i]);
               if (j < 0 || temp1 > temp2) {
                  j = i;
                  temp1 = temp2;
               }
            }
         }
         sum--;
         duration[j]--;
      }
   }

   return (double) target_length;
}

/* HTS_SStreamSet_initialize: initialize state stream set */
void HTS_SStreamSet_initialize(HTS_SStreamSet * sss)
{
   sss->nstream = 0;
   sss->nstate = 0;
   sss->sstream = NULL;
   sss->duration = NULL;
   sss->total_state = 0;
   sss->total_frame = 0;
}

/* HTS_SStreamSet_create: parse label and determine state duration */
HTS_Boolean HTS_SStreamSet_create(HTS_SStreamSet * sss, HTS_ModelSet * ms, char** label, size_t num_labels, double speed)
{
   size_t i, j, k;
   double temp;
   int shift;
   size_t state;
   HTS_SStream *sst;
   double *duration_mean, *duration_vari;
   double frame_length;

   /* initialize state sequence */
   sss->nstate = HTS_ModelSet_get_nstate(ms);
   sss->nstream = HTS_ModelSet_get_nstream(ms);
   sss->total_frame = 0;
   sss->total_state = num_labels * sss->nstate;
   sss->duration = cst_alloc(size_t,sss->total_state);
   sss->sstream = cst_alloc(HTS_SStream,sss->nstream);
   for (i = 0; i < sss->nstream; i++) {
      sst = &sss->sstream[i];
      sst->vector_length = HTS_ModelSet_get_vector_length(ms, i);
      sst->mean = bell_alloc_dmatrix(sss->total_state, sst->vector_length * HTS_ModelSet_get_window_size(ms, i));
      sst->vari = bell_alloc_dmatrix(sss->total_state, sst->vector_length * HTS_ModelSet_get_window_size(ms, i));
      if (HTS_ModelSet_is_msd(ms, i))
         sst->msd = cst_alloc(double,sss->total_state);
      else
         sst->msd = NULL;
      if (HTS_ModelSet_use_gv(ms, i)) {
         sst->gv_switch = cst_alloc(HTS_Boolean,sss->total_state);
         for (j = 0; j < sss->total_state; j++)
            sst->gv_switch[j] = TRUE;
      } else {
         sst->gv_switch = NULL;
      }
   }

   /* determine state duration */
   duration_mean = cst_alloc(double,sss->total_state);
   duration_vari = cst_alloc(double,sss->total_state);
   for (i = 0; i < num_labels; i++)
      HTS_ModelSet_get_duration(ms, label[i], &duration_mean[i * sss->nstate], &duration_vari[i * sss->nstate]);

   /* determine frame length */
   if (speed != 1.0) {
      temp = 0.0;
      for (i = 0; i < sss->total_state; i++) {
         temp += duration_mean[i];
      }
      frame_length = temp / speed;
      HTS_set_specified_duration(sss->duration, duration_mean, duration_vari, sss->total_state, frame_length);
   } else {
      HTS_set_default_duration(sss->duration, duration_mean, sss->total_state);
   }
   cst_free(duration_mean);
   cst_free(duration_vari);

   /* get parameter */
   for (i = 0, state = 0; i < num_labels; i++) {
      for (j = 2; j <= sss->nstate + 1; j++) {
         sss->total_frame += sss->duration[state];
         for (k = 0; k < sss->nstream; k++) {
            sst = &sss->sstream[k];
            if (sst->msd)
               HTS_ModelSet_get_parameter(ms, k, j, label[i], sst->mean[state], sst->vari[state], &sst->msd[state]);
            else
               HTS_ModelSet_get_parameter(ms, k, j, label[i], sst->mean[state], sst->vari[state], NULL);
         }
         state++;
      }
   }

   /* copy dynamic window */
   for (i = 0; i < sss->nstream; i++) {
      sst = &sss->sstream[i];
      sst->win_size = HTS_ModelSet_get_window_size(ms, i);
      sst->win_max_width = HTS_ModelSet_get_window_max_width(ms, i);
      sst->win_l_width = cst_alloc(int,sst->win_size);
      sst->win_r_width = cst_alloc(int,sst->win_size);
      sst->win_coefficient = cst_alloc(double *,sst->win_size);
      for (j = 0; j < sst->win_size; j++) {
         sst->win_l_width[j] = HTS_ModelSet_get_window_left_width(ms, i, j);
         sst->win_r_width[j] = HTS_ModelSet_get_window_right_width(ms, i, j);
         if (sst->win_l_width[j] + sst->win_r_width[j] == 0)
            sst->win_coefficient[j] = cst_alloc(double,(-2 * sst->win_l_width[j] + 1));
         else
            sst->win_coefficient[j] = cst_alloc(double,(-2 * sst->win_l_width[j]));
         sst->win_coefficient[j] -= sst->win_l_width[j];
         for (shift = sst->win_l_width[j]; shift <= sst->win_r_width[j]; shift++)
            sst->win_coefficient[j][shift] = HTS_ModelSet_get_window_coefficient(ms, i, j, shift);
      }
   }

   /* determine GV */
   for (i = 0; i < sss->nstream; i++) {
      sst = &sss->sstream[i];
      if (HTS_ModelSet_use_gv(ms, i)) {
         sst->gv_mean = cst_alloc(double,sst->vector_length);
         sst->gv_vari = cst_alloc(double,sst->vector_length);
         HTS_ModelSet_get_gv(ms, i, label[0], sst->gv_mean, sst->gv_vari);
      } else {
         sst->gv_mean = NULL;
         sst->gv_vari = NULL;
      }
   }

   for (i = 0; i < num_labels; i++)
      if (HTS_ModelSet_get_gv_flag(ms, label[i]) == FALSE)
         for (j = 0; j < sss->nstream; j++)
            if (HTS_ModelSet_use_gv(ms, j) == TRUE)
               for (k = 0; k < sss->nstate; k++)
                  sss->sstream[j].gv_switch[i * sss->nstate + k] = FALSE;

   return TRUE;
}

/* HTS_SStreamSet_get_nstream: get number of stream */
size_t HTS_SStreamSet_get_nstream(HTS_SStreamSet * sss)
{
   return sss->nstream;
}

/* HTS_SStreamSet_get_vector_length: get vector length */
size_t HTS_SStreamSet_get_vector_length(HTS_SStreamSet * sss, size_t stream_index)
{
   return sss->sstream[stream_index].vector_length;
}

/* HTS_SStreamSet_is_msd: get MSD flag */
HTS_Boolean HTS_SStreamSet_is_msd(HTS_SStreamSet * sss, size_t stream_index)
{
   return sss->sstream[stream_index].msd ? TRUE : FALSE;
}

/* HTS_SStreamSet_get_total_state: get total number of state */
size_t HTS_SStreamSet_get_total_state(HTS_SStreamSet * sss)
{
   return sss->total_state;
}

/* HTS_SStreamSet_get_total_frame: get total number of frame */
size_t HTS_SStreamSet_get_total_frame(HTS_SStreamSet * sss)
{
   return sss->total_frame;
}

/* HTS_SStreamSet_get_msd: get MSD parameter */
double HTS_SStreamSet_get_msd(HTS_SStreamSet * sss, size_t stream_index, size_t state_index)
{
   return sss->sstream[stream_index].msd[state_index];
}

/* HTS_SStreamSet_window_size: get dynamic window size */
size_t HTS_SStreamSet_get_window_size(HTS_SStreamSet * sss, size_t stream_index)
{
   return sss->sstream[stream_index].win_size;
}

int *HTS_SStreamSet_abandon_window_left_width(HTS_SStreamSet * sss, size_t stream_index)
{
//  Transfer ownership of window left width to caller
   int * retval;

   retval = sss->sstream[stream_index].win_l_width;
   sss->sstream[stream_index].win_l_width = NULL;
   return retval;
}

int *HTS_SStreamSet_abandon_window_right_width(HTS_SStreamSet * sss, size_t stream_index)
{
//  Transfer ownership of window right width to caller
   int * retval;

   retval = sss->sstream[stream_index].win_r_width;
   sss->sstream[stream_index].win_r_width = NULL;
   return retval;
}

double **HTS_SStreamSet_abandon_window_coefficient(HTS_SStreamSet * sss, size_t stream_index)
{
//  Transfer ownership of window coefficient to caller
   double ** retval;

   retval = sss->sstream[stream_index].win_coefficient;
   sss->sstream[stream_index].win_coefficient = NULL;
   return retval;
}

/* HTS_SStreamSet_get_window_max_width: get max width of dynamic window */
size_t HTS_SStreamSet_get_window_max_width(HTS_SStreamSet * sss, size_t stream_index)
{
   return sss->sstream[stream_index].win_max_width;
}

/* HTS_SStreamSet_use_gv: get GV flag */
HTS_Boolean HTS_SStreamSet_use_gv(HTS_SStreamSet * sss, size_t stream_index)
{
   return sss->sstream[stream_index].gv_mean ? TRUE : FALSE;
}

/* HTS_SStreamSet_get_duration: get state duration */
size_t HTS_SStreamSet_get_duration(HTS_SStreamSet * sss, size_t state_index)
{
   return sss->duration[state_index];
}

/* HTS_SStreamSet_get_mean: get mean parameter */
double HTS_SStreamSet_get_mean(HTS_SStreamSet * sss, size_t stream_index, size_t state_index, size_t vector_index)
{
   return sss->sstream[stream_index].mean[state_index][vector_index];
}

/* HTS_SStreamSet_set_mean: set mean parameter */
void HTS_SStreamSet_set_mean(HTS_SStreamSet * sss, size_t stream_index, size_t state_index, size_t vector_index, double f)
{
   sss->sstream[stream_index].mean[state_index][vector_index] = f;
}

/* HTS_SStreamSet_get_vari: get variance parameter */
double HTS_SStreamSet_get_vari(HTS_SStreamSet * sss, size_t stream_index, size_t state_index, size_t vector_index)
{
   return sss->sstream[stream_index].vari[state_index][vector_index];
}

double *HTS_SStreamSet_abandon_gv_mean(HTS_SStreamSet * sss, size_t stream_index)
{
//  Transfer ownership of GV mean to caller
   double * retval;

   retval = sss->sstream[stream_index].gv_mean;
   sss->sstream[stream_index].gv_mean = NULL;
   return retval;
}

double *HTS_SStreamSet_abandon_gv_vari(HTS_SStreamSet * sss, size_t stream_index)
//  Transfer ownership of GV variance to caller
{
   double * retval;

   retval = sss->sstream[stream_index].gv_vari;
   sss->sstream[stream_index].gv_vari = NULL;
   return retval;
}

/* HTS_SStreamSet_get_gv_switch: get GV switch */
HTS_Boolean HTS_SStreamSet_get_gv_switch(HTS_SStreamSet * sss, size_t stream_index, size_t state_index)
{
   return sss->sstream[stream_index].gv_switch[state_index];
}

/* HTS_SStreamSet_clear: free state stream set */
void HTS_SStreamSet_clear(HTS_SStreamSet * sss)
{
   size_t i, j;
   HTS_SStream *sst;

   if (sss->sstream) {
      for (i = 0; i < sss->nstream; i++) {
         sst = &sss->sstream[i];
         bell_free_dmatrix(sst->mean);
         bell_free_dmatrix(sst->vari);
         if (sst->msd)
            cst_free(sst->msd);
         if (sst->win_coefficient) {
            for (j = 0; j < sst->win_size; j++) {
               sst->win_coefficient[j] += sst->win_l_width[j];
               cst_free(sst->win_coefficient[j]);
            }
            cst_free(sst->win_coefficient);
         }
         if (sst->win_l_width)
            cst_free(sst->win_l_width);
         if (sst->win_r_width)
            cst_free(sst->win_r_width);
         if (sst->gv_mean)
            cst_free(sst->gv_mean);
         if (sst->gv_vari)
            cst_free(sst->gv_vari);
         if (sst->gv_switch)
            cst_free(sst->gv_switch);
      }
      cst_free(sss->sstream);
   }
   if (sss->duration)
      cst_free(sss->duration);

   HTS_SStreamSet_initialize(sss);
}
