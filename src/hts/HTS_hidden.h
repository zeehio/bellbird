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

#ifndef HTS_HIDDEN_H
#define HTS_HIDDEN_H

#include <stdint.h>
#include "cst_alloc.h"
/* hts_engine libraries */
#include "HTS_engine.h"

// misc

typedef struct _HTS_File {
   unsigned char type;
   void *pointer;
} HTS_File;

/* HTS_fopen: wrapper for fopen */
HTS_File *HTS_fopen_from_fn(const char *name, const char *opt);

/* HTS_fopen_from_fp: wrapper for fopen */
HTS_File *HTS_fopen_from_fp(HTS_File * fp, size_t size);

/* HTS_fopen_from_data: wrapper for fopen */
HTS_File *HTS_fopen_from_data(void *data, size_t size);

/* HTS_fclose: wrapper for fclose */
void HTS_fclose(HTS_File * fp);

/* HTS_feof: wrapper for feof */
int HTS_feof(HTS_File * fp);

/* HTS_fseek: wrapper for fseek */
int HTS_fseek(HTS_File * fp, long offset, int origin);

/* HTS_ftell: wrapper for ftell */
size_t HTS_ftell(HTS_File * fp);

/* HTS_fread_little_endian: fread with byteswap */
size_t HTS_fread_little_endian(void *buf, size_t size, size_t n, HTS_File * fp);

/* bell_get_pattern_token: get pattern token (single/double quote can be used) */
bell_boolean bell_get_pattern_token(HTS_File * fp, char *buff, size_t bufflen);

/* bell_get_token_from_fp: get token from file pointer (separators are space,tab,line break) */
bell_boolean bell_get_token_from_fp(HTS_File * fp, char *buff, size_t bufflen);

/* bell_get_token_from_fp_with_separator: get token from file pointer with specified separator */
bell_boolean bell_get_token_from_fp_with_separator(HTS_File * fp, char *buff, size_t bufflen, char separator);

/* bell_get_token_from_string_with_separator: get token from string with specified separator */
bell_boolean bell_get_token_from_string_with_separator(const char *str, size_t * index, char *buff, size_t bufflen, char separator);

/* HTS_finv: calculate 1.0/variance function */
double HTS_finv(const double x);

/*    Extra stuff we need */
#define HTS_MAXBUFLEN 1024

/* common ---------------------------------------------------------- */

#if !defined(WORDS_BIGENDIAN) && !defined(WORDS_LITTLEENDIAN)
#define WORDS_LITTLEENDIAN
#endif                          /* !WORDS_BIGENDIAN && !WORDS_LITTLEENDIAN */
#if defined(WORDS_BIGENDIAN) && defined(WORDS_LITTLEENDIAN)
#undef WORDS_BIGENDIAN
#endif                          /* WORDS_BIGENDIAN && WORDS_LITTLEENDIAN */

#define MAX_LF0   9.9034875525361280454891979401956     /* log(20000.0) */
#define MIN_LF0   2.9957322735539909934352235761425     /* log(20.0) */

/* model ----------------------------------------------------------- */

/* HTS_ModelSet_initialize: initialize model set */
void HTS_ModelSet_initialize(HTS_ModelSet * ms);

/* HTS_ModelSet_load: load HTS voices */
HTS_Boolean HTS_ModelSet_load(HTS_ModelSet * ms, char **voices);

/* HTS_ModelSet_get_sampling_frequency: get sampling frequency of HTS voices */
size_t HTS_ModelSet_get_sampling_frequency(HTS_ModelSet * ms);

/* HTS_ModelSet_get_fperiod: get frame period of HTS voices */
size_t HTS_ModelSet_get_fperiod(HTS_ModelSet * ms);

/* HTS_ModelSet_get_fperiod: get stream option */
const char *HTS_ModelSet_get_option(HTS_ModelSet * ms, size_t stream_index);

/* HTS_ModelSet_get_gv_flag: get GV flag */
HTS_Boolean HTS_ModelSet_get_gv_flag(HTS_ModelSet * ms, const char *string);

/* HTS_ModelSet_get_nstate: get number of state */
size_t HTS_ModelSet_get_nstate(HTS_ModelSet * ms);

/* HTS_ModelSet_get_nstream: get number of stream */
size_t HTS_ModelSet_get_nstream(HTS_ModelSet * ms);

/* HTS_ModelSet_get_vector_length: get vector length */
size_t HTS_ModelSet_get_vector_length(HTS_ModelSet * ms, size_t stream_index);

/* HTS_ModelSet_is_msd: get MSD flag */
HTS_Boolean HTS_ModelSet_is_msd(HTS_ModelSet * ms, size_t stream_index);

/* HTS_ModelSet_get_window_size: get dynamic window size */
size_t HTS_ModelSet_get_window_size(HTS_ModelSet * ms, size_t stream_index);

/* HTS_ModelSet_get_window_left_width: get left width of dynamic window */
int HTS_ModelSet_get_window_left_width(HTS_ModelSet * ms, size_t stream_index, size_t window_index);

