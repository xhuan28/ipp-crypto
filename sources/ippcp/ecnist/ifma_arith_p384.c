/*******************************************************************************
 * Copyright 2021 Intel Corporation
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

#include "ifma_arith_p384.h"
#include "ifma_defs.h"

#define LEN52 (NUMBER_OF_DIGITS(384, 52))

/* p384r1: p = 2^384 - 2^128 - 2^96 + 2^32 - 1 */
static const __ALIGN64 Ipp64u p384_x1[LEN52] = {
   0x00000000FFFFFFFF, 0x000FF00000000000, 0x000FFFFFFEFFFFFF, 0x000FFFFFFFFFFFFF, 0x000FFFFFFFFFFFFF, 0x000FFFFFFFFFFFFF, 0x000FFFFFFFFFFFFF, 0x00000000000FFFFF
};

/* modulus 4*p */
static const __ALIGN64 Ipp64u p384_x4[LEN52] = {
   0x00000003FFFFFFFC, 0x000FC00000000000, 0x000FFFFFFBFFFFFF, 0x000FFFFFFFFFFFFF, 0x000FFFFFFFFFFFFF, 0x000FFFFFFFFFFFFF, 0x000FFFFFFFFFFFFF, 0x00000000003FFFFF
};

/* to Montgomery conversion constant
 * rr = 2^((P384_LEN52*DIGIT_SIZE)*2) mod p384
 */
static const __ALIGN64 Ipp64u p384_rr[LEN52] = {
   0x0000000000000000, 0x000FE00000001000, 0x0000000000FFFFFF, 0x0000000000000020, 0x0000FFFFFFFE0000, 0x0000000020000000, 0x0000000000000100, 0x0000000000000000
};

static const __ALIGN64 Ipp64u ones[LEN52] = {
   1, 0, 0, 0, 0, 0, 0, 0
};

#define NORM_ROUND(R)                                                                           \
   {                                                                                            \
      const m512 carry       = srai_i64((R), DIGIT_SIZE);                            /* O(1) */ \
      const m512 shift_carry = maskz_permutexvar_i8(mask_shift_carry, idxi8, carry); /* O(1) */ \
      const m512 radix52     = and_i64((R), mask52radix);                            /* O(1) */ \
      (R)                    = add_i64(radix52, shift_carry);                        /* O(1) */ \
   }

IPP_OWN_DEFN(m512, ifma_norm52_p384, (const m512 a))
{
   const m512 mask52radix        = set1_i64(DIGIT_MASK);
   const mask64 mask_shift_carry = 0xFFFFFFFFFFFFFF00;

   const m512 idxi8 = set_i8(55, 54, 53, 52, 51, 50, 49, 48,
                             47, 46, 45, 44, 43, 42, 41, 40,
                             39, 38, 37, 36, 35, 34, 33, 32,
                             31, 30, 29, 28, 27, 26, 25, 24,
                             23, 22, 21, 20, 19, 18, 17, 16,
                             15, 14, 13, 12, 11, 10, 9, 8,
                             7, 6, 5, 4, 3, 2, 1, 0,
                             0, 0, 0, 0, 0, 0, 0, 0);

   m512 r = a;
   /* 7 reduction round = O(32) */
   NORM_ROUND(r)
   NORM_ROUND(r)
   NORM_ROUND(r)
   NORM_ROUND(r)
   NORM_ROUND(r)
   NORM_ROUND(r)
   NORM_ROUND(r)

   return r;
}

