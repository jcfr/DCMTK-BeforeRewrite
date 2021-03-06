/*
 *
 *  Copyright (C) 1994-2011, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were partly developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  For further copyrights, see the following paragraphs.
 *
 */

/*
**  Copyright (C) 1993/1994, OFFIS, Oldenburg University and CERIUM
**
**  This software and supporting documentation were
**  developed by
**
**    Institut OFFIS
**    Bereich Kommunikationssysteme
**    Westerstr. 10-12
**    26121 Oldenburg, Germany
**
**    Fachbereich Informatik
**    Abteilung Prozessinformatik
**    Carl von Ossietzky Universitaet Oldenburg
**    Ammerlaender Heerstr. 114-118
**    26111 Oldenburg, Germany
**
**    CERIUM
**    Laboratoire SIM
**    Faculte de Medecine
**    2 Avenue du Pr. Leon Bernard
**    35043 Rennes Cedex, France
**
**  for CEN/TC251/WG4 as a contribution to the Radiological
**  Society of North America (RSNA) 1993 Digital Imaging and
**  Communications in Medicine (DICOM) Demonstration.
**
**  THIS SOFTWARE IS MADE AVAILABLE, AS IS, AND NEITHER OFFIS,
**  OLDENBURG UNIVERSITY NOR CERIUM MAKE ANY WARRANTY REGARDING
**  THE SOFTWARE, ITS PERFORMANCE, ITS MERCHANTABILITY OR
**  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER
**  DISEASES OR ITS CONFORMITY TO ANY SPECIFICATION.  THE
**  ENTIRE RISK AS TO QUALITY AND PERFORMANCE OF THE SOFTWARE
**  IS WITH THE USER.
**
**  Copyright of the software and supporting documentation
**  is, unless otherwise stated, jointly owned by OFFIS,
**  Oldenburg University and CERIUM and free access is hereby
**  granted as a license to use this software, copy this
**  software and prepare derivative works based upon this
**  software. However, any distribution of this software
**  source code or supporting documentation or derivative
**  works (source code and supporting documentation) must
**  include the three paragraphs of this copyright notice.
**
*/

/*
**
** Author: Andrew Hewett                Created: 11-08-93
**
** Module: diutil
**
** Purpose:
**     This file contains the interface to
**     some general useful dicom utility routines
**
** Module Prefix: DU_
**
** Last Update:         $Author: uli $
** Update Date:         $Date: 2011-12-14 11:45:14 $
** CVS/RCS Revision:    $Revision: 1.17 $
** Status:              $State: Exp $
**
** CVS/RCS Log at end of file
*/

#ifndef DIUTIL_H
#define DIUTIL_H

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmnet/dicom.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/oflog/oflog.h"


extern DCMTK_DCMNET_EXPORT OFLogger DCM_dcmnetLogger;

#define DCMNET_LOGGER_NAME "dcmtk.dcmnet"

#define DCMNET_TRACE(msg) OFLOG_TRACE(DCM_dcmnetLogger, msg)
#define DCMNET_DEBUG(msg) OFLOG_DEBUG(DCM_dcmnetLogger, msg)
#define DCMNET_INFO(msg)  OFLOG_INFO(DCM_dcmnetLogger, msg)
#define DCMNET_WARN(msg)  OFLOG_WARN(DCM_dcmnetLogger, msg)
#define DCMNET_ERROR(msg) OFLOG_ERROR(DCM_dcmnetLogger, msg)
#define DCMNET_FATAL(msg) OFLOG_FATAL(DCM_dcmnetLogger, msg)


DCMTK_DCMNET_EXPORT void DU_stripTrailingSpaces(char *s);
DCMTK_DCMNET_EXPORT void DU_stripLeadingSpaces(char *s);
DCMTK_DCMNET_EXPORT void DU_stripLeadingAndTrailingSpaces(char *s);