/* HTS_ModelSet_get_window_right_width: get right width of dynamic window */
int HTS_ModelSet_get_window_right_width(HTS_ModelSet * ms, size_t stream_index, size_t window_index);

/* HTS_ModelSet_get_window_coefficient: get coefficient of dynamic window */
double HTS_ModelSet_get_window_coefficient(HTS_ModelSet * ms, size_t stream_index, size_t window_index, size_t coefficient_index);

/* HTS_ModelSet_get_window_max_width: get max width of dynamic window */
size_t HTS_ModelSet_get_window_max_width(HTS_ModelSet * ms, size_t stream_index);

/* HTS_ModelSet_use_gv: get GV flag */
HTS_Boolean HTS_ModelSet_use_gv(HTS_ModelSet * ms, size_t stream_index);

/* HTS_ModelSet_get_duration: get duration */
void HTS_ModelSet_get_duration(HTS_ModelSet * ms, const char *string, double *mean, double *vari);

/* HTS_ModelSet_get_parameter: get parameter */
void HTS_ModelSet_get_parameter(HTS_ModelSet * ms, size_t stream_index, size_t state_index, const char *string, double *mean, double *vari, double *msd);

/* HTS_ModelSet_get_gv: get GV */
void HTS_ModelSet_get_gv(HTS_ModelSet * ms, size_t stream_index, const char *string, double *mean, double *vari);

/* HTS_ModelSet_clear: free model set */
void HTS_ModelSet_clear(HTS_ModelSet * ms);

/* sstream --------------------------------------------------------- */

/* HTS_SStreamSet_initialize: initialize state stream set */
void HTS_SStreamSet_initialize(HTS_SStreamSet * sss);

/* HTS_SStreamSet_create: parse label and determine state duration */
HTS_Boolean HTS_SStreamSet_create(HTS_SStreamSet * sss, HTS_ModelSet * ms, char **label, size_t num_labels, double speed);

/* HTS_SStreamSet_get_nstream: get number of stream */
size_t HTS_SStreamSet_get_nstream(HTS_SStreamSet * sss);

/* HTS_SStreamSet_get_vector_length: get vector length */
size_t HTS_SStreamSet_get_vector_length(HTS_SStreamSet * sss, size_t stream_index);

/* HTS_SStreamSet_is_msd: get MSD flag */
HTS_Boolean HTS_SStreamSet_is_msd(HTS_SStreamSet * sss, size_t stream_index);

/* HTS_SStreamSet_get_total_state: get total number of state */
size_t HTS_SStreamSet_get_total_state(HTS_SStreamSet * sss);

/* HTS_SStreamSet_get_total_frame: get total number of frame */
size_t HTS_SStreamSet_get_total_frame(HTS_SStreamSet * sss);

/* HTS_SStreamSet_get_msd: get msd parameter */
double HTS_SStreamSet_get_msd(HTS_SStreamSet * sss, size_t stream_index, size_t state_index);

/* HTS_SStreamSet_window_size: get dynamic window size */
size_t HTS_SStreamSet_get_window_size(HTS_SStreamSet * sss, size_t stream_index);

// Transfer ownership of window left width to caller
int *HTS_SStreamSet_abandon_window_left_width(HTS_SStreamSet * sss, size_t stream_index);

// Transfer ownership of window right width to caller
int *HTS_SStreamSet_abandon_window_right_width(HTS_SStreamSet * sss, size_t stream_index);

// Transfer ownership of window coefficient to caller
double **HTS_SStreamSet_abandon_window_coefficient(HTS_SStreamSet * sss, size_t stream_index);

/* HTS_SStreamSet_get_window_max_width: get max width of dynamic window */
size_t HTS_SStreamSet_get_window_max_width(HTS_SStreamSet * sss, size_t stream_index);

/* HTS_SStreamSet_use_gv: get GV flag */
HTS_Boolean HTS_SStreamSet_use_gv(HTS_SStreamSet * sss, size_t stream_index);

/* HTS_SStreamSet_get_duration: get state duration */
size_t HTS_SStreamSet_get_duration(HTS_SStreamSet * sss, size_t state_index);

/* HTS_SStreamSet_get_mean: get mean parameter */
double HTS_SStreamSet_get_mean(HTS_SStreamSet * sss, size_t stream_index, size_t state_index, size_t vector_index);

/* HTS_SStreamSet_set_mean: set mean parameter */
void HTS_SStreamSet_set_mean(HTS_SStreamSet * sss, size_t stream_index, size_t state_index, size_t vector_index, double f);

/* HTS_SStreamSet_get_vari: get variance parameter */
double HTS_SStreamSet_get_vari(HTS_SStreamSet * sss, size_t stream_index, size_t state_index, size_t vector_index);

// Transfer ownership of GV mean to caller
double *HTS_SStreamSet_abandon_gv_mean(HTS_SStreamSet * sss, size_t stream_index);

// Transfer ownership of GV variance to caller
double *HTS_SStreamSet_abandon_gv_vari(HTS_SStreamSet * sss, size_t stream_index);

