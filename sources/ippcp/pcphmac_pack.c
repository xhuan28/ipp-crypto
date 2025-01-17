/*******************************************************************************
* Copyright 2014 Intel Corporation
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
//     HMAC General Functionality
// 
//  Contents:
//        ippsHMAC_Pack()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcphmac.h"
#include "pcptool.h"

/*F*
//    Name: ippsHMAC_Pack
//
// Purpose: Copy initialized context to the buffer.
//
// Returns:                Reason:
//    ippStsNullPtrErr        pSize == NULL
//                            pCtx == NULL
//    ippStsNoErr             no errors
//
// Parameters:
//    pCtx        pointer keyed hash state
//    pBuffer     pointer to the destination buffer
//    bufSize     size of destination buffer
//
*F*/
IPPFUN(IppStatus, ippsHMAC_Pack,(const IppsHMACState* pCtx, Ipp8u* pBuffer, int bufSize))
{
   /* test pointers */
   IPP_BAD_PTR2_RET(pCtx, pBuffer);
   /* test the context */
   IPP_BADARG_RET(!HMAC_VALID_ID(pCtx), ippStsContextMatchErr);

   {
      int ctxSize;
      ippsHMAC_GetSize(&ctxSize);
      /* test buffer length */
      IPP_BADARG_RET(ctxSize>bufSize, ippStsNoMemErr);

      CopyBlock(pCtx, pBuffer, ctxSize);
      IppsHMACState* pCopy = (IppsHMACState*)pBuffer;
      HMAC_RESET_CTX_ID(pCopy);
      return ippStsNoErr;
   }
}
