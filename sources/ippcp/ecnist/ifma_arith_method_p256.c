/*******************************************************************************
* Copyright 2017 Intel Corporation
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

#include "owndefs.h"

#if (_IPP32E >= _IPP32E_K1)

#include "ecnist/ifma_arith_method.h"
#include "ecnist/ifma_arith_p256.h"

IPP_OWN_DEFN(ifmaArithMethod *, gsArithGF_p256r1_avx512, (void))
{
   static ifmaArithMethod m = {
     /* import_to52 = */ convert_radix_to_52x5,
     /* export_to64 = */ convert_radix_to_64x4,
     /* encode      = */ ifma_tomont52_p256,
     /* decode      = */ ifma_frommont52_p256,
     /* mul         = */ ifma_amm52_p256,
     /* mul_dual    = */ ifma_amm52_dual_p256,
     /* sqr         = */ ifma_ams52_p256,
     /* sqr_dual    = */ ifma_ams52_dual_p256,
     /* norm        = */ ifma_norm52,
     /* norm_dual   = */ ifma_norm52_dual,
     /* lnorm       = */ ifma_lnorm52,
     /* lnorm_dual  = */ ifma_lnorm52_dual,
     /* add         = */ 0,
     /* sub         = */ 0,
     /* neg         = */ ifma_neg52_p256,
     /* div2        = */ ifma_half52_p256,
     /* inv         = */ ifma_aminv52_p256,
     /* red         = */ 0
   };

   return &m;
}

#endif /* #if (_IPP32E >= _IPP32E_K1) */
