/*
    bcLog.c
    Copyright (C) 2000-2009 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <windows.h>
#include <stdio.h>
#include "bcsvc.hxx"
#include "bcversion.h"
#include "bcCommon.h"
#include "bcsvc.h"
#include "memory.h"

static void AddToMessageLog(DWORD Event,WORD Count,LPCWSTR *Strings)
{
  HANDLE hEventSource;
  hEventSource=RegisterEventSourceW(NULL,SVC_DISPLAYNAMEW);
  if(hEventSource!=NULL)
  {
    PSID user=NULL; HANDLE token; PTOKEN_USER token_user=NULL;
    if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,FALSE,&token))
    {
      token_user=(PTOKEN_USER)DefaultTokenInformation(token,TokenUser);
      if(token_user)
        user=token_user->User.Sid;
      CloseHandle(token);
    }
    ReportEventW(hEventSource,EVENTLOG_ERROR_TYPE,0,Event,user,Count,0,Strings,NULL);
    DeregisterEventSource(hEventSource);
    free(token_user);
  }
}

void LogEx(wchar_t *Func,wchar_t *parm1,wchar_t *parm2)
{
  DWORD err_type=CheckErrorInfo();
  switch(err_type)
  {
    case 0:
    {
      wchar_t *ErrStrings[4];
      wchar_t *msg;
      DWORD count=4;
      FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ARGUMENT_ARRAY,NULL,GetLastError(),LANG_NEUTRAL,(wchar_t *)&msg,512,NULL);
      ErrStrings[0]=msg;
      ErrStrings[1]=Func;
      ErrStrings[2]=parm1;
      ErrStrings[3]=parm2;
      if(parm2==NULL) count--;
      if(parm1==NULL) count--;

      AddToMessageLog(MSG_ERROR2-4+count,(WORD)count,(LPCWSTR *)ErrStrings);
      LocalFree(msg);
      break;
    }
    case 1:
    {
      wchar_t *ErrStrings[1];
      wchar_t msg[64];
      ErrStrings[0]=msg;
      swprintf(msg,L"More than %ld errors occured.",GetMaxError());
      AddToMessageLog(MSG_ERRORA,1,(LPCWSTR *)ErrStrings);
      break;
    }
  }
}

void LogSys(wchar_t *Func,wchar_t *parm1,wchar_t *parm2)
{
  LogEx(Func,parm1,parm2);
}

#if 0
void LogAuditFail(void)
{
  HANDLE hEventSource;
  hEventSource=RegisterEventSourceW(NULL,SVC_DISPLAYNAMEW);
  if (hEventSource!=NULL)
  {
    ReportEventW(hEventSource,EVENTLOG_WARNING_TYPE,0,MSG_ERRORB,NULL,0,0,NULL,NULL);
    DeregisterEventSource(hEventSource);
  }
}
#endif
