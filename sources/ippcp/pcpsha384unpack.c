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
//     SHA512 message digest
// 
//  Contents:
//        ippsSHA384Unpack()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcphash.h"
#include "pcphash_rmf.h"
#include "pcptool.h"

/*F*
//    Name: ippsSHA384Unpack
//
// Purpose: Unpack buffer content into the initialized context.
//
// Returns:                Reason:
//    ippStsNullPtrErr        pState == NULL
//                            pBuffer== NULL
//    ippStsNoErr             no errors
//
// Parameters:
//    pBuffer     pointer to the input buffer
//    pState      pointer hash state
//
*F*/

IPPFUN(IppStatus, ippsSHA384Unpack,(const Ipp8u* pBuffer, IppsSHA384State* pState))
{
   return ippsSHA512Unpack(pBuffer, pState);
}
