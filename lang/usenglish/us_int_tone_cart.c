/*************************************************************************/
/*                                                                       */
/*                   Carnegie Mellon University and                      */
/*                Centre for Speech Technology Research                  */
/*                     University of Edinburgh, UK                       */
/*                       Copyright (c) 1998-2001                         */
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
/*  THE UNIVERSITY OF EDINBURGH, CARNEGIE MELLON UNIVERSITY AND THE      */
/*  CONTRIBUTORS TO THIS WORK DISCLAIM ALL WARRANTIES WITH REGARD TO     */
/*  THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY   */
/*  AND FITNESS, IN NO EVENT SHALL THE UNIVERSITY OF EDINBURGH, CARNEGIE */
/*  MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE FOR ANY SPECIAL,    */
/*  INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER          */
/*  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN  AN ACTION   */
/*  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF     */
/*  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2001                                     */
/*************************************************************************/
/*                                                                       */
/*  Derived directly from the tone model cart tree in University of      */
/*  Edinburgh's Festival Speech Synthesis Systems                        */
/*    file:  festival/lib/tobi.scm:f2b_int_tone_cart_tree                */
/*  which was in turn was trained from Boston University FM Radio Data   */
/*  Corpus                                                               */
/*                                                                       */
/*************************************************************************/
/*******************************************************/
/**  Autogenerated cart trees for us_int_tone    */
/*******************************************************/

#include "cst_string.h"
#include "cst_cart.h"
#include "cst_regex.h"
#include "bell_ff_sym.h"
#include "bell_relation_sym.h"
#include "us_int_tone_cart.h"

extern const cst_cart us_int_tone_cart;

static const cst_cart_node us_int_tone_cart_nodes[] = {
{ 0, CST_CART_OP_IS, CTNODE_NO_0000, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 1, CST_CART_OP_IS, CTNODE_NO_0002, &val_0002},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 2, CST_CART_OP_IS, CTNODE_NO_0004, &val_0004},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 1, CST_CART_OP_IS, CTNODE_NO_0006, &val_0005},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 3, CST_CART_OP_IS, CTNODE_NO_0008, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 1, CST_CART_OP_IS, CTNODE_NO_0010, &val_0007},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0012, &val_0008},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0014, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 1, CST_CART_OP_IS, CTNODE_NO_0016, &val_0009},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 5, CST_CART_OP_IS, CTNODE_NO_0018, &val_0006},
{ 6, CST_CART_OP_IS, CTNODE_NO_0019, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0021, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 6, CST_CART_OP_IS, CTNODE_NO_0023, &val_0007},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 2, CST_CART_OP_IS, CTNODE_NO_0025, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0027, &val_0013},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 6, CST_CART_OP_IS, CTNODE_NO_0029, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0014 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0031, &val_0015},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 9, CST_CART_OP_IS, CTNODE_NO_0033, &val_0016},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0035, &val_0007},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 9, CST_CART_OP_IS, CTNODE_NO_0037, &val_0010},
{ 6, CST_CART_OP_IS, CTNODE_NO_0038, &val_0009},
{ 4, CST_CART_OP_IS, CTNODE_NO_0039, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 255, CST_CART_OP_NONE, 0, &val_0014 },
{ 10, CST_CART_OP_IS, CTNODE_NO_0042, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 255, CST_CART_OP_NONE, 0, &val_0014 },
{ 255, CST_CART_OP_NONE, 0, &val_0011 },
{ 1, CST_CART_OP_IS, CTNODE_NO_0046, &val_0017},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0048, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 1, CST_CART_OP_IS, CTNODE_NO_0050, &val_0018},
{ 11, CST_CART_OP_IS, CTNODE_NO_0051, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 12, CST_CART_OP_IS, CTNODE_NO_0053, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0055, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 13, CST_CART_OP_IS, CTNODE_NO_0057, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 11, CST_CART_OP_IS, CTNODE_NO_0059, &val_0009},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0061, &val_0002},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 13, CST_CART_OP_IS, CTNODE_NO_0063, &val_0008},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 11, CST_CART_OP_IS, CTNODE_NO_0065, &val_0002},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 2, CST_CART_OP_IS, CTNODE_NO_0067, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 11, CST_CART_OP_IS, CTNODE_NO_0069, &val_0019},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 14, CST_CART_OP_IS, CTNODE_NO_0071, &val_0019},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0073, &val_0009},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 14, CST_CART_OP_IS, CTNODE_NO_0075, &val_0009},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0077, &val_0016},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 11, CST_CART_OP_IS, CTNODE_NO_0079, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 9, CST_CART_OP_IS, CTNODE_NO_0081, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 14, CST_CART_OP_IS, CTNODE_NO_0083, &val_0007},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 2, CST_CART_OP_IS, CTNODE_NO_0085, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0087, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 6, CST_CART_OP_IS, CTNODE_NO_0089, &val_0018},
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 5, CST_CART_OP_IS, CTNODE_NO_0091, &val_0008},
{ 255, CST_CART_OP_NONE, 0, &val_0020 },
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 255, CST_CART_OP_NONE, 0, &val_0003 },
{ 255, CST_CART_OP_NONE, 0, 0}};



static const char * const us_int_tone_feat_table[] = {
"lisp_syl_yn_question",
"R:"SYLSTRUCTURE".P."GPOS,
SSYL_IN,
"p."OLD_SYL_BREAK,
"n."OLD_SYL_BREAK,
OLD_SYL_BREAK,
"R:"SYLSTRUCTURE".P.R:"WORD".n."GPOS,
"R:"SYLSTRUCTURE".P.R:"WORD".p."GPOS,
SYL_IN,
SUB_PHRASES,
"n.stress",
"R:"SYLSTRUCTURE".P.R:"WORD".n.n."GPOS,
"p.p."OLD_SYL_BREAK,
"n.n."OLD_SYL_BREAK,
"R:"SYLSTRUCTURE".P.R:"WORD".p.p."GPOS,
NULL };

const cst_cart us_int_tone_cart = {
  us_int_tone_cart_nodes,
  us_int_tone_feat_table
};
