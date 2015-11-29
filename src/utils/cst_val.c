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
/*                        Copyright (c) 1999                             */
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
/*               Date:  December 1999                                    */
/*************************************************************************/
/*                                                                       */
/*  Typed values                                                          */
/*                                                                       */
/*************************************************************************/
#include <math.h>
#include "cst_file.h"
#include "cst_alloc.h"
#include "cst_error.h"
#include "cst_val.h"
#include "cst_string.h"
#include "cst_tokenstream.h"
#include "bell_file.h"

/* Private unsafe accessor macros */
#define CST_VAL_STRING_LVAL(X) ((X)->c.a.v.vval)
#define CST_VAL_TYPE(X) ((X)->c.a.type)
#define CST_VAL_INT(X) ((X)->c.a.v.ival)
#define CST_VAL_FLOAT(X) ((X)->c.a.v.fval)
#define CST_VAL_STRING(X) ((const char *)(CST_VAL_STRING_LVAL(X)))
#define CST_VAL_VOID(X) ((X)->c.a.v.vval)
#define CST_VAL_CAR(X) ((X)->c.cc.car)
#define CST_VAL_CDR(X) ((X)->c.cc.cdr)

#define CST_VAL_REFCOUNT(X) ((X)->c.a.ref_count)

static cst_val *new_val()
{
    return cst_alloc(struct cst_val_struct,1);
}

static int val_dec_refcount(const cst_val *b)
{
    cst_val *wb;

    wb = (cst_val *)(void *)b;

    if (CST_VAL_REFCOUNT(wb) == -1) 
	/* or is a cons cell in the text segment, how do I do that ? */
	return -1;
    else if (cst_val_consp(wb)) /* we don't ref count cons cells */
	return 0;
    else if (CST_VAL_REFCOUNT(wb) == 0)
    {
	/* Otherwise, trying to free a val outside an
           item/relation/etc has rather the opposite effect from what
           you might have intended... */
	return 0;
    }
    else
    {
	CST_VAL_REFCOUNT(wb) -= 1;
	return 	CST_VAL_REFCOUNT(wb);
    }
}

cst_val *int_val(int i)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = CST_VAL_TYPE_INT;
    CST_VAL_INT(v) = i;
    return v;
}
    
cst_val *float_val(float f)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = CST_VAL_TYPE_FLOAT;
    CST_VAL_FLOAT(v) = f;
    return v;
}

cst_val *string_val(const char *s)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = CST_VAL_TYPE_STRING;
    /* would be nice to note if this is a deletable string or not */
    CST_VAL_STRING_LVAL(v) = cst_strdup(s);
    return v;
}

cst_val *cons_val(const cst_val *a, const cst_val *b)
{
    cst_val *v = new_val();
    CST_VAL_CAR(v)=((!a || cst_val_consp(a)) ? 
		    (cst_val *)(void *)a:val_inc_refcount(a));
    CST_VAL_CDR(v)=((!b || cst_val_consp(b)) ? 
		    (cst_val *)(void *)b:val_inc_refcount(b));
    return v;
}

cst_val *val_new_typed(int type,void *vv)
{
    cst_val *v = new_val();
    CST_VAL_TYPE(v) = type;
    CST_VAL_VOID(v) = vv;
    return v;
}

void delete_val(cst_val *v)
{
    if (v)
    {
	if (cst_val_consp(v))
	{
	    delete_val(CST_VAL_CAR(v));
	    delete_val(CST_VAL_CDR(v));
	    cst_free(v);
	}
	else if (val_dec_refcount(v) == 0)
	{
	    if (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING)
		cst_free(CST_VAL_VOID(v));
	    else if (CST_VAL_TYPE(v) >= CST_VAL_TYPE_FIRST_FREE)
            {
                if (cst_val_defs[CST_VAL_TYPE(v)/2].delete_function)
                    (cst_val_defs[CST_VAL_TYPE(v)/2].delete_function)
                        (CST_VAL_VOID(v));
            }
            cst_free(v);
	}
    }
}