IPP_OWN_DEFN(void, ifma_norm52_dual_p384,
             (m512 * pr1, const m512 a1,
              m512 *pr2, const m512 a2))
{
   const m512 mask52radix        = set1_i64(DIGIT_MASK);
   const mask64 mask_shift_carry = 0xFFFFFFFFFFFFFF00;

   const m512 idxi8 = set_i8(55, 54, 53, 52, 51, 50, 49, 48,
                             47, 46, 45, 44, 43, 42, 41, 40,
                             39, 38, 37, 36, 35, 34, 33, 32,
                             31, 30, 29, 28, 27, 26, 25, 24,
                             23, 22, 21, 20, 19, 18, 17, 16,
                             15, 14, 13, 12, 11, 10, 9, 8,
                             7, 6, 5, 4, 3, 2, 1, 0,
                             0, 0, 0, 0, 0, 0, 0, 0);

   m512 r1 = a1;
   m512 r2 = a2;
   /* 7 reduction round = O(32) */
   /* 1-2 */
   NORM_ROUND(r1)
   NORM_ROUND(r1)
   /* 1-2 */
   NORM_ROUND(r2)
   NORM_ROUND(r2)
   /* 3-4 */
   NORM_ROUND(r1)
   NORM_ROUND(r1)
   /* 3-4 */
   NORM_ROUND(r2)
   NORM_ROUND(r2)
   /* 5-6 */
   NORM_ROUND(r1)
   NORM_ROUND(r1)
   /* 5-6 */
   NORM_ROUND(r2)
   NORM_ROUND(r2)
   /* 7-7 */
   NORM_ROUND(r1)
   NORM_ROUND(r2)

   *pr1 = r1;
   *pr2 = r2;

   return;
}

IPP_OWN_DEFN(m512, ifma_lnorm52_p384, (const m512 a))
{
   const m512 mask52             = set1_i64(DIGIT_MASK);
   const m512 one                = set1_i64(1ULL);
   const mask64 mask_shift_carry = 0xFFFFFFFFFFFFFF00;

   const m512 idxi8 = set_i8(55, 54, 53, 52, 51, 50, 49, 48,
                             47, 46, 45, 44, 43, 42, 41, 40,
                             39, 38, 37, 36, 35, 34, 33, 32,
                             31, 30, 29, 28, 27, 26, 25, 24,
                             23, 22, 21, 20, 19, 18, 17, 16,
                             15, 14, 13, 12, 11, 10, 9, 8,
                             7, 6, 5, 4, 3, 2, 1, 0,
                             0, 0, 0, 0, 0, 0, 0, 0);

   m512 r = a;
   int k1, k2;
   m512 carry = srai_i64(r, DIGIT_SIZE);
   carry      = maskz_permutexvar_i8(mask_shift_carry, idxi8, carry);

   r = and_i64(r, mask52);
   r = add_i64(r, carry);

   k2 = (int)(cmp_i64_mask(mask52, r, _MM_CMPINT_EQ));
   k1 = (int)(cmp_i64_mask(mask52, r, _MM_CMPINT_LT));

   k1 = k2 + (k1 << 1);
   k1 ^= k2;

   r = mask_add_i64(r, (mask8)(k1), r, one);
   r = and_i64(r, mask52);

   return r;
}

IPP_OWN_DEFN(void, ifma_lnorm52_dual_p384,
             (m512 * pr1, const m512 a1,
              m512 *pr2, const m512 a2))
{
   const m512 mask52             = set1_i64(DIGIT_MASK);
   const m512 one                = set1_i64(1UL);
   const mask64 mask_shift_carry = 0xFFFFFFFFFFFFFF00;

   const m512 idxi8 = set_i8(55, 54, 53, 52, 51, 50, 49, 48,
                             47, 46, 45, 44, 43, 42, 41, 40,
                             39, 38, 37, 36, 35, 34, 33, 32,
                             31, 30, 29, 28, 27, 26, 25, 24,
                             23, 22, 21, 20, 19, 18, 17, 16,
                             15, 14, 13, 12, 11, 10, 9, 8,
                             7, 6, 5, 4, 3, 2, 1, 0,
                             0, 0, 0, 0, 0, 0, 0, 0);

   m512 r1 = a1;
   m512 r2 = a2;
   int k1_1, k1_2, k2_1, k2_2;

   m512 carry1 = srai_i64(r1, DIGIT_SIZE);
   m512 carry2 = srai_i64(r2, DIGIT_SIZE);
   carry1      = maskz_permutexvar_i8(mask_shift_carry, idxi8, carry1);
   carry2      = maskz_permutexvar_i8(mask_shift_carry, idxi8, carry2);

   r1 = and_i64(r1, mask52);
   r1 = add_i64(r1, carry1);
   r2 = and_i64(r2, mask52);
   r2 = add_i64(r2, carry2);

   k2_1 = (int)(cmp_i64_mask(mask52, r1, _MM_CMPINT_EQ));
   k1_1 = (int)(cmp_i64_mask(mask52, r1, _MM_CMPINT_LT));
   k2_2 = (int)(cmp_i64_mask(mask52, r2, _MM_CMPINT_EQ));
   k1_2 = (int)(cmp_i64_mask(mask52, r2, _MM_CMPINT_LT));

   k1_1 = k2_1 + (k1_1 << 1);
   k1_1 ^= k2_1;
   k1_2 = k2_2 + (k1_2 << 1);
   k1_2 ^= k2_2;

   r1 = mask_add_i64(r1, (mask8)(k1_1), r1, one);
   r1 = and_i64(r1, mask52);
   r2 = mask_add_i64(r2, (mask8)(k1_2), r2, one);
   r2 = and_i64(r2, mask52);

   *pr1 = r1;
   *pr2 = r2;
   return;
}

