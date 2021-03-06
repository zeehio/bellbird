/*************************************************************************/
/*                This code has been modified for Bellbird.              */
/*                See COPYING for more copyright details.                */
/*                The unmodified source code copyright notice            */
/*                is included below.                                     */
/*************************************************************************/
/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2007                            */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Authors:  Alan W Black (awb@cs.cmu.edu)                   */
/*                Date:  November 2007                                   */
/*************************************************************************/
/*                                                                       */
/*  Implementation of Clustergen, Statistical Parameter Synthesizer in   */
/*  Flite                                                                */
/*                                                                       */
/*  A statistical corpus based synthesizer.                              */
/*  See Black, A. (2006), CLUSTERGEN: A Statistical Parametric           */
/*  Synthesizer using Trajectory Modeling", Interspeech 2006 - ICSLP,    */
/*  Pittsburgh, PA.                                                      */
/*  http://www.cs.cmu.edu/~awb/papers/is2006/IS061394.PDF                */
/*                                                                       */
/*  Uses MLSA for resynthesis and MLPG for smoothing                     */
/*  mlsa and mlpg come from Festvox's VC code (which came in turn        */
/*  came from NITECH's HTS                                               */
/*                                                                       */
/*************************************************************************/

#include "cst_cg.h"
#include "cst_item.h"
#include "cst_spamf0.h"
#include "cst_utterance.h"
#include "bell_ff_sym.h"
#include "bell_relation_sym.h"

cst_cg_db *new_cg_db(void)
{
    cst_cg_db* db = cst_alloc(cst_cg_db, 1);
    db->name = NULL;
    db->types = NULL;
    db->f0_trees = NULL;
    db->param_trees = NULL;
    db->spamf0_accent_tree = NULL;
    db->spamf0_phrase_tree = NULL;
    db->spamf0_accent_vectors = NULL;
    db->num_channels = NULL;
    db->num_frames = NULL;
    db->model_vectors = NULL;
    db->model_min = NULL;
    db->model_range = NULL;
    db->dur_stats = NULL;
    db->dur_cart = NULL;
    db->phone_states = NULL;
    db->dynwin = NULL;
    db->me_h = NULL;
    return db;
}

void delete_cg_db(cst_cg_db *db)
{ // Delete cg_db even if it contains partially loaded data
    int i,j;

    if (NULL == db)
    {
        return;
    }
    /* Woo Hoo!  We're gonna free this garbage with a big mallet */
    /* In spite of what the const qualifiers say ... */
    if (db->name)
    {
        cst_free((void *)db->name);
    }
    if (db->types)
    {
        for (i=0; db->types[i]; i++)
        {
            cst_free((void *)db->types[i]);
        }
        cst_free((void *)db->types);
    }
    if (db->f0_trees)
    {
        for (i=0; db->f0_trees[i]; i++)
        {
             delete_cart((cst_cart *)(void *)db->f0_trees[i]);
        }
        cst_free((void *)db->f0_trees);
    }
    if (db->param_trees)
    {
        for (j=0; j<db->num_param_models; j++)
        {
            if (db->param_trees[j])
            {
                for (i=0; db->param_trees[j][i]; i++)
                {
                    delete_cart((cst_cart *)(void *)db->param_trees[j][i]);
                }
                cst_free((void *)db->param_trees[j]);
            }
        }
        cst_free((void *)db->param_trees);
    }
    if (db->spamf0)
    {
        delete_cart((cst_cart *)(void *)db->spamf0_accent_tree);
        delete_cart((cst_cart *)(void *)db->spamf0_phrase_tree);
        if (db->spamf0_accent_vectors)
        {
            cst_free((void *)db->spamf0_accent_vectors[0]);
            cst_free((void *)db->spamf0_accent_vectors);
        }
    }
    if (db->model_vectors)
    {
        for (j = 0; j < db->num_param_models; j++)
        {
            cst_free((void *)db->model_vectors[j][0]);
            cst_free((void *)db->model_vectors[j]);
        }
        cst_free((void *)db->model_vectors);
    }
    if (db->num_channels)
    {
        cst_free(db->num_channels);
    }
    if (db->num_frames)
    {
        cst_free(db->num_frames);
    }
    if (db->model_min)
    {
        cst_free((void *)db->model_min);
    }
    if (db->model_range)
    {
        cst_free((void *)db->model_range);
    }
    if (db->dur_stats)
    {
        for (j = 0; j < db->num_dur_models; j++)
        {
            if (db->dur_stats[j])
            {
                for (i = 0; db->dur_stats[j][i]; i++)
                {
                    cst_free((void *)db->dur_stats[j][i]->phone);
                    cst_free((void *)db->dur_stats[j][i]);
                }
                cst_free((void *)db->dur_stats[j]);
            }
        }
        cst_free((void *)db->dur_stats);
    }
    if (db->dur_cart)
    {
        for (j = 0; j < db->num_dur_models; j++)
        {
            delete_cart((void *)db->dur_cart[j]);
        }
        cst_free((void *)db->dur_cart);
    }
    if (db->phone_states)
    {
        for (i=0; db->phone_states[i]; i++)
        {
            for (j=0; db->phone_states[i][j]; j++)
            {
                cst_free((void *)db->phone_states[i][j]);
            }
            cst_free((void *)db->phone_states[i]);
        }
        cst_free((void *)db->phone_states);
    }
    if (db->dynwin)
    {
        cst_free((void *)db->dynwin);
    }
    if (db->me_h)
    {
        cst_free((void *)db->me_h[0]);
        cst_free((void *)db->me_h);
    }
    cst_free((void *)db);
}

