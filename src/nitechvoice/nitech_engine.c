/*************************************************************************/
/*                This code has been modified for Bellbird.              */
/*                See COPYING for more copyright details.                */
/*                The unmodified source code copyright notice            */
/*                is included below.                                     */
/*************************************************************************/
/*  ---------------------------------------------------------------  */
/*      The HMM-Based Speech Synthesis System (HTS): version 1.1b    */
/*                        HTS Working Group                          */
/*                                                                   */
/*                   Department of Computer Science                  */
/*                   Nagoya Institute of Technology                  */
/*                                and                                */
/*    Interdisciplinary Graduate School of Science and Engineering   */
/*                   Tokyo Institute of Technology                   */
/*                      Copyright (c) 2001-2003                      */
/*                        All Rights Reserved.                       */
/*                                                                   */
/*  Permission is hereby granted, free of charge, to use and         */
/*  distribute this software and its documentation without           */
/*  restriction, including without limitation the rights to use,     */
/*  copy, modify, merge, publish, distribute, sublicense, and/or     */
/*  sell copies of this work, and to permit persons to whom this     */
/*  work is furnished to do so, subject to the following conditions: */
/*                                                                   */
/*    1. The code must retain the above copyright notice, this list  */
/*       of conditions and the following disclaimer.                 */
/*                                                                   */
/*    2. Any modifications must be clearly marked as such.           */
/*                                                                   */    
/*  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,  */
/*  HTS WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM    */
/*  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL       */
/*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF        */
/*  TECHNOLOGY, HTS WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE    */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY        */
/*  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  */
/*  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS   */
/*  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          */
/*  PERFORMANCE OF THIS SOFTWARE.                                    */
/*                                                                   */
/*  ---------------------------------------------------------------  */
/*    hts_engine.c : a compact HMM-based speech synthesis engine     */
/*                                                                   */ 
/*                                    2003/06/11 by Heiga Zen        */
/*  ---------------------------------------------------------------  */
/*  Modified by Alan W Black (awb@cs.cmu.edu)                        */
/*  April 2004                                                       */
/*  Make it compile with c++ and integrate as a Festival module      */
/*  ---------------------------------------------------------------  */

#include <ctype.h>
#include "cst_alloc.h"
#include "cst_error.h"
#include "cst_string.h"
#include "cst_utt_utils.h"
#include "cst_utterance.h"
#include "cst_wave.h"
#include "nitech_hidden.h"
#include "HTS_misc.h"
#include "bell_file.h"

// Choose between Unix and Windows directory separator character
#ifdef BELL_WINDOWS

#define DIR_SEP "\\"

#else

#define DIR_SEP "/"

#endif

