/*
 *
 *  Copyright (C) 1998-2011, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module: dcmsign
 *
 *  Author: Norbert Loxen, Marco Eichelberg
 *
 *  Purpose:
 *    classes: SiMAC
 *
 *  Last Update:      $Author: uli $
 *  Update Date:      $Date: 2011-12-14 12:14:12 $
 *  CVS/RCS Revision: $Revision: 1.7 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#ifndef SIMAC_H
#define SIMAC_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmsign/sitypes.h"

#ifdef WITH_OPENSSL

/**
 * a base class for all classes that implement hash functions.
 */
class DCMTK_DCMSIGN_EXPORT SiMAC
{
public:

  /// default constructor
  SiMAC() { }

  /// destructor
  virtual ~SiMAC() { }

  /** initializes the MAC algorithm.
   *  @return status code
   */
  virtual OFCondition initialize() = 0;

  /** feeds data into the MAC algorithm
   *  @param data pointer to raw data to be fed into the MAC, must not be NULL
   *  @param length number of bytes in raw data array
   *  @return status code
   */
  virtual OFCondition digest(const unsigned char *data, unsigned long length) = 0;

  /** finalizes the MAC and writes it to the given output array,
   *  which must be at least getSize() bytes large.
   *  After a call to finalize, the MAC algorithm must be initialized
   *  again, see initialize().
   *  @param result pointer to array of getSize() bytes into which the MAC is written
   *  @return status code
   */
  virtual OFCondition finalize(unsigned char *result) = 0;

  /** returns the size of a MAC in bytes.
   *  @return block size for this MAC algorithm
   */
  virtual unsigned long getSize() const = 0;

  /** returns the type of MAC algorithm computed by this object
   *  @return type of MAC algorithm
   */
  virtual E_MACType macType() const = 0;

  /** returns the DICOM identifier for this MAC algorithm
   *  @return DICOM defined term for algorithm
   */
  virtual const char *getDefinedTerm() const = 0;

};

#endif
#endif

/*
 *  $Log: simac.h,v $
 *  Revision 1.7  2011-12-14 12:14:12  uli
 *  Make it possible to precisely build dcmsign and dcmwlm as DLLs.
 *
 *  Revision 1.6  2010-10-14 13:17:25  joergr
 *  Updated copyright header. Added reference to COPYRIGHT file.
 *
 *  Revision 1.5  2005-12-08 16:04:37  meichel
 *  Changed include path schema for all DCMTK header files
 *
 *  Revision 1.4  2003/06/04 14:21:03  meichel
 *  Simplified include structure to avoid preprocessor limitation
 *    (max 32 #if levels) on MSVC5 with STL.
 *
 *  Revision 1.3  2001/09/26 14:30:20  meichel
 *  Adapted dcmsign to class OFCondition
 *
 *  Revision 1.2  2001/06/01 15:50:49  meichel
 *  Updated copyright header
 *
 *  Revision 1.1  2000/11/07 16:48:55  meichel
 *  Initial release of dcmsign module for DICOM Digital Signatures
 *
 *
 */