/* R = (A/2) */
IPP_OWN_DEFN(m512, ifma_half52_p384, (const m512 a))
{
   const m512 M    = loadu_i64(p384_x1);
   const m512 zero = setzero_i64();
   const m512 one  = set1_i64(1LL);

   const mask8 is_last_one = cmp_i64_mask(and_i64(a, one), zero, _MM_CMPINT_EQ);
   const mask8 mask        = (mask8)((is_last_one & 1) - 1);

   m512 r = mask_add_i64(a, mask, a, M);
   r      = ifma_lnorm52_p384(r);

   /* 1-bit shift right */
   /* extract last bit + >> 64 */
   const mask64 mask_shift = 0x00FFFFFFFFFFFFFF;
   const m512 idx_shift    = set_i8(0, 0, 0, 0, 0, 0, 0, 0,
                                 63, 62, 61, 60, 59, 58, 57, 56,
                                 55, 54, 53, 52, 51, 50, 49, 48,
                                 47, 46, 45, 44, 43, 42, 41, 40,
                                 39, 38, 37, 36, 35, 34, 33, 32,
                                 31, 30, 29, 28, 27, 26, 25, 24,
                                 23, 22, 21, 20, 19, 18, 17, 16,
                                 15, 14, 13, 12, 11, 10, 9, 8);

   m512 shift_right = maskz_permutexvar_i8(mask_shift, idx_shift, and_i64(r, one));
   /* set last bit is first bit (52 radix) */
   shift_right = slli_i64(shift_right, DIGIT_SIZE - 1);
   /* join first new bit */
   r = srli_i64(r, 1);          /* create slot by first bit 1111 -> 0111 */
   r = add_i64(r, shift_right); /* join first and other bit */

   return r;
}

IPP_OWN_DEFN(m512, ifma_neg52_p384, (const m512 a))
{
   const m512 M4 = loadu_i64(p384_x4);

   /* a == 0 ? 0xFF : 0 */
   const mask8 mask_zero = is_zero_i64(a);

   /* r = 4*p - a */
   m512 r = mask_sub_i64(a, ~mask_zero, M4, a);
   r      = ifma_norm52_p384(r);
   return r;
}

