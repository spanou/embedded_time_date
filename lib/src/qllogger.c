/*=============================================================================
 * Copyright (c) 2019, Sakis Panou <sakis.panou@gmail.com>
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *===========================================================================*/
#include <stdio.h>
#include <stdarg.h>

#include "qllogger.h"

#ifndef LOG_LEVEL
    #define LOG_LEVEL OFF
#endif/*_LOG_LEVEL_*/

#ifndef LOG_MODE
    #define LOG_MODE LEVEL
#endif

#if(LOG_LEVEL > OFF)
    static LogLevel globalLogLevel = LOG_LEVEL;
    static LogMode globalLogMode = LOG_MODE;
#endif/*(LOG_LEVEL > OFF)*/

void qlLogger(LogLevel l, const char* const fmt, ...){
#if(LOG_LEVEL > OFF)
    if(fmt==NULL){
        return;
    }

    if(globalLogMode==LEVEL){
        if(l < globalLogLevel)
            return;
    } else {
        if((globalLogLevel & l)==0)
            return;
    }

    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
#endif/*(LOG_LEVEL > OFF)*/
}