static float cg_state_duration(cst_item *s, cst_cg_db *cg_db)
{
    float zdur, dur;
    const char *n;
    int i, x, dm;

    for (dm=0,zdur=0.0; dm < cg_db->num_dur_models; dm++)
        zdur += val_float(cart_interpret(s,cg_db->dur_cart[dm]));
    zdur /= dm;  /* get average zdur prediction from all dur models */
    n = item_feat_string(s,"name");

    /* Note we only use the dur stats from the first model, that is */
    /* correct, but wouldn't be if the dur tree was trained on different */
    /* data */
    for (x=i=0; cg_db->dur_stats[0][i]; i++)
    {
        if (cst_streq(cg_db->dur_stats[0][i]->phone,n))
        {
            x=i;
            break;
        }
    }
    if (!cg_db->dur_stats[0][i])  /* unknown type name */
        x = 0;

    dur = (zdur*cg_db->dur_stats[0][x]->stddev)+cg_db->dur_stats[0][x]->mean;

    /*    dur = 1.2 * (float)exp((float)dur); */

    return dur;
}

static void cg_make_hmmstates(cst_utterance *utt, cst_cg_db *cg_db)
{
    /* Build HMM state structure below the segment structure */
    cst_relation *hmmstate, *segstate;
    cst_item *seg, *s, *ss;
    const char *segname;
    int sp,p;

    hmmstate = utt_relation_create(utt,HMMSTATE);
    segstate = utt_relation_create(utt,SEGSTATE);

    for (seg = UTT_REL_HEAD(utt,SEGMENT); seg; seg=item_next(seg))
    {
        ss = relation_append(segstate,seg);
        segname = item_feat_string(seg,"name");
        for (p=0; cg_db->phone_states[p]; p++)
            if (cst_streq(segname,cg_db->phone_states[p][0]))
                break;
        if (cg_db->phone_states[p] == NULL)
            p = 0;  /* unknown phoneme */
        for (sp=1; cg_db->phone_states[p][sp]; sp++)
        {
            s = relation_append(hmmstate,NULL);
            item_add_daughter(ss,s);
            item_set_string(s,"name",cg_db->phone_states[p][sp]);
        }
    }

    return;
}