/* Accessor functions */
int val_int(const cst_val *v)
{
    if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_INT))
	return CST_VAL_INT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_FLOAT))
	return (int)CST_VAL_FLOAT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING))
	return atoi(CST_VAL_STRING(v));
    else
    {
	cst_errmsg("VAL: tried to access int in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

float val_float(const cst_val *v)
{
    if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_INT))
	return (float)CST_VAL_INT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_FLOAT))
	return CST_VAL_FLOAT(v);
    else if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING))
	return cst_atof(CST_VAL_STRING(v));
    else
    {
	cst_errmsg("VAL: tried to access float in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

const char *val_string(const cst_val *v)
{
    if (v && (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING))
	return CST_VAL_STRING(v);
    else
    {
	cst_errmsg("VAL: tried to access string in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

const cst_val *val_car(const cst_val *v)
{
    if (v && cst_val_consp(v))
	return CST_VAL_CAR(v);
    else
    {
	cst_errmsg("VAL: tried to access car in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

const cst_val *val_cdr(const cst_val *v)
{
    if (v && cst_val_consp(v))
	return CST_VAL_CDR(v);
    else
    {
	cst_errmsg("VAL: tried to access cdr in %d typed val\n",
		   (v ? CST_VAL_TYPE(v) : -1));
	cst_error();
    }
    return 0;
}

void *val_generic(const cst_val *v, int type, const char *stype)
{   /* a generic access function that checks the expected type */
    if (v && CST_VAL_TYPE(v) == type)
	return CST_VAL_VOID(v);
    else
    {
        cst_errmsg("VAL: tried to access %s in %d type val\n",
                       stype,
                       (v ? CST_VAL_TYPE(v) : -1));
        cst_error();
    }
    return NULL;
}

int cst_val_consp(const cst_val *v)
{
    /* To keep a val cell down to 8 bytes we identify non-cons cells  */
    /* with non-zero values in the least significant bit of the first */
    /* address in the cell (this is a standard technique used on Lisp */
    /* machines)                                                      */
    const cst_val_atom *t;

    t = (const cst_val_atom *)v;
    if (t->type % 2 == 0)
      return TRUE;
    else
      return FALSE;
}

const cst_val *set_car(cst_val *v1, const cst_val *v2)
{
    /* destructive set car, be careful you have a pointer to current car */
    
    if (!cst_val_consp(v1))
    {
	cst_errmsg("VAL: tried to set car of non-consp cell\n");
	cst_error();
	return NULL;
    }
    else
    {
	val_dec_refcount(CST_VAL_CAR(v1));
	val_inc_refcount(v1);
	CST_VAL_CAR(v1) = (cst_val *)v2;
    }
    return v1;
}

void val_print(cst_file fd,const cst_val *v)
{
    const cst_val *p;

    if (v == NULL)
	bell_fprintf(fd,"[null]");
    else if (CST_VAL_TYPE(v) == CST_VAL_TYPE_INT)
	bell_fprintf(fd,"%d",val_int(v));
    else if (CST_VAL_TYPE(v) == CST_VAL_TYPE_FLOAT)
	bell_fprintf(fd,"%f",val_float(v));
    else if (CST_VAL_TYPE(v) == CST_VAL_TYPE_STRING)
	bell_fprintf(fd,"%s",val_string(v));
    else if (cst_val_consp(v))
    {
	bell_fprintf(fd,"(");
	for (p=v; p; )
	{
	    val_print(fd,val_car(p));
	    p=val_cdr(p);
	    if (p)
		bell_fprintf(fd," ");
            if (p && !cst_val_consp(p))  /* dotted pairs for non-list */
            {                            
                bell_fprintf(fd,". ");
                val_print(fd,p);
                break;
            }
	}
	bell_fprintf(fd,")");
    }
    else 
	bell_fprintf(fd,"[Val %s 0x%p]",
		cst_val_defs[CST_VAL_TYPE(v)/2].name,CST_VAL_VOID(v));
}

cst_val *val_reverse(cst_val *l)
{   /* destructively reverse the list */
    cst_val *n,*np,*nl;
    for (nl=0,n=l; n; nl=n,n=np)
    {
	np=CST_VAL_CDR(n);
	CST_VAL_CDR(n) = nl;
    }
    return nl;
}

cst_val *val_append(cst_val *l1, cst_val *l2)
{
    /* Destructively add l2 to the end of l1 return l1 */
    cst_val *t;

    if (l1 == 0)
	return l2;
    else
    {
	for (t=l1; val_cdr(t); t=CST_VAL_CDR(t));
	CST_VAL_CDR(t) = l2;
	return l1;
    }
}

int val_length(const cst_val *l)
{
    const cst_val *n;
    int i;

    for (i=0,n=l; n; n=val_cdr(n))
	i++;

    return i;
}

int val_equal(const cst_val *v1, const cst_val *v2)
{
    if (v1 == v2)
	return TRUE;  /* its eq so its equal */
    else if (v1 == 0)
	return FALSE;
    else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE(v2))
    {
	if (cst_val_consp(v1))
	    return ((val_equal(val_car(v1),val_car(v2))) &&
		    (val_equal(val_cdr(v1),val_cdr(v2))));
	else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE_INT)
	    return (val_int(v1) == val_int(v2));
	else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE_FLOAT)
	    return (val_float(v1) == val_float(v2));
	else if (CST_VAL_TYPE(v1) == CST_VAL_TYPE_STRING)
	    return (cst_streq(CST_VAL_STRING(v1),CST_VAL_STRING(v2)));
	else 
	    return CST_VAL_VOID(v1) == CST_VAL_VOID(v2);
    }
    else
	return FALSE;
}

cst_val *val_inc_refcount(const cst_val *b)
{
    cst_val *wb;

    /* Well I was lying, they're not really const, but this is the place */
    /* where breaking const is reasonable                              */
    wb = (cst_val *)(void *)b;

    if (CST_VAL_REFCOUNT(wb) == -1) 
	/* or is a cons cell in the text segment, how do I do that ? */
	return wb;
    else if (!cst_val_consp(wb)) /* we don't ref count cons cells */
	CST_VAL_REFCOUNT(wb) += 1;
    return wb;
}

const cst_val *val_assoc_string(const char *v1,const cst_val *al)
{
    const cst_val *i;

    for (i=al; i; i=val_cdr(i))
    {
	if (cst_streq(v1,val_string(val_car(val_car(i)))))
	    return val_car(i);
    }
    return NULL;
}

cst_val *val_readlist_string(const char *str)
{   /* not fully general but a good start */
    cst_tokenstream *ts;
    cst_val *v = NULL;
    const char *p;

    ts = ts_open_string(str,
                        cst_ts_default_whitespacesymbols,
                        "",
                        "",
                        "");

    while (!ts_eof(ts))
    {
        p = ts_get(ts);
        v = cons_val(string_val(p),v);
    }

    ts_close(ts);

    return val_reverse(v);
}
