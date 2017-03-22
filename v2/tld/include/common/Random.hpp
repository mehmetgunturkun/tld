#ifndef RANDOM_GEN_H
#define RANDOM_GEN_H

/* Period parameters */
#define RAND_N 624
#define RAND_M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

class Random {
public:
    static unsigned long* mt; /* the array for the state vector  */
    static int mti; /* mti==N+1 means mt[N] is not initialized */

    /* initializes mt[N] with a seed */
    static void init_genrand(unsigned long s) {
        mt[0]= s & 0xffffffffUL;
        for (mti=1; mti<RAND_N; mti++) {
            mt[mti] =
            (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
            /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
            /* In the previous versions, MSBs of the seed affect   */
            /* only MSBs of the array mt[].                        */
            /* 2002/01/09 modified by Makoto Matsumoto             */
            mt[mti] &= 0xffffffffUL;
            /* for >32 bit machines */
        }
    }

    /* initialize by an array with array-length */
    /* init_key is the array for initializing keys */
    /* key_length is its length */
    /* slight change for C++, 2004/2/26 */
    static void init_by_array(unsigned long init_key[], int key_length) {
        int i, j, k;
        init_genrand(19650218UL);
        i=1; j=0;
        k = (RAND_N>key_length ? RAND_N : key_length);
        for (; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
              + init_key[j] + j; /* non linear */
            mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
            i++; j++;
            if (i>=RAND_N) { mt[0] = mt[RAND_N-1]; i=1; }
            if (j>=key_length) j=0;
        }
        for (k=RAND_N-1; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
              - i; /* non linear */
            mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
            i++;
            if (i>=RAND_N) { mt[0] = mt[RAND_N-1]; i=1; }
        }

        mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
    }

    /* generates a random number on [0,0xffffffff]-interval */
    static unsigned long genrand_int32(void) {
        unsigned long y;
        static unsigned long mag01[2]={0x0UL, MATRIX_A};
        /* mag01[x] = x * MATRIX_A  for x=0,1 */

        if (mti >= RAND_N) { /* generate N words at one time */
            int kk;

            if (mti == RAND_N+1)   /* if init_genrand() has not been called, */
                init_genrand(5489UL); /* a default initial seed is used */

            for (kk=0;kk<RAND_N-RAND_M;kk++) {
                y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
                mt[kk] = mt[kk+RAND_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
            }
            for (;kk<RAND_N-1;kk++) {
                y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
                mt[kk] = mt[kk+(RAND_M-RAND_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
            }
            y = (mt[RAND_N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
            mt[RAND_N-1] = mt[RAND_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

            mti = 0;
        }

        y = mt[mti++];

        /* Tempering */
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680UL;
        y ^= (y << 15) & 0xefc60000UL;
        y ^= (y >> 18);

        return y;
    }

    /* generates a random number on [0,0x7fffffff]-interval */
    static long genrand_int31(void) {
        return (long)(genrand_int32()>>1);
    }

    /* generates a random number on [0,1]-real-interval */
    static double genrand_real1(void) {
        return genrand_int32()*(1.0/4294967295.0);
        /* divided by 2^32-1 */
    }

    /* generates a random number on [0,1)-real-interval */
    static double genrand_real2(void) {
        return genrand_int32()*(1.0/4294967296.0);
        /* divided by 2^32 */
    }

    /* generates a random number on (0,1)-real-interval */
    static double genrand_real3(void) {
        return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0);
        /* divided by 2^32 */
    }

    /* generates a random number on [0,1) with 53-bit resolution*/
    static double genrand_res53(void) {
        unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6;
        return(a*67108864.0+b)*(1.0/9007199254740992.0);
    }

    static void seed() {
        init_genrand(100);
    }

    static float randomFloat() {
        return genrand_res53();
    }

    static vector<int> randPerm(int size) {
        vector<float> randoms;
        for (int i = 0; i < size; i++) {
            randoms.push_back(randomFloat());
        }
        vector<float> sortedRandoms;
        sortedRandoms.insert(sortedRandoms.end(), randoms.begin(), randoms.end());
        sort(sortedRandoms.begin(), sortedRandoms.end());
        vector<int> indexes;
        for (int i = 0; i < size; i++) {
            float sortedValue = sortedRandoms[i];
            for (int j = 0; j < size; j++) {
                if (randoms[j] == sortedValue) {
                    indexes.push_back(j);
                    break;
                }
            }
        }
        return indexes;
    }

    template <class T>
    static vector<T> randomSample(vector<T> vector) {
        int portion = (int) vector.size();
        std::vector<T> randomIndices(portion);

        std::vector<int> v = Random::randPerm((int) vector.size());

        for (int i = 0; i < portion; i++) {
            int index = v[i];
            randomIndices[i] = vector[index];
        }
        return randomIndices;
    }

    template <class T>
    static vector<T> randomSample(vector<T> vector, int portion) {
        std::vector<T> randomIndices(portion);

        std::vector<int> v = Random::randPerm((int) vector.size());
        std::sort (v.begin(), v.begin()+portion);

        for (int i = 0; i < portion; i++) {
            int index = v[i];
            randomIndices[i] = vector[index];
        }
        return randomIndices;
    }

    template <class T>
    static vector<T> splitData(vector<T> vector, int part) {
        int size = (int) vector.size();
        std::vector<T> shuffledVector = Random::randomSample(vector);

        int partSize = size / part;
        std::vector<T> firstPart(shuffledVector.begin(), shuffledVector.begin() + partSize);
        return firstPart;
    }
};
#endif
