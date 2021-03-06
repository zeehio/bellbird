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
/*                         Copyright (c) 2008                            */
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
/*               Date:  May 2008                                         */
/*************************************************************************/
/*                                                                       */
/*  To avoid an initialization phase where regexes need to be set up we  */
/*  precompile them into statics that can be used directly --            */
/*  (dhd championed this technique, and this is a reimplementation of    */
/*  his compilation technique this)                                      */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cst_regex.h"

void *wrapped_calloc(int size)
{
    void *p = NULL;
    p = (void *)calloc(size,1);

    if (NULL == p)
    {
        printf("wrapped_calloc:can't calloc %d bytes\n", size);
        exit(1);
    }

    return p;
}

/* These define the different escape conventions for the FSF's */
/* regexp code and Henry Spencer's */

static const char *fsf_magic="^$*+?[].\\";
static const char *fsf_magic_backslashed="()|<>";
static const char *spencer_magic="^$*+?[].()|\\\n";
static const char *spencer_magic_backslashed="<>";

/* Adaptation of rjc's mapping of fsf format to henry spencer's format */
/* of escape sequences, as taken from EST_Regex.cc in EST              */
static char *regularize(const char *unregex,int match)
{
    char *reg = (char *)wrapped_calloc(strlen(unregex)*2+3);
    char *r=reg;
    const char *e;
    int magic=0,last_was_bs=0;
    const char * in_brackets=NULL;
    const char *ex = (unregex?unregex:"");

    if (match && *ex != '^')
	*(r++) = '^';

    for (e=ex; *e ; e++)
    {
	if (*e == '\\' && !last_was_bs)
	{
	    last_was_bs=1;
	    continue;
	}

	magic=strchr((last_was_bs?fsf_magic_backslashed:fsf_magic), *e)!=NULL;

	if (in_brackets)
	{
	    *(r++) = *e;
	    if (*e  == ']' && (e-in_brackets)>1)
		in_brackets=0;
	}
	else if (magic)
	{
	    if (strchr(spencer_magic_backslashed, *e))
		*(r++) = '\\';

	    *(r++) = *e;
	    if (*e  == '[')
		in_brackets=e;
	}
	else
	{
	    if (strchr(spencer_magic, *e))
		*(r++) = '\\';

	    *(r++) = *e;
	}
	last_was_bs=0;
    }

    if (match && (e==ex || *(e-1) != '$'))
    {
	if (last_was_bs)
	    *(r++) = '\\';
	*(r++) = '$';
    }

    *r='\0';

    return reg;
}

static cst_regex *new_cst_regex(const char *str)
{
    cst_regex *r;
    char *reg_str = regularize(str,1);

    r = hs_regcomp(reg_str);
    free(reg_str);

    return r;
}

static void delete_cst_regex(cst_regex *r)
{
    if (r)
	hs_regdelete(r);

    return;
}

static void regex_to_C(const char *name, const cst_regex *rgx)
{
    int i;

    printf("static const unsigned char %s_rxprog[] = {\n   ",name);
    for (i=0; i<rgx->regsize; i++)
    {
        printf("%d, ", (unsigned char)rgx->program[i]);
        if (i%16 == 15)
            printf("\n   ");
    }
    printf("\n};\n");
    printf("static const cst_regex %s_rx = {\n   ",name);
    printf("%d, ",rgx->regstart);
    printf("%d, ",rgx->reganch);
    if (rgx->regmust == NULL)
    {
        printf("NULL, ");
    }
    else
    {
#ifndef BELL_WINDOWS
        printf("%s_rxprog + %td, ", name, rgx->regmust - rgx->program);
#else
        printf("%s_rxprog + %Id, ", name, rgx->regmust - rgx->program);
#endif
    }

    printf("%d, ",rgx->regmlen);
    printf("%d,\n   ",rgx->regsize);
    printf("(char *)%s_rxprog\n",name);
    printf("};\n");

    printf("const cst_regex * const %s = &%s_rx;\n\n",name, name);
}

int main(int argc, char **argv)
{
    cst_regex *rgx;

    if (argc != 3)
    {
        printf("Usage: compile_regexes name regex\n"
               "  Compiles regex into a C structure called name\n");
        exit(0);
    }

    rgx = new_cst_regex(argv[2]);
    regex_to_C(argv[1],rgx);
    delete_cst_regex(rgx);
    
    return 0;
}
