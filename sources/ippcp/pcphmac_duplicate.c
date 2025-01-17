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
//        ippsHMAC_Duplicate()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcphmac.h"
#include "pcptool.h"

/*F*
//    Name: ippsHMAC_Duplicate
//
// Purpose: Clone HMAC state.
//
// Returns:                Reason:
//    ippStsNullPtrErr        pSrcState == NULL
//                            pDstState == NULL
//    ippStsContextMatchErr   pSrcState->idCtx != idCtxHMAC
//                            pDstState->idCtx != idCtxHMAC
//    ippStsNoErr             no errors
//
// Parameters:
//    pSrcState   pointer to the source HMAC state
//    pDstState   pointer to the target HMAC state
//
// Note:
//    pDstState may not to be initialized by ippsHMACInit()
//
*F*/
IPPFUN(IppStatus, ippsHMAC_Duplicate,(const IppsHMACState* pSrcCtx, IppsHMACState* pDstCtx))
{
   /* test state pointers */
   IPP_BAD_PTR2_RET(pSrcCtx, pDstCtx);
   /* test states ID */
   IPP_BADARG_RET(!HMAC_VALID_ID(pSrcCtx), ippStsContextMatchErr);

   /* copy HMAC state without Hash context */
   CopyBlock(pSrcCtx, pDstCtx, (int)(IPP_UINT_PTR(&HASH_CTX(pSrcCtx)) - IPP_UINT_PTR(pSrcCtx)));
   HMAC_SET_CTX_ID(pDstCtx);
   /* copy Hash context separately */
   ippsHashDuplicate(&HASH_CTX(pSrcCtx), &HASH_CTX(pDstCtx));

   return ippStsNoErr;
}
