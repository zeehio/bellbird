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
/*                         Copyright (c) 2000                            */
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
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2001                                     */
/*************************************************************************/
/*  Generic models for US English                                        */
/*************************************************************************/

#include "flite.h"
#include "usenglish.h"
#include "us_text.h"
#include "us_ffeatures.h"
#include "us_ffeatures_hts.h"
#include "cst_string.h"
#include "../lang/cmulex/cmu_lex.h"
#include "cst_cg.h"

static const char * const us_english_punctuation = "\"'`.,:;!?(){}[]";
static const char * const us_english_prepunctuation = "\"'`({[";
static const char * const us_english_singlecharsymbols = "";
static const char * const us_english_whitespace = " \t\n\r";

void usenglish_init(bell_voice *v)
{
    /* utterance break function */
    v->utt_break = &default_utt_break;

    /* Phoneset */
    v->phoneset = &us_phoneset;

    /* Text analyser */
    v->text_whitespace = us_english_whitespace;
    v->text_postpunctuation = us_english_punctuation;
    v->text_prepunctuation = us_english_prepunctuation;
    v->text_singlecharsymbols = us_english_singlecharsymbols;

    v->tokentowords = &us_tokentowords;

    /* very simple POS tagger */
    v->pos_tagger_cart = (&us_pos_cart);

    /* Phrasing */
    v->phrasing_cart = &us_phrasing_cart;

    /* Intonation */
    v->int_cart_accents = &us_int_accent_cart;
    v->int_cart_tones = &us_int_tone_cart;

    v->synth_methods = cst_alloc(cst_uttfunc, 10);
    v->synth_methods[0] = &default_tokenization;
    v->synth_methods[1] = &default_textanalysis;
    v->synth_methods[2] = &default_pos_tagger;
//  synth_methods[3] is phrasing function and is set below
    v->synth_methods[4] = &default_lexical_insertion;
    v->synth_methods[5] = &default_pause_insertion;
    v->synth_methods[6] = &cart_intonation;
    v->synth_methods[7] = &cmu_postlex;
//  synth_methods[8] is wave synthesis function for cg voices and is set below
    v->synth_methods[9] = NULL;

    if (v->name && cst_streq(v->name,"hts"))
    {
      v->synth_methods[3] = &hts_phrasing;
      v->synth_methods[8] = NULL;

      us_ff_register_hts(v->ffunctions);
    }
    else
    {
      v->synth_methods[3] = &default_phrasing;
      v->synth_methods[8] = &cg_synth;

      us_ff_register(v->ffunctions);
    }
}
