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
/*   tree.c : decision trees handling functions                      */
/*                                                                   */ 
/*                                   2003/06/11 by Heiga Zen         */
/*  ---------------------------------------------------------------  */

#include <string.h>
#include <ctype.h>
#include "cst_alloc.h"
#include "cst_error.h"
#include "cst_string.h"

#include "nitech_hidden.h"

#include "HTS_misc.h"

static bell_boolean DPMatch (char *str, char *pat, int pos, int max)
{
   if (pos > max) return 0;
   if (*str == '\0' && *pat == '\0') return 1;
   
   if (*pat == '*')
   {
      if ( DPMatch(str+1, pat, pos+1, max)==1 )
         return 1;
      else
         return DPMatch(str+1, pat+1, pos+1, max);
   }
   if (*str == *pat || *pat == '?')
   {
      if ( DPMatch(str+1, pat+1, pos+1, max+1)==1 )
         return 1;
      else 
         if (*(pat + 1) == '*')
            return DPMatch(str+1, pat+2, pos+1, max+1);
   }
   
   return 0;
}

static bell_boolean PMatch (char *str, char *pat)
{
   int i, max = 0;
   for(i=0; i < (int)cst_strlen(pat); i++)
      if (pat[i] != '*') max++;
         
   return DPMatch(str, pat, 0, cst_strlen(str)-max);
}

static bell_boolean QMatch (char *str, Question *q)
{
   bell_boolean flag = 0;
   Pattern *p;
  
   for (p=q->phead; p!=q->ptail; p=p->next)
   {
      flag = PMatch(str, p->pat);
      if (flag)
         return 1;
   }
   
   return 0;
}

int SearchTree (char *str, Node *node)
{
   bell_boolean answer = QMatch(str, node->quest);

   if (answer) {
      if (node->yes->pdf>0) 
         return node->yes->pdf;
      else 
         return SearchTree(str, node->yes);
   }
   else
   {
      if (node->no->pdf>0) 
         return node->no->pdf;
      else
        return SearchTree (str, node->no);
   }
   
   return -1;
}

static void LoadQuestions(HTS_File *fp, Question *q)
{
   char buf[1024];

   HTS_get_pattern_token(fp, buf);
   q->qName = cst_strdup(buf);
   q->phead = q->ptail = cst_alloc(Pattern,1);

   HTS_get_pattern_token(fp, buf);
   if ( cst_streq(buf,"{") )
   {
      while ( !cst_streq(buf,"}") )
      {
          HTS_get_pattern_token(fp, buf);
          q->ptail->pat = cst_strdup(buf);
          q->ptail->next = cst_alloc(Pattern,1);
          q->ptail = q->ptail->next;
          HTS_get_pattern_token(fp, buf);
      }
   }
}

static bell_boolean IsTree (Tree *tree, char *buf)
{
   char *s,*l,*r;

   s = buf;
   if ( ((l = strchr(s, '[')) == NULL) || ((r = strrchr(s, ']'))==NULL) )
   {
      return 0;
   }
   else
   {
      *r = '\0';
      s = l+1;
      tree->state = atoi(s);
   }
   
   return 1;
}

static bell_boolean IsNum (char *buf)
{
   int i;

   for (i=0; i<(int)cst_strlen(buf); i++)
      if (! (isdigit(buf[i]) || (buf[i] == '-'))) 
         return 0;
      
   return 1;
}

static Question *FindQuestion(TreeSet *ts, Mtype type, char *buf)
{
   Question *q;
   
   for (q=ts->qhead[type];q!=ts->qtail[type];q=q->next)
      if ( cst_streq(buf,q->qName) )
         return q;
      
   printf(" Error ! Cannot find question %s ! \n",buf);
   cst_error();

   return 0;
}

static int name2num(char *buf)
{
   return (atoi(strrchr(buf,'_')+1));
}

static Node *FindNode (Node *node, int num)
{
   Node *dest;
   
   if (node->idx==num) return node;
   else
   {
      if (node->yes != NULL)
      {
         dest = FindNode(node->yes, num);
         if (dest) return dest;
      }
      if (node->no != NULL)
      {
         dest = FindNode(node->no, num);
         if (dest) return dest;
      }
   }
   return NULL;
}
         
static void LoadTree (TreeSet *ts, HTS_File *fp, Tree *tree, Mtype type)
{
   char buf[1024];
   Node *node;
   
   HTS_get_pattern_token(fp, buf);
   node = cst_alloc(Node,1);
   tree->root = node;
   
   if ( cst_streq(buf,"{") )
   {
      while ( HTS_get_pattern_token(fp, buf) , !cst_streq(buf,"}") )
      {
         node = FindNode(tree->root, atoi(buf));
         HTS_get_pattern_token(fp, buf);     /* load a question applied at this node */
         
         node->quest = FindQuestion(ts, type, buf);
         node->yes = cst_alloc(Node,1);
         node->no  = cst_alloc(Node,1);

         HTS_get_pattern_token(fp, buf);
         if (IsNum(buf))
         {
            node->no->idx = atoi(buf);
         }
         else
         {
            node->no->pdf = name2num(buf);
         }
         
         HTS_get_pattern_token(fp, buf);
         if (IsNum(buf))
         {
            node->yes->idx = atoi(buf);
         }
         else
         {
            node->yes->pdf = name2num(buf);
         }
      }
   }
   else 
   {
      node->pdf = name2num(buf);
   }
}
   
void LoadTreesFile(TreeSet *ts, Mtype type)
{
   char buf[1024];
   Question *q;
   Tree *t;

   HTS_File *hfp = HTS_fopen_from_cst_file(ts->fp[type]);;
  
   q = cst_alloc(Question,1);
   ts->qhead[type] = q;  ts->qtail[type] = NULL;

   t = cst_alloc(Tree,1);
   ts->thead[type] = t;  ts->ttail[type] = NULL;
   
   while (!HTS_feof(hfp))
   {
      HTS_get_pattern_token(hfp, buf);
      if ( cst_streq(buf,"QS") )
      {
         LoadQuestions(hfp, q);
         q->next = cst_alloc(Question,1);
         q = ts->qtail[type] = q->next;
         q->next = NULL;
      }
      if (IsTree(t, buf))
      {
         LoadTree(ts, hfp, t, type);
         t->next = cst_alloc(Tree,1);
         t = ts->ttail[type] = t->next;
         t->next = NULL;
      }
   }
   cst_free(hfp);
}

void InitTreeSet(TreeSet *ts) 
{
   ts->fp[DUR] = NULL;
   ts->fp[LF0] = NULL;
   ts->fp[MCP] = NULL;
   
   return; 
} 

static void delete_tree_nodes(Node *node)
{
    if (!node)
	return;
    if (node->yes)
	delete_tree_nodes(node->yes);
    if (node->no)
	delete_tree_nodes(node->no);
    cst_free(node);
}

void FreeTrees(TreeSet *ts, Mtype type)
{
    Question *nq, *qq;
    Pattern *pp, *np;
    Tree *tt, *nt;

    for (qq = ts->qhead[type]; qq; qq = nq)
    {
	nq = qq->next;

	cst_free(qq->qName);
	for (pp = qq->phead; pp; pp = np)
	{
	    np = pp->next;
	    cst_free(pp->pat);
	    cst_free(pp);
	}
	cst_free(qq);
    }

    for (tt = ts->thead[type]; tt; tt = nt)
    {
	nt = tt->next;
	delete_tree_nodes(tt->root);
	cst_free(tt);
    }
}
