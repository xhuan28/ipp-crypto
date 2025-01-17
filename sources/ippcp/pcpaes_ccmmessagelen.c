/*******************************************************************************
* Copyright 2013 Intel Corporation
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
// 
//     Context:
//        ippsAES_CCMMessageLen()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcpaesauthccm.h"
#include "pcptool.h"

#if (_ALG_AES_SAFE_==_ALG_AES_SAFE_COMPACT_SBOX_)
#  include "pcprijtables.h"
#endif

/*F*
//    Name: ippsAES_CCMMessageLen
//
// Purpose: Setup expected length of payload (in bytes).
//
// Returns:                Reason:
//    ippStsNullPtrErr        pState == NULL
//    ippStsContextMatchErr   !VALID_AESCCM_ID()
//    ippStsNoErr             no errors
//
// Parameters:
//    msgLen   length in bytes of the message expected to be processed
//    pState   pointer to the AES-CCM state
//
*F*/
IPPFUN(IppStatus, ippsAES_CCMMessageLen,(Ipp64u msgLen, IppsAES_CCMState* pState))
{
   /* test context */
   IPP_BAD_PTR1_RET(pState);
   IPP_BADARG_RET(!VALID_AESCCM_ID(pState), ippStsContextMatchErr);

   /* init for new message */
   AESCCM_MSGLEN(pState) = msgLen;

   return ippStsNoErr;
}
