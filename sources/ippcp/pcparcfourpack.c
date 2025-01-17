/*******************************************************************************
* Copyright 2005 Intel Corporation
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
//     RC4 implementation
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcparcfour.h"
#include "pcptool.h"

/*F*
//    Name: ippsARCFourPack
//
// Purpose: Copy initialized context to the buffer.
//
// Returns:                Reason:
//    ippStsNullPtrErr        pSize == NULL
//                            pCtx == NULL
//    ippStsNoErr             no errors
//
// Parameters:
//    pCtx        pointer ARCFour spec
//    pSize       pointer to the packed spec size
//
*F*/
IPPFUN(IppStatus, ippsARCFourPack,(const IppsARCFourState* pCtx, Ipp8u* pBuffer))
{
   /* test pointers */
   IPP_BAD_PTR2_RET(pCtx, pBuffer);
   /* test the context */
   IPP_BADARG_RET(!RC4_VALID_ID(pCtx), ippStsContextMatchErr);

   CopyBlock(pCtx, pBuffer, sizeof(IppsARCFourState));
   IppsARCFourState* pCopy = (IppsARCFourState*)pBuffer;
   RC4_RESET_ID(pCopy);
   
   return ippStsNoErr;
}
