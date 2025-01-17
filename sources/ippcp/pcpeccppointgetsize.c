/*******************************************************************************
* Copyright 2003 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* 
// 
//  Purpose:
//     Cryptography Primitive.
//     EC (prime) Point
// 
//  Contents:
//        ippsECCPPointGetSize()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcpeccp.h"

/*F*
//    Name: ippsECCPPointGetSize
//
// Purpose: Returns size of EC Point context (bytes).
//
// Returns:                Reason:
//    ippStsNullPtrErr        NULL == pSzie
//    ippStsSizeErr           2>feBitSize
//    ippStsNoErr             no errors
//
// Parameters:
//    feBitSize   size of field element (bits)
//    pSize       pointer to the size of EC Point context
//
*F*/
IPPFUN(IppStatus, ippsECCPPointGetSize, (int feBitSize, int* pSize))
{
   /* test size's pointer */
   IPP_BAD_PTR1_RET(pSize);

   /* test size of field element */
   IPP_BADARG_RET((2>feBitSize), ippStsSizeErr);

   {
      int elemLen = BITS_BNU_CHUNK(feBitSize);
      *pSize= (Ipp32s)sizeof(IppsGFpECPoint)
             +elemLen*(Ipp32s)sizeof(BNU_CHUNK_T) /* X */
             +elemLen*(Ipp32s)sizeof(BNU_CHUNK_T) /* Y */
             +elemLen*(Ipp32s)sizeof(BNU_CHUNK_T);/* Z */
   }
   return ippStsNoErr;
}