static int cg_make_params(cst_utterance *utt, cst_cg_db *cg_db)
{
    /* puts in the frame items */
    /* historically called "mcep" but can actually be any random vectors */
    cst_relation *mcep, *mcep_link;
    cst_item *s, *mcep_parent, *mcep_frame;
    int num_frames;
    float start, end;
    float dur_stretch, tok_stretch, rdur;

    mcep = utt_relation_create(utt,MCEP);
    mcep_link = utt_relation_create(utt,MCEP_LINK);
    end = 0.0;
    num_frames = 0;
    dur_stretch = get_param_float(utt->features,"duration_stretch", 1.0);

    for (s = UTT_REL_HEAD(utt,HMMSTATE); s; s=item_next(s))
    {
        start = end;
        tok_stretch = ffeature_float(s,"R:"SEGSTATE".P.R:"SYLSTRUCTURE".P.P.R:"TOKEN".P."LOCAL_DUR_STRETCH);
        if (tok_stretch == 0)
            tok_stretch = 1.0;
        rdur = tok_stretch*dur_stretch*cg_state_duration(s,cg_db);
        /* Guarantee duration to be alt least one frame */
        if (rdur < cg_db->frame_advance)
            end = start + cg_db->frame_advance;
        else
            end = start + rdur;
        item_set_float(s,"end",end);
        mcep_parent = relation_append(mcep_link, s);
        for ( ; (num_frames * cg_db->frame_advance) <= end; num_frames++ )
        {
            mcep_frame = relation_append(mcep,NULL);
            item_add_daughter(mcep_parent,mcep_frame);
            item_set_int(mcep_frame,FRAME_NUMBER,num_frames);
            item_set(mcep_frame,"name",item_feat(mcep_parent,"name"));
        }
    }

    /* Copy duration up onto Segment relation */
    for (s = UTT_REL_HEAD(utt,SEGMENT); s; s=item_next(s))
        item_set(s,"end",ffeature(s,"R:"SEGSTATE".dn.end"));

    return num_frames;
}

static int voiced_frame(cst_item *m)
{
    const char *ph_vc;
    const char *ph_name;
    cst_item *ph_item;

    ph_item = path_to_item(m,"R:"MCEP_LINK".P.R:"SEGSTATE".P");
    ph_vc = ffeature_string(ph_item,PH_VC);
    ph_name = ffeature_string(ph_item,"name");

    if (cst_streq(ph_name,"pau"))
        return 0; /* unvoiced */
    else if (cst_streq("+",ph_vc))
        return 1; /* voiced */
    else if (item_feat_float(m,VOICING) > 0.5)
        /* Even though the range is 0-10, I *do* mean 0.5 */
        return 1; /* voiced */
    else
        return 0; /* unvoiced */
}

static float catmull_rom_spline(float p,float p0,float p1,float p2,float p3)
{
    float q;

    q = ( 0.5 *
          ( ( 2.0 * p1 ) +
            ( p * (-p0 + p2) ) +
            ( (p*p) * (((2.0 * p0) - (5.0 * p1)) +
                       ((4.0 * p2) - p3))) +
            ( (p*p*p) * (-p0 +
                         ((3.0 * p1) - (3.0 * p2)) +
                         p3))));
    return q;
}

