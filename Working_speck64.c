#include <stdio.h>
#include <stdint.h>

#define ROTL(x, y) (((x) << (y)) | ((x) >> (32 - (y))))
#define ROTR(x, y) (((x) >> (y)) | ((x) << (32 - (y))))

uint32_t lcg(uint32_t seed) {               //pseudo random number generatorn
    return (1103515245 * seed + 12345) % 2147483648;
}

void generate_key(uint32_t k[27], uint32_t l[2 * 27]) {
    uint32_t seed = 123456789;
    for (int i = 0; i < 27; i++) {
        k[i] = lcg(seed);
        printf("Seed (%d): 0x%08X\n",i,k[i]);//randomized number for the key
        seed = k[i];
        // printf("Required Seed (%d):  0x%08X\n",i,k[i]);

    }
}

void speck(uint32_t subm_x, uint32_t subm_y) {
    uint32_t n = 32;   // Word size
    uint32_t m = 4;   // # of key words
    uint32_t T = 27;   // Number of rounds
    uint32_t x;   // Encrypted x
    uint32_t y;   // Encrypted y
    uint32_t alpha = 8; // Number of right shifts
    uint32_t beta = 3;  // Number of leftt shifts
    uint32_t k[T];
   
    uint32_t l[2 * T];

    generate_key(k, l);
    // for (int i = 0; i < 27; i++) 
    // {
    // printf("generated key(%d):  0x%08X\n",i,k[i]);
    // }

    x = subm_x;
    y = subm_y;

    /* *************** KEY EXTENSION ***************** */
    for (int i = 0; i < T; i++) {
        l[i + m - 1] = (k[i] + ROTR(l[i], alpha)) ^ i;
        //  printf("l[i + m - 1]0x%08X i=%d;m=%d (%d): 0x%08X\n",l[i],i,m,i,k[i]);
        k[i + 1] = ROTL(k[i], beta) ^ l[i + m - 1];
        printf("generated key(%d):  0x%08X\n",i,k[i]);
    }
    /* *************** ENCRYPTION ********************* */
    for (int i = 0; i < T; i++) {
        x = (ROTR(x, alpha) + y) ^ k[i];
        y = ROTL(y, beta) ^ x;
                // printf("Encrypted(%d): x = 0x%08X, y = 0x%08X\n",i, x, y);

    }
        printf("F_Encrypted: x = 0x%08X, y = 0x%08X\n", x, y);


    /* *************** DECRYPTION ********************* */
    for (int i = T - 1; i >= 0; i--) {
        y = ROTR(x ^ y, beta);
        x = ROTL((x ^ k[i]) - y, alpha);
            // printf("Decrypted(%d): x = 0x%08X, y = 0x%08X\n",i, x, y);

    }

    printf("F_Decrypted: x = 0x%08X, y = 0x%08X\n", x, y);
}

int main() {
    uint32_t x = 0xB3D76888;
    uint32_t y = 0xB0C0FFFF;

    speck(x, y);

    return 0;
}
