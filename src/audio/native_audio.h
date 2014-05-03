/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2001                             */
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
/*                                                                       */
/*  Native audio support                                                 */
/*                                                                       */
/*************************************************************************/

#ifndef _NATIVE_AUDIO_H__
#define _NATIVE_AUDIO_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define BELL_AUDIO_16BIT 2 // Each 16bit audio element has 2 bytes per channel per frame

typedef struct cst_audiodev_struct {
    int sps;
    int channels;
    void *platform_data;
} cst_audiodev;

#ifdef CST_AUDIO_ALSA

#define AUDIO_OPEN_NATIVE audio_open_alsa
#define AUDIO_CLOSE_NATIVE audio_close_alsa
#define AUDIO_WRITE_NATIVE audio_write_alsa

#else

#define AUDIO_OPEN_NATIVE audio_open_none
#define AUDIO_CLOSE_NATIVE audio_close_none
#define AUDIO_WRITE_NATIVE audio_write_none

#endif

cst_audiodev *AUDIO_OPEN_NATIVE(unsigned int sps, int channels);
int AUDIO_CLOSE_NATIVE(cst_audiodev *ad);
int AUDIO_WRITE_NATIVE(cst_audiodev *ad,void *buff,int num_frames);

#endif
