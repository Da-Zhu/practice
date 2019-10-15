/*
 * standalone.c - minimal bootstrap for C library
 * Copyright (C) 2000 ARM Limited.
 * All rights reserved.
 */

/*
 * RCS $Revision: 1 $
 * Checkin $Date: 07/07/05 2:54p $ 0
 * Revising $Author: Sjlu0 $
 */

/*
 * This code defines a run-time environment for the C library.
 * Without this, the C startup code will attempt to use semi-hosting
 * calls to get environment information.
 */
 
#include "type_define.h"
#include "usart.h"



#pragma import(__use_no_semihosting_swi)  
#pragma import(_main_redirection)  
  
  
const char __stdin_name[150];  
const char __stdout_name[150];  
const char __stderr_name[150];  
typedef int FILEHANDLE;  
  
 
FILEHANDLE _sys_open(const char *name, int openmode)  
{  
    return 0;  
}  
 
int _sys_close(FILEHANDLE fh)  
{  
    return 0;  
}  

int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)  
{  
    return 0;  
}  
 
int _sys_read(FILEHANDLE fh, unsigned char*buf, unsigned len, int mode)  
{  
    return 0;  
}  
  
int _sys_istty(FILEHANDLE fh)  
{  
    return 0;  
}  
  
int _sys_seek(FILEHANDLE fh, long pos)  
{  
    return 0;  
}  
  
int _sys_ensure(FILEHANDLE fh)  
{  
    return 0;  
}  

long _sys_flen(FILEHANDLE fh)  
{  
    return 0;  
}  
  
int _sys_tmpnam(char *name, int fileno, unsigned maxlength)  
{  
    return 0;  
}  
  

int remove(const char *filename)  
{  
    return 0;  
}  
  
char *_sys_command_string(char *cmd, int len)  
{  
    return 0;  
}  
  
 

void _sys_exit(int return_code)
{
label:  goto label; /* endless loop */
}

void _ttywrch(int ch)
{
    char tempch = (char)ch;
    (void)tempch;
}



int fputc(int ch, FILE *f)
{
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
		return (ch);
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(DEBUG_USART);
}

/* end of file standalone.c */