/* HTS_SStreamSet_get_gv_switch: get GV switch */
HTS_Boolean HTS_SStreamSet_get_gv_switch(HTS_SStreamSet * sss, size_t stream_index, size_t state_index);

/* HTS_SStreamSet_clear: free state stream set */
void HTS_SStreamSet_clear(HTS_SStreamSet * sss);

/* pstream --------------------------------------------------------- */

/* HTS_PStreamSet_initialize: initialize parameter stream set */
void HTS_PStreamSet_initialize(HTS_PStreamSet * pss);

/* HTS_PStreamSet_create: parameter generation using GV weight */
HTS_Boolean HTS_PStreamSet_create(HTS_PStreamSet * pss, HTS_SStreamSet * sss, double *msd_threshold);

/* HTS_PStreamSet_get_nstream: get number of stream */
size_t HTS_PStreamSet_get_nstream(HTS_PStreamSet * pss);

/* HTS_PStreamSet_get_vector_length: get features length */
size_t HTS_PStreamSet_get_vector_length(HTS_PStreamSet * pss, size_t stream_index);

/* HTS_PStreamSet_get_total_frame: get total number of frame */
size_t HTS_PStreamSet_get_total_frame(HTS_PStreamSet * pss);

/* HTS_PStreamSet_get_parameter: get parameter */
double HTS_PStreamSet_get_parameter(HTS_PStreamSet * pss, size_t stream_index, size_t frame_index, size_t vector_index);

// Transfer ownership of parameter to caller
double **HTS_PStreamSet_abandon_parameter(HTS_PStreamSet * pss, size_t stream_index);

/* HTS_PStreamSet_get_msd_flag: get generated MSD flag per frame */
HTS_Boolean HTS_PStreamSet_get_msd_flag(HTS_PStreamSet * pss, size_t stream_index, size_t frame_index);

/* HTS_PStreamSet_is_msd: get MSD flag */
HTS_Boolean HTS_PStreamSet_is_msd(HTS_PStreamSet * pss, size_t stream_index);

/* HTS_PStreamSet_clear: free parameter stream set */
void HTS_PStreamSet_clear(HTS_PStreamSet * pss);

/* gstream --------------------------------------------------------- */

/* HTS_GStreamSet_initialize: initialize generated parameter stream set */
void HTS_GStreamSet_initialize(HTS_GStreamSet * gss);

/* HTS_GStreamSet_create: generate speech */
HTS_Boolean HTS_GStreamSet_create(HTS_GStreamSet * gss, HTS_PStreamSet * pss, size_t sampling_rate, size_t fperiod, double alpha, double beta);

/* HTS_GStreamSet_get_total_nsamples: get total number of sample */
size_t HTS_GStreamSet_get_total_nsamples(HTS_GStreamSet * gss);

// Transfer ownership of generated speech array to caller
int16_t * HTS_GStreamSet_abandon_speech_array(HTS_GStreamSet * gss);

/* HTS_GStreamSet_clear: free generated parameter stream set */
void HTS_GStreamSet_clear(HTS_GStreamSet * gss);

/* vocoder --------------------------------------------------------- */

/* HTS_Vocoder: structure for setting of vocoder */
typedef struct _HTS_Vocoder {
   HTS_Boolean is_first;
   size_t fprd;                 /* frame shift */
   unsigned long next;          /* temporary variable for random generator */
   double rate;                 /* sampling rate */
   double pitch_of_curr_point;  /* used in excitation generation */
   double pitch_counter;        /* used in excitation generation */
   double pitch_inc_per_point;  /* used in excitation generation */
   double *excite_ring_buff;    /* used in excitation generation */
   size_t excite_buff_size;     /* used in excitation generation */
   size_t excite_buff_index;    /* used in excitation generation */
   unsigned char sw;            /* switch used in random generator */
   double *spectrum2en_buff;    /* used in spectrum2en */
   size_t spectrum2en_size;     /* buffer size for spectrum2en */
   double r1, r2, s;            /* used in random generator */
   double *postfilter_buff;     /* used in postfiltering */
   size_t postfilter_size;      /* buffer size for postfiltering */
   double *c, *cc, *cinc, *d1;  /* used in the MLSA filter */
   int    d2offset;             /* start of history terms in wraparound buffer of MLSA filter */
} HTS_Vocoder;

/* HTS_Vocoder_initialize: initialize vocoder */
void HTS_Vocoder_initialize(HTS_Vocoder * v, size_t m, size_t rate, size_t fperiod);

/* HTS_Vocoder_synthesize: pulse/noise excitation and MLSA filter based waveform synthesis */
void HTS_Vocoder_synthesize(HTS_Vocoder * v, size_t m, double lf0, double *spectrum, size_t nlpf, double *lpf, double alpha, double beta, int16_t *wavedata);

/* HTS_Vocoder_clear: clear vocoder */
void HTS_Vocoder_clear(HTS_Vocoder * v);

#endif                          /* !HTS_HIDDEN_H */
