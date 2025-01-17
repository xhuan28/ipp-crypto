/*******************************************************************************
* Copyright 2002 Intel Corporation
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
// Montgomery engine preparation (GetSize/init/Set)
*/

/*
//     Intel(R) Integrated Performance Primitives. Cryptography Primitives.
// 
//     Context:
//        gsMethod_RSA_gpr_public()
//
*/

#include "owncp.h"
#include "pcpngmontexpstuff.h"
#include "gsscramble.h"
#include "pcpngrsamethod.h"
#include "pcpngrsa.h"

#if (_IPP32E>=_IPP32E_L9)
#include "pcpngmontexpstuff_avx2.h"

IPP_OWN_DEFN (gsMethod_RSA*, gsMethod_RSA_avx2_public, (void))
{
   static gsMethod_RSA m = {
      RSA_AVX2_MIN_BITSIZE, RSA_AVX2_MAX_BITSIZE,  /* RSA range */

      /* public key exponentiation: public, binary gpr */
      gsMontExpBinBuffer,
      gsModExpBin_BNU,
      NULL
   };
   return &m;
}
#endif /* _IPP32E_L9 */
