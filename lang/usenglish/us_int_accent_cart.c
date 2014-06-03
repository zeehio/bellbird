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
/*  Derived directly from the accent model cart tree in University of    */
/*  Edinburgh's Festival Speech Synthesis Systems                        */
/*    file:  festival/lib/tobi.scm:f2b_int_accent_cart_tree              */
/*  which was in turn was trained from Boston University FM Radio Data   */
/*  Corpus                                                               */
/*                                                                       */
/*************************************************************************/
/*******************************************************/
/**  Autogenerated cart trees for us_int_accent    */
/*******************************************************/

#include "cst_string.h"
#include "cst_cart.h"
#include "cst_regex.h"
#include "bell_ff_sym.h"
#include "bell_relation_sym.h"
#include "us_int_accent_cart.h"

extern const cst_cart us_int_accent_cart;

static const cst_cart_node us_int_accent_cart_nodes[] = {
{ 0, CST_CART_OP_IS, CTNODE_NO_0000, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 1, CST_CART_OP_IS, CTNODE_NO_0002, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 2, CST_CART_OP_IS, CTNODE_NO_0004, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 3, CST_CART_OP_IS, CTNODE_NO_0006, &val_0003},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0008, &val_0004},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0010, &val_0005},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 5, CST_CART_OP_IS, CTNODE_NO_0012, &val_0003},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0014, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0016, &val_0007},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0018, &val_0008},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0020, &val_0009},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 6, CST_CART_OP_IS, CTNODE_NO_0022, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0024, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 4, CST_CART_OP_IS, CTNODE_NO_0026, &val_0011},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0028, &val_0012},
{ 9, CST_CART_OP_IS, CTNODE_NO_0029, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 10, CST_CART_OP_IS, CTNODE_NO_0032, &val_0013},
{ 11, CST_CART_OP_IS, CTNODE_NO_0033, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 12, CST_CART_OP_IS, CTNODE_NO_0035, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 13, CST_CART_OP_IS, CTNODE_NO_0037, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 9, CST_CART_OP_IS, CTNODE_NO_0039, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 14, CST_CART_OP_IS, CTNODE_NO_0041, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0043, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 3, CST_CART_OP_IS, CTNODE_NO_0046, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 5, CST_CART_OP_IS, CTNODE_NO_0048, &val_0013},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 10, CST_CART_OP_IS, CTNODE_NO_0050, &val_0012},
{ 7, CST_CART_OP_IS, CTNODE_NO_0051, &val_0014},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 15, CST_CART_OP_IS, CTNODE_NO_0054, &val_0015},
{ 14, CST_CART_OP_IS, CTNODE_NO_0055, &val_0010},
{ 7, CST_CART_OP_IS, CTNODE_NO_0056, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 12, CST_CART_OP_IS, CTNODE_NO_0058, &val_0015},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0060, &val_0014},
{ 12, CST_CART_OP_IS, CTNODE_NO_0061, &val_0013},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 16, CST_CART_OP_IS, CTNODE_NO_0065, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 13, CST_CART_OP_IS, CTNODE_NO_0068, &val_0010},
{ 9, CST_CART_OP_IS, CTNODE_NO_0069, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0016 },
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 9, CST_CART_OP_IS, CTNODE_NO_0072, &val_0000},
{ 3, CST_CART_OP_IS, CTNODE_NO_0073, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0075, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 14, CST_CART_OP_IS, CTNODE_NO_0077, &val_0010},
{ 10, CST_CART_OP_IS, CTNODE_NO_0078, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 12, CST_CART_OP_IS, CTNODE_NO_0082, &val_0013},
{ 17, CST_CART_OP_IS, CTNODE_NO_0083, &val_0010},
{ 10, CST_CART_OP_IS, CTNODE_NO_0084, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0016 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 12, CST_CART_OP_IS, CTNODE_NO_0088, &val_0017},
{ 7, CST_CART_OP_IS, CTNODE_NO_0089, &val_0014},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 11, CST_CART_OP_IS, CTNODE_NO_0092, &val_0006},
{ 15, CST_CART_OP_IS, CTNODE_NO_0093, &val_0013},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 15, CST_CART_OP_IS, CTNODE_NO_0095, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 10, CST_CART_OP_IS, CTNODE_NO_0097, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0099, &val_0014},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 255, CST_CART_OP_NONE, 0, &val_0018 },
{ 5, CST_CART_OP_IS, CTNODE_NO_0102, &val_0015},
{ 8, CST_CART_OP_IS, CTNODE_NO_0103, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 10, CST_CART_OP_IS, CTNODE_NO_0105, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0016 },
{ 11, CST_CART_OP_IS, CTNODE_NO_0108, &val_0004},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 9, CST_CART_OP_IS, CTNODE_NO_0110, &val_0012},
{ 10, CST_CART_OP_IS, CTNODE_NO_0111, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 15, CST_CART_OP_IS, CTNODE_NO_0114, &val_0019},
{ 14, CST_CART_OP_IS, CTNODE_NO_0115, &val_0010},
{ 13, CST_CART_OP_IS, CTNODE_NO_0116, &val_0014},
{ 255, CST_CART_OP_NONE, 0, &val_0018 },
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 13, CST_CART_OP_IS, CTNODE_NO_0120, &val_0005},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 17, CST_CART_OP_IS, CTNODE_NO_0122, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 3, CST_CART_OP_IS, CTNODE_NO_0124, &val_0013},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 8, CST_CART_OP_IS, CTNODE_NO_0126, &val_0010},
{ 16, CST_CART_OP_IS, CTNODE_NO_0127, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 3, CST_CART_OP_IS, CTNODE_NO_0129, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 10, CST_CART_OP_IS, CTNODE_NO_0131, &val_0010},
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 3, CST_CART_OP_IS, CTNODE_NO_0133, &val_0000},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 7, CST_CART_OP_IS, CTNODE_NO_0136, &val_0006},
{ 255, CST_CART_OP_NONE, 0, &val_0016 },
{ 15, CST_CART_OP_IS, CTNODE_NO_0138, &val_0020},
{ 255, CST_CART_OP_NONE, 0, &val_0018 },
{ 12, CST_CART_OP_IS, CTNODE_NO_0140, &val_0015},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 15, CST_CART_OP_IS, CTNODE_NO_0142, &val_0013},
{ 255, CST_CART_OP_NONE, 0, &val_0018 },
{ 14, CST_CART_OP_IS, CTNODE_NO_0144, &val_0010},
{ 15, CST_CART_OP_IS, CTNODE_NO_0145, &val_0012},
{ 255, CST_CART_OP_NONE, 0, &val_0001 },
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, &val_0002 },
{ 255, CST_CART_OP_NONE, 0, 0}};



static const char * const us_int_accent_feat_table[] = {
"R:"SYLSTRUCTURE".P.R:"TOKEN".P.EMPH",
"n.R:"SYLSTRUCTURE".P.R:"TOKEN".P.EMPH",
"p.R:"SYLSTRUCTURE".P.R:"TOKEN".P.EMPH",
SSYL_IN,
"R:"SYLSTRUCTURE".P."GPOS,
SSYL_OUT,
"stress",
"R:"SYLSTRUCTURE".P.R:"WORD".p."GPOS,
"p."SYL_BREAK,
SYL_BREAK,
"p.p."SYL_BREAK,
"R:"SYLSTRUCTURE".P.R:"WORD".p.p."GPOS,
SYL_OUT,
"R:"SYLSTRUCTURE".P.R:"WORD".n."GPOS,
"n.stress",
SYL_IN,
"n."SYL_BREAK,
"n.n."SYL_BREAK,
NULL };

const cst_cart us_int_accent_cart = {
  us_int_accent_cart_nodes,
  us_int_accent_feat_table
};