#define MULT_ROUND(R, A, B, IDX)                                            \
   const m512 Bi##R##IDX     = permutexvar_i8(idx##IDX, (B));               \
   const m512 amBiLo##R##IDX = madd52lo_i64(zero, (A), Bi##R##IDX);         \
   m512 tr##R##IDX           = madd52hi_i64(zero, (A), Bi##R##IDX);         \
   {                                                                        \
      /* low */                                                             \
      (R)           = add_i64((R), amBiLo##R##IDX);                         \
      const m512 R0 = permutexvar_i8(idx0, (R));                            \
      const m512 u  = add_i64(slli_i64(R0, 32), R0);                        \
      tr##R##IDX    = madd52hi_i64(tr##R##IDX, M, u);                       \
      (R)           = madd52lo_i64((R), M, u);                              \
      /* shift */                                                           \
      const m512 carryone = maskz_srai_i64(maskone, (R), DIGIT_SIZE);       \
      tr##R##IDX          = add_i64(tr##R##IDX, carryone);                  \
      (R)                 = maskz_permutexvar_i8(mask_sr64, idx_sr64, (R)); \
      /* hi */                                                              \
      (R) = add_i64((R), tr##R##IDX);                                       \
   }

/* gueron data */
/* R = (A*B) - no normalization radix 52 */
IPP_OWN_DEFN(m512, ifma_amm52_p384, (const m512 a, const m512 b))
{
   const m512 M        = loadu_i64(p384_x1); /* p */
   const m512 zero     = setzero_i64();
   const mask8 maskone = 0x1;
   /* index broadcast */
   const m512 idx0 = set_i8(REPL8(7, 6, 5, 4, 3, 2, 1, 0));         /* B[0] */
   const m512 idx1 = set_i8(REPL8(15, 14, 13, 12, 11, 10, 9, 8));   /* B[1] */
   const m512 idx2 = set_i8(REPL8(23, 22, 21, 20, 19, 18, 17, 16)); /* B[2] */
   const m512 idx3 = set_i8(REPL8(31, 30, 29, 28, 27, 26, 25, 24)); /* B[3] */
   const m512 idx4 = set_i8(REPL8(39, 38, 37, 36, 35, 34, 33, 32)); /* B[4] */
   const m512 idx5 = set_i8(REPL8(47, 46, 45, 44, 43, 42, 41, 40)); /* B[5] */
   const m512 idx6 = set_i8(REPL8(55, 54, 53, 52, 51, 50, 49, 48)); /* B[6] */
   const m512 idx7 = set_i8(REPL8(63, 62, 61, 60, 59, 58, 57, 56)); /* B[7] */

   /* shift right 64 bit line [R >> 64] */
   const mask64 mask_sr64 = 0x00FFFFFFFFFFFFFF;
   const m512 idx_sr64    = set_i8(0, 0, 0, 0, 0, 0, 0, 0,
                                63, 62, 61, 60, 59, 58, 57, 56,
                                55, 54, 53, 52, 51, 50, 49, 48,
                                47, 46, 45, 44, 43, 42, 41, 40,
                                39, 38, 37, 36, 35, 34, 33, 32,
                                31, 30, 29, 28, 27, 26, 25, 24,
                                23, 22, 21, 20, 19, 18, 17, 16,
                                15, 14, 13, 12, 11, 10, 9, 8);

   m512 r = setzero_i64();
   /* P384
     * m' mod b = 2^32 + 1
     *
     * Algorithm
     * a[] b[] - input data (radix 52) m[] - module p384
     *
     * when u =  R[0]*m' mod b
     * 1) R = R + a[] * b[i] (lo)
     * 2) R = R + m[] * u    (lo)
     * 3) R = R >> 64
     * 4) R = R + a[] * b[i] (hi)
     * 5) R = R + m[] * u    (hi)
     */
   /* one round = O(32) */
   MULT_ROUND(r, a, b, 0)
   MULT_ROUND(r, a, b, 1)
   MULT_ROUND(r, a, b, 2)
   MULT_ROUND(r, a, b, 3)
   MULT_ROUND(r, a, b, 4)
   MULT_ROUND(r, a, b, 5)
   MULT_ROUND(r, a, b, 6)
   MULT_ROUND(r, a, b, 7)

   return r;
}

IPP_OWN_DEFN(void, ifma_amm52_dual_p384, (m512 * pr1, const m512 a1, const m512 b1, m512 *pr2, const m512 a2, const m512 b2))
{
   const m512 M        = loadu_i64(p384_x1); /* p */
   const m512 zero     = setzero_i64();
   const mask8 maskone = 0x1;
   /* index broadcast */
   const m512 idx0 = set_i8(REPL8(7, 6, 5, 4, 3, 2, 1, 0));         /* B[0] */
   const m512 idx1 = set_i8(REPL8(15, 14, 13, 12, 11, 10, 9, 8));   /* B[1] */
   const m512 idx2 = set_i8(REPL8(23, 22, 21, 20, 19, 18, 17, 16)); /* B[2] */
   const m512 idx3 = set_i8(REPL8(31, 30, 29, 28, 27, 26, 25, 24)); /* B[3] */
   const m512 idx4 = set_i8(REPL8(39, 38, 37, 36, 35, 34, 33, 32)); /* B[4] */
   const m512 idx5 = set_i8(REPL8(47, 46, 45, 44, 43, 42, 41, 40)); /* B[5] */
   const m512 idx6 = set_i8(REPL8(55, 54, 53, 52, 51, 50, 49, 48)); /* B[6] */
   const m512 idx7 = set_i8(REPL8(63, 62, 61, 60, 59, 58, 57, 56)); /* B[7] */

   /* shift right 64 bit line [R >> 64] */
   const mask64 mask_sr64 = 0x00FFFFFFFFFFFFFF;
   const m512 idx_sr64    = set_i8(0, 0, 0, 0, 0, 0, 0, 0,
                                63, 62, 61, 60, 59, 58, 57, 56,
                                55, 54, 53, 52, 51, 50, 49, 48,
                                47, 46, 45, 44, 43, 42, 41, 40,
                                39, 38, 37, 36, 35, 34, 33, 32,
                                31, 30, 29, 28, 27, 26, 25, 24,
                                23, 22, 21, 20, 19, 18, 17, 16,
                                15, 14, 13, 12, 11, 10, 9, 8);

   m512 r1 = setzero_i64();
   m512 r2 = setzero_i64();
   /* P384
     * m' mod b = 2^32 + 1
     *
     * Algorithm
     * a[] b[] - input data (radix 52) m[] - module p384
     *
     * when u =  R[0]*m' mod b
     * 1) R = R + a[] * b[i] (lo)
     * 2) R = R + m[] * u    (lo)
     * 3) R = R >> 64
     * 4) R = R + a[] * b[i] (hi)
     * 5) R = R + m[] * u    (hi)
     */
   /* one round = O(32) */
   /* 0-1 */
   MULT_ROUND(r1, a1, b1, 0)
   MULT_ROUND(r1, a1, b1, 1)
   /* 0-1 */
   MULT_ROUND(r2, a2, b2, 0)
   MULT_ROUND(r2, a2, b2, 1)
   /* 2-3 */
   MULT_ROUND(r1, a1, b1, 2)
   MULT_ROUND(r1, a1, b1, 3)
   /* 2-3 */
   MULT_ROUND(r2, a2, b2, 2)
   MULT_ROUND(r2, a2, b2, 3)
   /* 4-5 */
   MULT_ROUND(r1, a1, b1, 4)
   MULT_ROUND(r1, a1, b1, 5)
   /* 4-5 */
   MULT_ROUND(r2, a2, b2, 4)
   MULT_ROUND(r2, a2, b2, 5)
   /* 6-7 */
   MULT_ROUND(r1, a1, b1, 6)
   MULT_ROUND(r1, a1, b1, 7)
   /* 6-7 */
   MULT_ROUND(r2, a2, b2, 6)
   MULT_ROUND(r2, a2, b2, 7)

   /* set out */
   *pr1 = r1;
   *pr2 = r2;
   return;
}

/* R = (A*B) with norm */
__INLINE m512 ifma_amm52_p384_norm(const m512 a, const m512 b)
{
   m512 r = ifma_amm52_p384(a, b);
   /* normalization */
   return ifma_lnorm52_p384(r);
}

/* R = (A*A) with norm */
__INLINE m512 m512_sqr_norm(const m512 a)
{
   return ifma_amm52_p384_norm(a, a);
}

/* to Montgomery domain */
IPP_OWN_DEFN(m512, ifma_tomont52_p384, (const m512 a))
{
   const m512 RR = loadu_i64(p384_rr);
   return ifma_amm52_p384_norm(a, RR);
}

static m512 mod_reduction_p384(const m512 a)
{
   const m512 M    = loadu_i64(p384_x1);
   const m512 zero = setzero_i64();

   /* r = a - M */
   m512 r = sub_i64(a, M);
   r      = ifma_norm52_p384(r);

   /* 1 < 0 */
   const mask8 lt   = cmp_i64_mask(zero, srli_i64(r, DIGIT_SIZE - 1), _MM_CMPINT_LT);
   const mask8 mask = check_bit(lt, 7);

   /* maks != 0 ? a : r */
   r = mask_mov_i64(r, mask, a);
   return r;
}

/* from Montgomery domain */
IPP_OWN_DEFN(m512, ifma_frommont52_p384, (const m512 a))
{
   const m512 one = loadu_i64(ones);

   /* from mont */
   m512 r = ifma_amm52_p384_norm(a, one);
   r      = mod_reduction_p384(r);
   return r;
}

#define sqr(R, A) (R) = m512_sqr_norm((A))
#define mul(R, A, B) (R) = ifma_amm52_p384_norm((A), (B));
#define mul_dual(R1, A1, B1, R2, A2, B2)                       \
   ifma_amm52_dual_p384(&(R1), (A1), (B1), &(R2), (A2), (B2)); \
   ifma_lnorm52_dual_p384(&(R1), (R1), &(R2), (R2))

__INLINE m512 ifma_ams52_p384_ntimes(const m512 a, Ipp32s n)
{
   m512 r = a;
   for (; n > 0; --n)
      sqr(r, r);
   return r;
}

#define sqr_ntimes(R, A, N) (R) = ifma_ams52_p384_ntimes((A), (N))

/* R = 1/z */
IPP_OWN_DEFN(m512, ifma_aminv52_p384, (const m512 z))
{
   m512 u, v, zD, zE, zF;
   u = v = zD = zE = zF = setzero_i64();

   sqr(u, z);             /*  u = z^2 */
   mul(v, u, z);          /*  v = z^2 * z     = z^3  */
   sqr_ntimes(zF, v, 2);  /* zF = (z^3)^(2^2) = z^12 */
                          /**/
   mul(zD, zF, z);        /* zD = z^11 * z    = z^xD */
   mul_dual(zE, zF, u,    /* zE = z^11 * z^2  = z^xE */
            zF, zF, v);   /* zF = z^11 * z^3  = z^xF */
                          /**/
   sqr_ntimes(u, zF, 4);  /* u  = (z^xF)^(2^4)  = z^xF0 */
   mul_dual(zD, u, zD,    /* zD = z^xF0 * z^xD  = z^xFD */
            zE, u, zE);   /* zE = z^xF0 * z^xE  = z^xFE */
   mul(zF, u, zF);        /* zF = z^xF0 * z^xF  = z^xFF */
                          /**/
   sqr_ntimes(u, zF, 8);  /* u = (z^xFF)^(2^8)    = z^xFF00 */
   mul_dual(zD, u, zD,    /* zD = z^xFF00 * z^xFD = z^xFFFD */
            zE, u, zE);   /* zE = z^xFF00 * z^xFE = z^xFFFE */
   mul(zF, u, zF);        /* zF = z^xFF00 * z^xFF = z^xFFFF */
                          /**/
   sqr_ntimes(u, zF, 16); /* u = (z^xFFFF)^(2^16)       = z^xFFFF0000 */
   mul_dual(zD, u, zD,    /* zD = z^xFFFF0000 * z^xFFFD = z^xFFFFFFFD */
            zE, u, zE);   /* zE = z^xFFFF0000 * z^xFFFE = z^xFFFFFFFE */
   mul(zF, u, zF);        /* zF = z^xFFFF0000 * z^xFFFF = z^xFFFFFFFF */
                          /**/
   sqr_ntimes(u, zF, 32); /* u = (z^xFFFFFFFF)^(2^32)               = z^xFFFFFFFF00000000 */
   mul_dual(zE, u, zE,    /* zE = z^xFFFFFFFF00000000 * z^xFFFFFFFE = z^xFFFFFFFFFFFFFFFE */
            zF, u, zF);   /* zF = z^xFFFFFFFF00000000 * z^xFFFFFFFF = z^xFFFFFFFFFFFFFFFF */
   /* v =  z^xFFFFFFFFFFFFFFFF.0000000000000000 * z^xFFFFFFFFFFFFFFFF
                = z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF */
   sqr_ntimes(v, zF, 64);
   mul(v, v, zF);
   /* v =  z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.0000000000000000 * z^xFFFFFFFFFFFFFFFF
                  = z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFFF */
   sqr_ntimes(v, v, 64);
   mul(v, v, zF);
   /* v =  z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.0000000000000000 * z^xFFFFFFFFFFFFFFFE
                = z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFE */
   sqr_ntimes(v, v, 64);
   mul(v, v, zE);
   /* v =  z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFE.0000000000000000 * z^xFFFFFFFF00000000
                = z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFE.FFFFFFFF00000000 */
   sqr_ntimes(v, v, 64);
   mul(v, v, u);
   /* r =  z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFE.FFFFFFFF00000000.0000000000000000 * z^xFFFFFFFD
                = z^xFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFFF.FFFFFFFFFFFFFFFE.FFFFFFFF00000000.00000000FFFFFFFD */
   sqr_ntimes(v, v, 64);
   m512 r = setzero_i64();
   mul(r, v, zD);
   return r;
}

#undef sqr
#undef mul
#undef mul_dual
#undef sqr_ntimes

IPP_OWN_DEFN(m512, convert_radix_to_52x8, (const Ipp64u *arad64))
{
   /* load mask to register */
   const mask8 mask_load = 0x3F;
   const m512 mask_rad52 = set1_i64(DIGIT_MASK);
   /* set data */
   const m512 idx16       = set_i16(31, 31, 23, 22,
                              22, 21, 20, 19,
                              19, 18, 17, 16,
                              16, 15, 14, 13,
                              12, 11, 10, 9,
                              9, 8, 7, 6,
                              6, 5, 4, 3,
                              3, 2, 1, 0);
   const m512 shift_right = set_i64(12LL, 8LL, 4LL, 0LL, 12LL, 8LL, 4LL, 0LL);

   m512 r = maskz_loadu_i64(mask_load, arad64);
   r      = permutexvar_i16(idx16, r);
   r      = srlv_i64(r, shift_right);
   r      = and_i64(mask_rad52, r);
   return r;
}

IPP_OWN_DEFN(void, convert_radix_to_64x6, (Ipp64u * rrad64, const m512 arad52))
{
   /* mask store */
   const mask8 mask_store = 0x3F;
   const m512 shift_left  = set_i64(4LL, 0LL, 4LL, 0LL, 4LL, 0LL, 4LL, 0LL);
   const m512 idx_up8     = set_i8(63, 63, 63, 63, 63, 63, 63, 63,
                               63, 63, 63, 63, 62, 61, 60, 59,
                               55, 55, 54, 53, 52, 51, 50, 49,
                               48, 46, 45, 44, 43, 42, 41, 40,
                               31, 31, 31, 31, 31, 31, 30, 29,
                               23, 23, 23, 23, 22, 21, 20, 19,
                               15, 15, 15, 14, 13, 12, 11, 10,
                               7, 6, 5, 4, 3, 2, 1, 0);
   const m512 idx_down8   = set_i8(63, 63, 63, 63, 63, 63, 63, 63,
                                 63, 63, 63, 63, 63, 63, 63, 63,
                                 58, 57, 56, 55, 55, 55, 55, 55,
                                 39, 39, 39, 39, 39, 39, 39, 38,
                                 37, 36, 35, 34, 33, 32, 31, 31,
                                 28, 27, 26, 25, 24, 23, 23, 23,
                                 18, 17, 16, 15, 15, 15, 15, 15,
                                 9, 8, 7, 7, 7, 7, 7, 7);

   m512 r = arad52;

   r      = sllv_i64(r, shift_left);
   m512 T = permutexvar_i8(idx_up8, r);
   r      = permutexvar_i8(idx_down8, r);
   r      = or_i64(r, T);
   mask_storeu_i64(rrad64, mask_store, r);
   return;
}

#endif // #if (_IPP32E >= _IPP32E_K1)