static void cg_F0_interpolate_spline(cst_utterance *utt,
                                     bell_track *param_track)
{
    float start_f0, mid_f0, end_f0;
    int start_index, end_index, mid_index;
    int nsi, nei, nmi;  /* next syllable indices */
    float nmid_f0, pmid_f0;
    cst_item *syl;
    int i;
    float m;
    cst_item *mcep_link_item;

    mid_f0 = end_f0 = -1.0;

    for (syl=UTT_REL_HEAD(utt,SYLLABLE); syl; syl=item_next(syl))
    {
        mcep_link_item = path_to_item(syl,"R:"SYLSTRUCTURE".d1.R:"SEGSTATE".d1.R:"MCEP_LINK);
        start_index = ffeature_int(mcep_link_item,"d1."FRAME_NUMBER);
        end_index = ffeature_int(mcep_link_item,"dn."FRAME_NUMBER);
        mid_index = (int)((start_index + end_index)/2.0);

        start_f0 = param_track->frames[start_index][0];
        if (end_f0 > 0.0)
            start_f0 = end_f0;  /* not first time through */
        if (mid_f0 < 0.0)
            pmid_f0 = start_f0;  /* first time through */
        else
            pmid_f0 = mid_f0;
        mid_f0 =  param_track->frames[mid_index][0];
        if (item_next(syl)) /* not last syllable */
            end_f0 = (param_track->frames[end_index-1][0]+
                      param_track->frames[end_index][0])/2.0;
        else
            end_f0 = param_track->frames[end_index-1][0];
        nmid_f0=end_f0; /* in case there is no next syl */

        if (item_next(syl))
        {
            mcep_link_item = path_to_item(syl,"n.R:"SYLSTRUCTURE".d1.R:"SEGSTATE".d1.R:"MCEP_LINK);
            nsi = ffeature_int(mcep_link_item,"d1."FRAME_NUMBER);
            nei = ffeature_int(mcep_link_item,"dn."FRAME_NUMBER);
            nmi = (int)((nsi + nei)/2.0);
            nmid_f0 = param_track->frames[nmi][0];
        }
        /* start to mid syl */
        m = 1.0 / (mid_index - start_index);
        for (i=0; ((start_index+i)<mid_index); i++)
            param_track->frames[start_index+i][0] = 
                 catmull_rom_spline(i*m,pmid_f0,start_f0,mid_f0,end_f0);

        /* mid syl to end */
        m = 1.0 / (end_index - mid_index);
        for (i=0; ((mid_index+i)<end_index); i++)
            param_track->frames[mid_index+i][0] =
                catmull_rom_spline(i*m,start_f0,mid_f0,end_f0,nmid_f0);
    }

    return;
}

static void cg_smooth_F0(cst_utterance *utt,cst_cg_db *cg_db,
                         bell_track *param_track)
{
    /* Smooth F0 and mark unvoiced frames as 0.0 */
    cst_item *mcep;
    int i;
    float mean, stddev;

    cg_F0_interpolate_spline(utt,param_track);

    mean = get_param_float(utt->features,"f0_target_mean", cg_db->f0_mean);
    stddev = 
        get_param_float(utt->features,"f0_target_stddev", cg_db->f0_stddev);
    
    for (i=0,mcep=UTT_REL_HEAD(utt,MCEP); mcep; i++,mcep=item_next(mcep))
    {
        if (voiced_frame(mcep))
        {
            /* scale the F0 -- which normally wont change it at all */
            param_track->frames[i][0] = 
                (((param_track->frames[i][0]-cg_db->f0_mean)/cg_db->f0_stddev) 
                 *stddev)+mean;
            /* Some safety checks */
            if (param_track->frames[i][0] < 50)
                param_track->frames[i][0] = 50;
            if (param_track->frames[i][0] > 700)
                param_track->frames[i][0] = 700;
        }
        else /* Unvoice it */
            param_track->frames[i][0] = 0.0;
    }

    return;
}

