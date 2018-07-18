/*******************************************************************************
* Copyright 2016-2018 Intel Corporation
* All Rights Reserved.
*
* If this  software was obtained  under the  Intel Simplified  Software License,
* the following terms apply:
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*
*
* If this  software  was obtained  under the  Apache License,  Version  2.0 (the
* "License"), the following terms apply:
*
* You may  not use this  file except  in compliance  with  the License.  You may
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*
* Unless  required  by   applicable  law  or  agreed  to  in  writing,  software
* distributed under the License  is distributed  on an  "AS IS"  BASIS,  WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
* See the   License  for the   specific  language   governing   permissions  and
* limitations under the License.
*******************************************************************************/

/* 
// 
//  Purpose:
//     Cryptography Primitive.
//     Security Hash Standard
//     Generalized Functionality
// 
//  Contents:
//        ippsHashUpdate_rmf()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcphash_rmf.h"
#include "pcptool.h"

/*F*
//    Name: ippsHashUpdate_rmf
//
// Purpose: Updates intermediate hash value based on input stream.
//
// Returns:                   Reason:
//    ippStsNullPtrErr           pState == NULL
//    ippStsNullPtrErr           pSrc==0 but len!=0
//    ippStsContextMatchErr      pState->idCtx != idCtxHash
//    ippStsLengthErr            len <0
//    ippStsNoErr                no errors
//
// Parameters:
//    pSrc     pointer to the input stream
//    len      input stream length
//    pState   pointer to the Hash context
//
*F*/
IPPFUN(IppStatus, ippsHashUpdate_rmf,(const Ipp8u* pSrc, int len, IppsHashState_rmf* pState))
{
   /* test state pointer and ID */
   IPP_BAD_PTR1_RET(pState);
   pState= (IppsHashState_rmf*)( IPP_ALIGNED_PTR(pState, HASH_ALIGNMENT) );
   IPP_BADARG_RET(idCtxHash !=HASH_CTX_ID(pState), ippStsContextMatchErr);

   /* test input length */
   IPP_BADARG_RET((len<0), ippStsLengthErr);
   /* test source pointer */
   IPP_BADARG_RET((len && !pSrc), ippStsNullPtrErr);

   if(len) {
      const IppsHashMethod* method = HASH_METHOD(pState);
      hashUpdateF hashFunc = method->hashUpdate;   /* processing function */
      int msgBlkSize = method->msgBlkSize;         /* messge block size */

      int procLen;

      int idx = HAHS_BUFFIDX(pState);
      Ipp64u lenLo = HASH_LENLO(pState);
      Ipp64u lenHi = HASH_LENHI(pState);
      lenLo += len;
      if(lenLo < HASH_LENLO(pState)) lenHi++;

      /* if internal buffer is not empty */
      if(idx) {
         procLen = IPP_MIN(len, (msgBlkSize-idx));
         CopyBlock(pSrc, HASH_BUFF(pState)+idx, procLen);
         idx += procLen;

         /* process complete message block  */
         if(msgBlkSize==idx) {
            hashFunc(HASH_VALUE(pState), HASH_BUFF(pState), msgBlkSize);
            idx = 0;
         }

         /* update message pointer and length */
         pSrc += procLen;
         len  -= procLen;
      }

      /* process main part of the input*/
      procLen = len & ~(msgBlkSize-1);
      if(procLen) {
         hashFunc(HASH_VALUE(pState), pSrc, procLen);
         pSrc += procLen;
         len  -= procLen;
      }

      /* store the rest of input in the buffer */
      if(len) {
         CopyBlock(pSrc, HASH_BUFF(pState), len);
         idx += len;
      }

      /* update length of processed message */
      HASH_LENLO(pState) = lenLo;
      HASH_LENHI(pState) = lenHi;
      HAHS_BUFFIDX(pState) = idx;
   }

   return ippStsNoErr;
}