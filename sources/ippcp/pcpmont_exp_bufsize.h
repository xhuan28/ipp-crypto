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
//     DL over Prime Field
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcpdlp.h"
#include "pcptool.h"

/*
// Size of scratch buffer, involved in MontExp operation
//
// nExponents     - number of exponents
// expBitSize     - (max) sizeof exponent (in bits)
// modulusBitSize - size of modulus (bits)
*/
static cpSize cpMontExpScratchBufferSize(cpSize modulusBitSize, cpSize expBitSize, cpSize nExponents)
{
    /* sizeof table element */
    cpSize elmDataSize = BITS_BNU_CHUNK(modulusBitSize) * (Ipp32s)sizeof(BNU_CHUNK_T);
    /* get window_size */
    cpSize w = (nExponents == 1) ? cpMontExp_WinSize(expBitSize) : /* use optimal window size, if single-scalar operation */
        nExponents;                     /* or pseudo-oprimal if multi-scalar operation */

                                        /* number of table entries */
    cpSize nPrecomputed = 1 << w;

    cpSize bufferSize = elmDataSize*nPrecomputed + (CACHE_LINE_SIZE - 1);
    return bufferSize;
}