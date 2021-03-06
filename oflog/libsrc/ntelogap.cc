// Module:  DCMTK_LOG4CPLUS
// File:    nteventlogappender.cxx
// Created: 4/2003
// Author:  Michael CATANZARITI
//
// Copyright 2003-2009 Michael CATANZARITI
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "dcmtk/oflog/ntelogap.h"
#include "dcmtk/oflog/loglevel.h"
#include "dcmtk/oflog/streams.h"
#include "dcmtk/oflog/helpers/loglog.h"
#include "dcmtk/oflog/spi/logevent.h"


#if defined (DCMTK_LOG4CPLUS_HAVE_NT_EVENT_LOG)

using namespace dcmtk::log4cplus;
using namespace dcmtk::log4cplus::spi;
using namespace dcmtk::log4cplus::helpers;


//////////////////////////////////////////////////////////////////////////////
// File LOCAL methods
//////////////////////////////////////////////////////////////////////////////

namespace {

    bool
    FreeSid(SID* pSid)
    {
        return ::HeapFree(GetProcessHeap(), 0, (LPVOID)pSid) != 0;
    }


    bool
    CopySid(SID** ppDstSid, SID* pSrcSid)
    {
        bool bSuccess = false;

        DWORD dwLength = ::GetLengthSid(pSrcSid);
        *ppDstSid = (SID *) ::HeapAlloc(GetProcessHeap(),
        HEAP_ZERO_MEMORY, dwLength);

        if(::CopySid(dwLength, *ppDstSid, pSrcSid)) {
            bSuccess = true;
        }
        else {
            FreeSid(*ppDstSid);
        }

        return bSuccess;
    }



    bool
    GetCurrentUserSID(SID** ppSid)
    {
        bool bSuccess = false;

        // Pseudohandle so don't need to close it
        HANDLE hProcess = ::GetCurrentProcess();
        HANDLE hToken = NULL;
        if(::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
            // Get the required size
            DWORD tusize = 0;
            GetTokenInformation(hToken, TokenUser, NULL, 0, &tusize);
            TOKEN_USER* ptu = (TOKEN_USER*)new BYTE[tusize];

            if(GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, tusize, &tusize)) {
                bSuccess = CopySid(ppSid, (SID *)ptu->User.Sid);
            }

            CloseHandle(hToken);
            delete [] ptu;
        }

        return bSuccess;
    }





    HKEY
    regGetKey(const tstring& subkey, DWORD* disposition)
    {
        HKEY hkey = 0;
        RegCreateKeyEx(HKEY_LOCAL_MACHINE,
                       subkey.c_str(),
                       0,
                       NULL,
                       REG_OPTION_NON_VOLATILE,
                       KEY_SET_VALUE,
                       NULL,
                       &hkey,
                       disposition);
        return hkey;
    }



    void
    regSetString(HKEY hkey, const tstring& name, const tstring& value)
    {
        RegSetValueEx(hkey,
                      name.c_str(),
                      0,
                      REG_SZ,
                      (LPBYTE)value.c_str(),
                      OFstatic_cast(DWORD, value.length() * sizeof(tchar)));
    }



    void
    regSetDword(HKEY hkey, const tstring& name, DWORD value)
    {
        RegSetValueEx(hkey,
                      name.c_str(),
                      0,
                      REG_DWORD,
                      (LPBYTE)&value,
                      sizeof(DWORD));
    }

}



//////////////////////////////////////////////////////////////////////////////
// dcmtk::log4cplus::NTEventLogAppender ctor and dtor
//////////////////////////////////////////////////////////////////////////////

NTEventLogAppender::NTEventLogAppender(const tstring& server,
                                       const tstring& log,
                                       const tstring& source)
: server(server),
  log(log),
  source(source),
  hEventLog(NULL),
  pCurrentUserSID(NULL)
{
    init();
}



NTEventLogAppender::NTEventLogAppender(const Properties properties, tstring& error)
: Appender(properties),
  hEventLog(NULL),
  pCurrentUserSID(NULL)
{
    server = properties.getProperty( DCMTK_LOG4CPLUS_TEXT("server") );
    log = properties.getProperty( DCMTK_LOG4CPLUS_TEXT("log") );
    source = properties.getProperty( DCMTK_LOG4CPLUS_TEXT("source") );

    init();
}



