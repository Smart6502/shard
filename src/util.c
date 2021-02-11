/*
BSD 2-Clause License

Copyright (c) 2021, Smart6502
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
FILE *fp ;
static int SESSION_TRACKER;

void log_print(char* filename, int line, char *fmt, ...)
{
    va_list         list;
    char            *p, *r;
    int             e;

    if(SESSION_TRACKER > 0)
      fp = fopen (".shard.log","a+");
    else
      fp = fopen (".shard.log","w");
    
    fprintf(fp,"\033[1;32m[ Shard ] ");
    fprintf(fp,"[%s -> %d] \033[0m",filename,line);
    va_start( list, fmt );

    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )
        {
            fputc( *p,fp );
        }
        else
        {
            switch ( *++p )
            {
            case 's':
            {
                r = va_arg( list, char * );

                fprintf(fp,"%s", r);
                continue;
            }
            case 'd':
            {
                e = va_arg( list, int );

                fprintf(fp,"%d", e);
                continue;
            }
            default:
                fputc( *p, fp );
            }
        }
    }
    va_end( list );
    fputc( '\n', fp );
    SESSION_TRACKER++;
    fclose(fp);
}

