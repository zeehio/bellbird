###########################################################################
##              This Makefile has been modified for bellbird.            ##
##              Please see COPYING file for copyright notice             ##
##              of the modified file. The original copyright             ##
##              notice for the unmodified file is included               ##
##              below.                                                   ##
###########################################################################
###########################################################################
##                                                                       ##
##                  Language Technologies Institute                      ##
##                     Carnegie Mellon University                        ##
##                      Copyright (c) 1999-2011                          ##
##                        All Rights Reserved.                           ##
##                                                                       ##
##  Permission is hereby granted, free of charge, to use and distribute  ##
##  this software and its documentation without restriction, including   ##
##  without limitation the rights to use, copy, modify, merge, publish,  ##
##  distribute, sublicense, and/or sell copies of this work, and to      ##
##  permit persons to whom this work is furnished to do so, subject to   ##
##  the following conditions:                                            ##
##   1. The code must retain the above copyright notice, this list of    ##
##      conditions and the following disclaimer.                         ##
##   2. Any modifications must be clearly marked as such.                ##
##   3. Original authors' names are not deleted.                         ##
##   4. The authors' names are not used to endorse or promote products   ##
##      derived from this software without specific prior written        ##
##      permission.                                                      ##
##                                                                       ##
##  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         ##
##  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      ##
##  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   ##
##  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      ##
##  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    ##
##  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   ##
##  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          ##
##  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       ##
##  THIS SOFTWARE.                                                       ##
##                                                                       ##
###########################################################################
##                                                                       ##
##    Fast efficient small run-time speech synthesis system              ##
##    http://cmuflite.org                                                ##
##                                                                       ##
##       Authors:  Alan W Black (awb@cs.cmu.edu)                         ##
##                 Kevin A. Lenzo (lenzo@cs.cmu.edu)                     ##
##                 and others see ACKNOWLEDGEMENTS                       ##
##          Date:  Jan 2011                                              ##
##       Version:  1.5 release                                           ##
##                                                                       ## 
###########################################################################
TOP=.
DIRNAME=
BUILD_DIRS = include src lang
ALL_DIRS=config $(BUILD_DIRS) main 
FILES = Makefile README ACKNOWLEDGEMENTS COPYING $(CONFIG)

HOST_ONLY_DIRS = main
ALL = $(BUILD_DIRS)

include $(TOP)/config/common_make_rules