void
NTEventLogAppender::init()
{
    if(source.empty()) {
        getLogLog().warn(  DCMTK_LOG4CPLUS_TEXT("Source option not set for appender [")
                         + name
                         + DCMTK_LOG4CPLUS_TEXT("]."));
        return;
    }

    if(log.length() == 0) {
        log = DCMTK_LOG4CPLUS_TEXT("Application");
    }

    // current user security identifier
    GetCurrentUserSID(&pCurrentUserSID);

    addRegistryInfo();

    hEventLog = ::RegisterEventSource(server.empty () ? 0 : server.c_str(),
        source.c_str());
    if (! hEventLog || hEventLog == HANDLE(ERROR_INVALID_HANDLE))
        getLogLog().warn (DCMTK_LOG4CPLUS_TEXT("Event source registration failed."));
}



NTEventLogAppender::~NTEventLogAppender()
{
    destructorImpl();

    if(pCurrentUserSID != NULL) {
        FreeSid(pCurrentUserSID);
        pCurrentUserSID = NULL;
    }
}



//////////////////////////////////////////////////////////////////////////////
// dcmtk::log4cplus::NTEventLogAppender public methods
//////////////////////////////////////////////////////////////////////////////

void
NTEventLogAppender::close()
{
    if(hEventLog != NULL) {
        ::DeregisterEventSource(hEventLog);
        hEventLog = NULL;
    }
    closed = true;
}



//////////////////////////////////////////////////////////////////////////////
// dcmtk::log4cplus::NTEventLogAppender protected methods
//////////////////////////////////////////////////////////////////////////////

void
NTEventLogAppender::append(const InternalLoggingEvent& event)
{
    BOOL bSuccess;

    if(hEventLog == NULL) {
        getLogLog().warn(DCMTK_LOG4CPLUS_TEXT("NT EventLog not opened."));
        return;
    }

    tostringstream buf;
    layout->formatAndAppend(buf, event);
    OFSTRINGSTREAM_GETSTR(buf, s);

    bSuccess = ::ReportEvent(hEventLog,
                                  getEventType(event),
                                  getEventCategory(event),
                                  0x1000,
                                  pCurrentUserSID,
                                  1,
                                  0,
                                  &s,
                                  NULL);

    OFSTRINGSTREAM_FREESTR(s);

    if(!bSuccess) {
        getLogLog().error(DCMTK_LOG4CPLUS_TEXT("Cannot report event in NT EventLog."));
    }
}




WORD
NTEventLogAppender::getEventType(const InternalLoggingEvent& event)
{
    WORD ret_val;

    switch ((int)event.getLogLevel())
    {
    case FATAL_LOG_LEVEL:
    case ERROR_LOG_LEVEL:
        ret_val = EVENTLOG_ERROR_TYPE;
        break;
    case WARN_LOG_LEVEL:
        ret_val = EVENTLOG_WARNING_TYPE;
        break;
    case INFO_LOG_LEVEL:
    case DEBUG_LOG_LEVEL:
    default:
        ret_val = EVENTLOG_INFORMATION_TYPE;
        break;
    }

    return ret_val;
}



WORD
NTEventLogAppender::getEventCategory(const InternalLoggingEvent& event)
{
    WORD ret_val;

    switch (event.getLogLevel())
    {
    case FATAL_LOG_LEVEL:
        ret_val = 1;
        break;
    case ERROR_LOG_LEVEL:
        ret_val = 2;
        break;
    case WARN_LOG_LEVEL:
        ret_val = 3;
        break;
    case INFO_LOG_LEVEL:
        ret_val = 4;
        break;
    case DEBUG_LOG_LEVEL:
    default:
        ret_val = 5;
        break;
    }

    return ret_val;
}


// Add this source with appropriate configuration keys to the registry.
void
NTEventLogAppender::addRegistryInfo()
{
    DWORD disposition;
    HKEY hkey = 0;
    tstring subkey =   DCMTK_LOG4CPLUS_TEXT("SYSTEM\\CurrentControlSet\\Services\\EventLog\\")
                     + log
                     + DCMTK_LOG4CPLUS_TEXT("\\")
                     + source;

    hkey = regGetKey(subkey, &disposition);
    if(disposition == REG_CREATED_NEW_KEY) {
        regSetString(hkey,
                     DCMTK_LOG4CPLUS_TEXT("EventMessageFile"),
                     DCMTK_LOG4CPLUS_TEXT("NTEventLogAppender.dll"));
        regSetString(hkey,
                     DCMTK_LOG4CPLUS_TEXT("CategoryMessageFile"),
                     DCMTK_LOG4CPLUS_TEXT("NTEventLogAppender.dll"));
        regSetDword(hkey, DCMTK_LOG4CPLUS_TEXT("TypesSupported"), (DWORD)7);
        regSetDword(hkey, DCMTK_LOG4CPLUS_TEXT("CategoryCount"), (DWORD)5);
    }

    RegCloseKey(hkey);
    return;
}


#endif