static cst_utterance *cg_predict_params(cst_utterance *utt,int num_frames, cst_cg_db *cg_db,
                                        bell_track **p_param_track, bell_track **p_str_track)
{
    bell_track *param_track;
    bell_track *str_track;
    cst_item *mcep;
    const cst_cart *mcep_tree, *f0_tree;
    int i,j,f,p,o,pm;
    const char *mname;
    float f0_val;
    float voicing;
    int extra_feats = 0;

    extra_feats = 1;  /* voicing */
    if (cg_db->mixed_excitation)
    {
        extra_feats += 5;
        *p_str_track = new_track(num_frames,5, 0);
    }
    str_track = *p_str_track;

    *p_param_track = new_track(num_frames,
                       ( (cg_db->num_channels[0] - (2 * extra_feats)) /2)-1,
                       cg_db->num_param_models);
    param_track = *p_param_track;

    for (i=0,mcep=UTT_REL_HEAD(utt,MCEP); mcep; i++,mcep=item_next(mcep))
    {
        mname = item_feat_string(mcep,"name");
        for (p=0; cg_db->types[p]; p++)
            if (cst_streq(mname,cg_db->types[p]))
                break;
        if (cg_db->types[p] == NULL)
            p=0; /* if there isn't a matching tree, use the first one */

        /* Predict F0 */
        f0_tree = cg_db->f0_trees[p];
        f0_val = val_float(cart_interpret(mcep,f0_tree));
        param_track->frames[i][0] = f0_val;
        /* what about stddev ? */

        if (cg_db->num_param_models > 1)
        {   /* MULTI model */
            /* Predict spectral coeffs */
            voicing = 0.0;
            for (pm=0; pm<cg_db->num_param_models; pm++)
            {
                mcep_tree = cg_db->param_trees[pm][p];
                f = val_int(cart_interpret(mcep,mcep_tree));

                /* Old code used to average in param[0] with F0 too (???) */

                // Defer mcep copying to mlpg functions,
                // pass indexes to voice data instead
                param_track->idxmcep[i][pm] = f;

//              mixed excitation
                if (cg_db->mixed_excitation)
                {
                    o = (cg_db->num_channels[0])- (2 * extra_feats);
                    for (j=0; j<5; j++)
                    {
                        if (pm == 0) str_track->frames[i][j] = 0.0;
                        str_track->frames[i][j] +=
                            BELL_MODEL_VECTOR(model_vectors[pm],f,
                                            (o+(2*j))) /
                            (float)cg_db->num_param_models;
                    }
                }

                /* last coefficient is average voicing for cluster */
                voicing /= (float)(pm+1);
                voicing +=
                    BELL_MODEL_VECTOR(model_vectors[pm],f,
                                    cg_db->num_channels[pm]-2) /
                    (float)(pm+1);
            }
            item_set_float(mcep,VOICING,voicing);
        }
        else
        {   /* SINGLE model */
            /* Predict Spectral */
            mcep_tree = cg_db->param_trees[0][p];
            f = val_int(cart_interpret(mcep,mcep_tree));

            param_track->frames[i][0] =
                (param_track->frames[i][0]+
                 BELL_MODEL_VECTOR(model_vectors[0],f,0))/2.0;
            // Defer mcep copying to mlpg functions,
            // pass indexes to voice data instead
            param_track->idxmcep[i][0] = f;

            if (cg_db->mixed_excitation)
            {
                o = (cg_db->num_channels[0])- (2 * extra_feats);
                for (j=0; j<5; j++)
                {
                    str_track->frames[i][j] =
                        BELL_MODEL_VECTOR(model_vectors[0],f,(o+(2*j)));
                }
            }
            /* last coefficient is average voicing for cluster */
            item_set_float(mcep,VOICING,
                           BELL_MODEL_VECTOR(model_vectors[0],f,
                                           cg_db->num_channels[0]-2));
        }
    }

    cg_smooth_F0(utt,cg_db,param_track);

    return utt;
}

static void cg_resynth(cst_utterance *utt, cst_cg_db *cg_db,
                       bell_track *param_track, bell_track *str_track)
{
    cst_wave *w;

    cg_mlpg(param_track, cg_db);
    w = mlsa_resynthesis(param_track,str_track,cg_db);

    if (w == NULL)
    {
        /* Synthesis Failed, probably because it was interrupted */
        UTT_SET_FEAT_INT(utt,"Interrupted",1);
        w = new_wave();
    }

    utt_set_wave(utt,w);

    return;
}

cst_utterance *cg_synth(cst_utterance *utt)
{
    int num_frames;
    bell_track *str_track = NULL; // A track will be allocated in cg_predict_params
                                  // if the voice is of mixed excitation type.
    bell_track *param_track = NULL; // A track will be allocated in cg_predict_params
    cst_cg_db *cg_db = utt->vox->cg_db;

    cg_make_hmmstates(utt, cg_db);
    num_frames=cg_make_params(utt, cg_db);
    cg_predict_params(utt,num_frames, cg_db, &param_track, &str_track);
    if (cg_db->spamf0)
    {
	cst_spamf0(utt, num_frames, cg_db, param_track);
    }
    cg_resynth(utt, cg_db, param_track, str_track);

    delete_track(str_track);
    delete_track(param_track);

    return utt;
}