static cst_wave * nitech_process (char **lines, size_t num_lines,
                   PStreamChol *mceppst, PStreamChol *lf0pst, nitechP *gp,
                   ModelSet *ms, TreeSet *ts, VocoderSetup *vs)
{
    cst_wave *w;
    char buff[HTS_MAXBUFLEN];
    Tree *tree;
    int state, diffdur=0;
    int i;
    size_t j;
    size_t data_index;
    Model *m, *mm, *nm;
    UttModel um;

    m = cst_alloc(Model,1);
    um.mtail = um.mhead = m;
    um.totalframe = um.nState = um.nModel = 0;

    for (j = 0; j < num_lines; j++) {

// copy label
        if (!isgraph((int) lines[j][0]))
            break;
        data_index = 0;
        if (bell_validate_atoi(lines[j], NULL)) {   /* has frame information */
            bell_get_token_from_string(lines[j], &data_index, buff, HTS_MAXBUFLEN);
            /* throw away start information */
            bell_get_token_from_string(lines[j], &data_index, buff, HTS_MAXBUFLEN);
            /* throw away end information */
            bell_get_token_from_string(lines[j], &data_index, buff, HTS_MAXBUFLEN);
            m->name = cst_strdup(buff);
        } else {
            m->name = cst_strdup(lines[j]);
        }

// for duration
        m->durpdf = SearchTree(m->name, ts->thead[DUR]->root);
        FindDurPDF(m, ms, gp->rho, diffdur);
        um.totalframe += m->totaldur;

// for excitation
        m->lf0pdf      = cst_alloc(int,ms->nstate+2);
        m->lf0mean     = cst_alloc(float *,ms->nstate+2);
        m->lf0variance =  cst_alloc(float *,ms->nstate+2);
        m->voiced      = cst_alloc(bell_boolean, ms->nstate+2); /* added 2 to see if still overruns data */

        for (tree=ts->thead[LF0],state=2; tree!=ts->ttail[LF0]; tree=tree->next,state++) {
            m->lf0pdf[state] = SearchTree(m->name, tree->root);
            FindLF0PDF(state, m, ms, gp->uv);
        }

// for spectrum
        m->mceppdf      = cst_alloc(int,ms->nstate+2);
        m->mcepmean     = cst_alloc(float *,ms->nstate+2);
        m->mcepvariance = cst_alloc(float *,ms->nstate+2);

        for (tree=ts->thead[MCP],state=2; tree!=ts->ttail[MCP]; tree=tree->next,state++) {
            m->mceppdf[state] = SearchTree(m->name, tree->root);
            FindMcpPDF(state, m, ms);
        }

      m->next = cst_alloc(Model,1);
      m = um.mtail = m->next;
      
      um.nModel++;
      um.nState+=ms->nstate;
    }
   
    w=pdf2speech(mceppst,lf0pst,gp,ms,&um,vs);

// Traverse HMM linked list freeing contents
    for (mm=um.mhead; mm; mm=nm)
    {
        nm = mm->next;
        for (i=0; i<ms->nstate+2; i++)
        {
            if (mm->lf0mean) cst_free(mm->lf0mean[i]);
            if (mm->lf0variance) cst_free(mm->lf0variance[i]);
        }
        cst_free(mm->mcepvariance);
        cst_free(mm->mcepmean);
        cst_free(mm->mceppdf);
        cst_free(mm->voiced);
        cst_free(mm->lf0variance);
        cst_free(mm->lf0mean);
        cst_free(mm->lf0pdf);
        cst_free(mm->dur);
        cst_free(mm->name);
        cst_free(mm);
    }
    return w;
}

bell_boolean nitech_engine_synthesize_from_strings(nitech_engine * ntengine, char **lines, size_t num_lines)
{
    cst_wave *w;
    nitechP gp;
   
    /* default value for control parameters */
    gp.rho      = 0.0;  // speech rate control
    gp.alpha    = 0.42; // all-pass constant for frequency warping
    gp.uv       = 0.5;  // UV threshold for voiced/unvoiced status
   
    /* generate speech */
    w=nitech_process(lines, num_lines, &(ntengine->mceppst), &(ntengine->lf0pst),
                       &gp, ntengine->ms, ntengine->ts, &(ntengine->vs));

    /* Attach waveform to utterance for data return */
    utt_set_wave(ntengine->utt,w);

    return TRUE;
}

