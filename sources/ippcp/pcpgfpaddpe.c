/*******************************************************************************
* Copyright 2018 Intel Corporation
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
//     Intel(R) Integrated Performance Primitives. Cryptography Primitives.
//     Operations over GF(p).
// 
//     Context:
//        ippsGFpAdd_PE()
// 
*/
#include "owndefs.h"
#include "owncp.h"

#include "pcpgfpstuff.h"
#include "pcpgfpxstuff.h"
#include "pcptool.h"


/*F*
// Name: ippsGFpAdd_PE
//
// Purpose: Sum of GF element and parent GF element
//
// Returns:                   Reason:
//    ippStsNullPtrErr              NULL == pGFp
//                                  NULL == pA
//                                  NULL == pR
//                                  NULL == pParentB
//
//    ippStsContextMatchErr         invalid pGFp->idCtx
//                                  invalid pA->idCtx
//                                  invalid pR->idCtx
//                                  invalid pParentB->idCtx
//
//    ippStsOutOfRangeErr           GFPE_ROOM() != GFP_FELEN()
//
//    ippStsBadArgErr               pGFp does not specify prime field
//
//    ippStsNoErr                   no error
//
// Parameters:
//    pA         Pointer to the context of the first finite field element.
//    pParentB   Pointer to the context of the second finite field element.
//    pR         Pointer to the context of the result finite field element.
//    pGFp       Pointer to the context of the finite field.
//
*F*/

IPPFUN(IppStatus, ippsGFpAdd_PE,(const IppsGFpElement* pA, const IppsGFpElement* pParentB,
                                 IppsGFpElement* pR, IppsGFpState* pGFp))
{
   IPP_BAD_PTR4_RET(pA, pParentB, pR, pGFp);
   IPP_BADARG_RET( !GFP_VALID_ID(pGFp), ippStsContextMatchErr );
   IPP_BADARG_RET( !GFPE_VALID_ID(pA), ippStsContextMatchErr );
   IPP_BADARG_RET( !GFPE_VALID_ID(pParentB), ippStsContextMatchErr );
   IPP_BADARG_RET( !GFPE_VALID_ID(pR), ippStsContextMatchErr );
   {
      gsModEngine* pGFE = GFP_PMA(pGFp);
      IPP_BADARG_RET( GFP_IS_BASIC(pGFE), ippStsBadArgErr )
      IPP_BADARG_RET( (GFPE_ROOM(pA)!=GFP_FELEN(pGFE)) || (GFPE_ROOM(pR)!=GFP_FELEN(pGFE)), ippStsOutOfRangeErr);
      IPP_BADARG_RET( (GFPE_ROOM(pParentB)!=GFP_FELEN(GFP_PARENT(pGFE))), ippStsOutOfRangeErr);

      cpGFpxAdd_GFE(GFPE_DATA(pR), GFPE_DATA(pA), GFPE_DATA(pParentB), pGFE);
      return ippStsNoErr;
   }
}
