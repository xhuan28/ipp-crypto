/*******************************************************************************
* Copyright 2016 Intel Corporation
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
//     Cryptography Primitive. AES keys expansion
// 
//  Contents:
//        aes192_KeyExpansion_NI()
//
*/

#include "owndefs.h"
#include "owncp.h"
#include "pcpaesm.h"
#include "pcpaes_keys_ni.h"

#if (_AES_NI_ENABLING_==_FEATURE_ON_) || (_AES_NI_ENABLING_==_FEATURE_TICKTOCK_)

//////////////////////////////////////////////////////////////////////
/*
// AES-192 key expansion
*/
static void aes192_assist(__m128i* temp1, __m128i * temp2, __m128i * temp3)
{
   __m128i temp4;
   *temp2 = _mm_shuffle_epi32 (*temp2, 0x55);
   temp4 = _mm_slli_si128 (*temp1, 0x4);
   *temp1 = _mm_xor_si128 (*temp1, temp4);
   temp4 = _mm_slli_si128 (temp4, 0x4);
   *temp1 = _mm_xor_si128 (*temp1, temp4);
   temp4 = _mm_slli_si128 (temp4, 0x4);
   *temp1 = _mm_xor_si128 (*temp1, temp4);
   *temp1 = _mm_xor_si128 (*temp1, *temp2);
   *temp2 = _mm_shuffle_epi32(*temp1, 0xff);
   temp4 = _mm_slli_si128 (*temp3, 0x4);
   *temp3 = _mm_xor_si128 (*temp3, temp4);
   *temp3 = _mm_xor_si128 (*temp3, *temp2);
}

IPP_OWN_DEFN (void, aes192_KeyExpansion_NI, (Ipp8u* keyExp, const Ipp8u* userkey))
{
   __m128i *pKeySchedule = (__m128i*)keyExp;

   __m128i temp[3];
   /*
      temp[0] = temp1
      temp[1] = temp2
      temp[2] = temp3
   */
   
   temp[0] = _mm_loadu_si128((__m128i*)userkey);
   temp[2] =  _mm_cvtsi64_si128(((Ipp64s*)(userkey+16))[0]);//_mm_loadu_si128((__m128i*)(userkey+16)); // read 8 bytes only other are zero

   pKeySchedule[0]=temp[0];
   pKeySchedule[1]=temp[2];
   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x1);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[1] = _mm_unpacklo_epi64 (pKeySchedule[1], temp[0]);
   pKeySchedule[2] = _mm_alignr_epi8 (temp[2], temp[0], 8);

   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x2);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[3]=temp[0];
   pKeySchedule[4]=temp[2];
   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x4);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[4] = _mm_unpacklo_epi64(pKeySchedule[4], temp[0]);
   pKeySchedule[5] = _mm_alignr_epi8(temp[2], temp[0], 8);

   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x8);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[6]=temp[0];
   pKeySchedule[7]=temp[2];
   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x10);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[7] = _mm_unpacklo_epi64(pKeySchedule[7], temp[0]);
   pKeySchedule[8] = _mm_alignr_epi8(temp[2], temp[0],8);

   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x20);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[9]=temp[0];
   pKeySchedule[10]=temp[2];
   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x40);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[10] = _mm_unpacklo_epi64(pKeySchedule[10], temp[0]);
   pKeySchedule[11] = _mm_alignr_epi8(temp[2], temp[0], 8);

   temp[1]=_mm_aeskeygenassist_si128 (temp[2],0x80);
   aes192_assist(&temp[0], &temp[1], &temp[2]);
   pKeySchedule[12]=temp[0];

   /* clear secret data */
   for(int i = 0; i < sizeof(temp)/sizeof(temp[0]); i++){
      temp[i] = _mm_xor_si128(temp[i],temp[i]);
   }
}

#endif /* #if (_AES_NI_ENABLING_==_FEATURE_ON_) || (_AES_NI_ENABLING_==_FEATURE_TICKTOCK_) */

