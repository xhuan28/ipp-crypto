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
//
//  Purpose:
//     Cryptography Primitive.
//     Digesting message according to SHA256
//
//  Contents:
//        ippsHashMethod_SHA224_TT()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcphash.h"
#include "pcphash_rmf.h"
#include "pcptool.h"
#include "pcpsha256stuff.h"

/*F*
//    Name: ippsHashMethod_SHA224_TT
//
// Purpose: Return SHA224 method
//          (using the Intel® Secure Hash Algorithm - New Instructions (Intel® SHA-NI) instructions set
//             if it is available at run time)
//
// Returns:
//          Pointer to SHA224 hash-method
//          (using the Intel SHA-NI instructions set
//             if it is available at run time)
//
*F*/

IPPFUN( const IppsHashMethod*, ippsHashMethod_SHA224_TT, (void) )
{
   static IppsHashMethod method = {
      ippHashAlg_SHA224,
      IPP_SHA224_DIGEST_BITSIZE/8,
      MBS_SHA256,
      MLR_SHA256,
      0,
      0,
      0,
      0
   };

   method.hashInit   = sha224_hashInit;
   method.hashUpdate = sha256_hashUpdate;
   method.hashOctStr = sha224_hashOctString;
   method.msgLenRep  = sha256_msgRep;

   #if (_SHA_NI_ENABLING_==_FEATURE_TICKTOCK_ || _SHA_NI_ENABLING_==_FEATURE_ON_)
   if(IsFeatureEnabled(ippCPUID_SHA))
      method.hashUpdate = sha256_ni_hashUpdate;
   #endif
   return &method;
}