bell_boolean nitech_engine_initialize(nitech_engine *ntengine, const char * fn_voice)
{
    char * filename;
    bell_boolean retval = TRUE;

    /* delta window handler for log f0 */
    ntengine->lf0pst.dw.fn = cst_alloc(char *,3);
    ntengine->lf0pst.dw.fn[1] = bell_build_filename(fn_voice,"hts"DIR_SEP"lf0_dyn.win");
    ntengine->lf0pst.dw.fn[2] = bell_build_filename(fn_voice,"hts"DIR_SEP"lf0_acc.win");
    ntengine->lf0pst.dw.num = 3;

    ReadWin(&(ntengine->lf0pst));

    /* delta window handler for mel-cepstrum */
    ntengine->mceppst.dw.fn = cst_alloc(char *,3);
    ntengine->mceppst.dw.fn[1] = bell_build_filename(fn_voice,"hts"DIR_SEP"mcep_dyn.win");
    ntengine->mceppst.dw.fn[2] = bell_build_filename(fn_voice,"hts"DIR_SEP"mcep_acc.win");
    ntengine->mceppst.dw.num = 3;

    ReadWin(&(ntengine->mceppst));

    /* initialize TreeSet and ModelSet */
    ntengine->ts = NULL;
    ntengine->ts = InitTreeSet();
    ntengine->ms = NULL;
    ntengine->ms = InitModelSet();

    filename = bell_build_filename(fn_voice,"hts"DIR_SEP"trees-dur.inf");
    ntengine->ts->fp[DUR]=bell_fopen(filename,"r");
    cst_free(filename);
    if (ntengine->ts->fp[DUR] == NULL)
    {
        cst_errmsg("nitech_engine: can't open hts"DIR_SEP"trees-dur.inf file\n");
        retval = FALSE;
    }
    filename = bell_build_filename(fn_voice,"hts"DIR_SEP"trees-lf0.inf");
    ntengine->ts->fp[LF0]=bell_fopen(filename,"r");
    cst_free(filename);
    if (ntengine->ts->fp[LF0] == NULL)
    {
        cst_errmsg("nitech_engine: can't open hts"DIR_SEP"trees-lf0.inf file\n");
        retval = FALSE;
    }
    filename = bell_build_filename(fn_voice,"hts"DIR_SEP"trees-mcep.inf");
    ntengine->ts->fp[MCP]=bell_fopen(filename,"r");
    cst_free(filename);
    if (ntengine->ts->fp[MCP] == NULL)
    {
        cst_errmsg("nitech_engine: can't open hts"DIR_SEP"trees-mcep.inf file\n");
        retval = FALSE;
    }
    filename = bell_build_filename(fn_voice,"hts"DIR_SEP"duration.pdf");
    ntengine->ms->fp[DUR]=bell_fopen(filename,"rb");
    cst_free(filename);
    if (ntengine->ms->fp[DUR] == NULL)
    {
        cst_errmsg("nitech_engine: can't open hts"DIR_SEP"duration.pdf file\n");
        retval = FALSE;
    }
    filename = bell_build_filename(fn_voice,"hts"DIR_SEP"lf0.pdf");
    ntengine->ms->fp[LF0]=bell_fopen(filename,"rb");
    cst_free(filename);
    if (ntengine->ms->fp[LF0] == NULL)
    {
        cst_errmsg("nitech_engine: can't open hts"DIR_SEP"lf0.pdf file\n");
        retval = FALSE;
    }
    filename = bell_build_filename(fn_voice,"hts"DIR_SEP"mcep.pdf");
    ntengine->ms->fp[MCP]=bell_fopen(filename,"rb");
    cst_free(filename);
    if (ntengine->ms->fp[MCP] == NULL)
    {
        cst_errmsg("nitech_engine: can't open hts/mcep.pdf file\n");
        retval = FALSE;
    }

    /* load tree files for duration, log f0 and mel-cepstrum */
    if (retval) LoadTreesFile(ntengine->ts, DUR);
    if (retval) LoadTreesFile(ntengine->ts, LF0);
    if (retval) LoadTreesFile(ntengine->ts, MCP);

    /* load model files for duration, log f0 and mel-cepstrum */
    if (retval) LoadModelFiles(ntengine->ms);

    // close tree and model files
    if (ntengine->ts->fp[DUR]) bell_fclose(ntengine->ts->fp[DUR]);
    if (ntengine->ts->fp[LF0]) bell_fclose(ntengine->ts->fp[LF0]);
    if (ntengine->ts->fp[MCP]) bell_fclose(ntengine->ts->fp[MCP]);
    if (ntengine->ms->fp[DUR]) bell_fclose(ntengine->ms->fp[DUR]);
    if (ntengine->ms->fp[LF0]) bell_fclose(ntengine->ms->fp[LF0]);
    if (ntengine->ms->fp[MCP]) bell_fclose(ntengine->ms->fp[MCP]);

    // check the number of window
    if (retval && ntengine->lf0pst.dw.num != ntengine->ms->lf0stream)
    {
        cst_errmsg("nitech_engine: dynamic window for f0 is illegal\n");
        retval = FALSE;
    }
    if (retval && ntengine->ms->mcepvsize % ntengine->mceppst.dw.num != 0)
    {
        cst_errmsg("nitech_engine: dynamic window for mcep is illegal\n");
        retval = FALSE;
    }

    // Initialize vocoder
    if (retval) init_vocoder(ntengine->ms->mcepvsize-1, &(ntengine->vs));

    return retval;
}

void nitech_engine_clear(nitech_engine * ntengine)
{
    int i;

// Free trees
    nitech_free_treeset(ntengine->ts);

// Free modelset
    DeleteModelSet(ntengine->ms);

// Free vocoder
    free_vocoder(&(ntengine->vs));

// Free windows
    for (i=1; i<ntengine->lf0pst.dw.num; i++)
    {
        cst_free(ntengine->lf0pst.dw.fn[i]);
    }
    cst_free(ntengine->lf0pst.dw.fn);
    for (i=1; i<ntengine->mceppst.dw.num; i++)
    {
        cst_free(ntengine->mceppst.dw.fn[i]);
    }
    cst_free(ntengine->mceppst.dw.fn);
    FreeWin(&(ntengine->lf0pst));
    FreeWin(&(ntengine->mceppst));
}