DCMTK_DCMNET_EXPORT OFBool DU_getStringDOElement(DcmItem *obj, DcmTagKey t, char *s);
DCMTK_DCMNET_EXPORT OFBool DU_putStringDOElement(DcmItem *obj, DcmTagKey t, const char *s);
DCMTK_DCMNET_EXPORT OFBool DU_getShortDOElement(DcmItem *obj, DcmTagKey t, Uint16 *us);
DCMTK_DCMNET_EXPORT OFBool DU_putShortDOElement(DcmItem *obj, DcmTagKey t, Uint16 us);

DCMTK_DCMNET_EXPORT OFBool DU_findSOPClassAndInstanceInDataSet(
  DcmItem *obj,
  char* sopClass,
  char* sopInstance,
  OFBool tolerateSpacePaddedUIDs = OFFalse);

DCMTK_DCMNET_EXPORT OFBool DU_findSOPClassAndInstanceInFile(
  const char *fname,
  char* sopClass,
  char* sopInstance,
  OFBool tolerateSpacePaddedUIDs = OFFalse);

DCMTK_DCMNET_EXPORT const char *DU_cechoStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_cstoreStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_cfindStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_cmoveStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_cgetStatusString(Uint16 statusCode);

DCMTK_DCMNET_EXPORT const char *DU_ncreateStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_ngetStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_nsetStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_nactionStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_ndeleteStatusString(Uint16 statusCode);
DCMTK_DCMNET_EXPORT const char *DU_neventReportStatusString(Uint16 statusCode);

#endif

/*
** CVS Log
** $Log: diutil.h,v $
** Revision 1.17  2011-12-14 11:45:14  uli
** Make it possible to perfectly build dcmnet and dcmtls a DLLs.
**
** Revision 1.16  2011-09-28 14:07:26  joergr
** Added function that converts the status of a C-ECHO response to a string.
**
** Revision 1.15  2011-08-19 07:40:30  joergr
** Output C-FIND response messages to a separate logger (on INFO level).
**
** Revision 1.14  2011-05-03 07:46:38  uli
** Remove a pointless return value from some function. This helps in static code
** analysis to ensure memory is never lost.
**
** Revision 1.13  2011-04-18 07:00:59  uli
** Use global variables for the logger objects. This removes the thread-unsafe
** static local variables which were used before.
**
** Revision 1.12  2010-12-01 08:26:10  joergr
** Added OFFIS copyright header (beginning with the year 1994).
**
** Revision 1.11  2010-10-14 13:17:22  joergr
** Updated copyright header. Added reference to COPYRIGHT file.
**
** Revision 1.10  2010-05-21 11:47:52  uli
** Replaced DU_fileSize() with OFStandard::getFileSize().
**
** Revision 1.9  2009-11-18 11:53:58  uli
** Switched to logging mechanism provided by the "new" oflog module.
**
** Revision 1.8  2007-07-12 12:18:00  onken
** Added status codes and corresponding printing routines for DIMSE-N.
**
** Revision 1.7  2005/12/08 16:02:22  meichel
** Changed include path schema for all DCMTK header files
**
** Revision 1.6  2002/11/25 18:00:37  meichel
** Converted compile time option to leniently handle space padded UIDs
**   in the Storage Service Class into command line / config file option.
**
** Revision 1.5  2000/02/03 11:50:11  meichel
** Moved UID related functions from dcmnet (diutil.h) to dcmdata (dcuid.h)
**   where they belong. Renamed access functions to dcmSOPClassUIDToModality
**   and dcmGuessModalityBytes.
**
** Revision 1.4  1999/06/10 10:56:35  meichel
** Adapted imagectn to new command line option scheme.
**   Added support for Patient/Study Only Q/R model and C-GET (experimental).
**
** Revision 1.3  1998/01/14 14:37:06  hewett
** Added basic support for the Structured Reporting (SR) SOP Classes.
**
** Revision 1.2  1997/07/21 08:40:11  andreas
** - Replace all boolean types (BOOLEAN, CTNBOOLEAN, DICOM_BOOL, BOOL)
**   with one unique boolean type OFBool.
**
** Revision 1.1.1.1  1996/03/26 18:38:45  hewett
** Initial Release.
**
**
*/
