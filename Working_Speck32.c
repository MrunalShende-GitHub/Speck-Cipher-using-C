
//speck32 working for round 1 & 2, dont change anything here. 
    #include <stdio.h>
    #include <stdint.h>

    #define ROTL(x, y) (((x) << (y)) | ((x) >> (16 - (y))))
    #define ROTR(x, y) (((x) >> (y)) | ((x) << (16 - (y))))

    uint16_t lcg(uint16_t seed) {               //pseudo random number generator
        return (1103515245 * seed + 12345) % 2147483648;
    }

    void generate_key(uint16_t k[16], uint16_t l[2 * 16]) {
        uint16_t seed = 0x1234;
        for (int i = 0; i < 16; i++) {
            k[i] = lcg(seed);
            // printf("Seed (%d): 0x%04X\n", i, k[i]); //randomized number for the key
            seed = k[i];
        }
    }

    void speck(uint16_t subm_p, uint16_t subm_q) {
        uint16_t n = 16;   // Word size
        uint16_t m = 4;   // # of key words
        uint16_t T = 2;   // Number of rounds
        uint16_t x;   // Encrypted x
        uint16_t y;   // Encrypted y
        uint16_t alpha = 8; // Number of right shifts
        uint16_t beta = 3;  // Number of left shifts
        uint16_t k[16 ];
        uint16_t l[2 * 16];

        generate_key(k, l);

        x = subm_p;
        y = subm_q;

         /* *************** KEY EXTENSION ***************** */
    for (int i = 0; i < T; i++) {
        l[i + m - 1] = (k[i] + ROTR(l[i], alpha)) ^ i;
        //  printf("l[i + m - 1]0x%08X i=%d;m=%d (%d): 0x%08X\n",l[i],i,m,i,k[i]);
        k[i + 1] = ROTL(k[i], beta) ^ l[i + m - 1];
        printf("generated key(%d):  0x%04X\n",i,k[i]);
    }
    /* *************** ENCRYPTION ********************* */
    for (int i = 0; i < T; i++) {
        x = (ROTR(x, alpha) + y) ^ k[i];
        y = ROTL(y, beta) ^ x;
                printf("Encrypted(%d): x = 0x%04X, y = 0x%04X :Key:0x%04X\n",i, x, y,k[i]);

    }
        printf("F_Encrypted: x = 0x%04X, y = 0x%04X\n", x, y);


    /* *************** DECRYPTION ********************* */
    for (int i = T - 1; i >= 0; i--) {
        y = ROTR(x ^ y, beta);
        x = ROTL((x ^ k[i]) - y, alpha);
            printf("Decrypted(%d): x = 0x%04X, y = 0x%04X :Key:0x%04X\n",i, x, y,k[i]);

    }

    printf("F_Decrypted: x = 0x%04X, y = 0x%04X\n", x, y);
}

    int main() {
        uint16_t x =0x1209; //0011001000011011;
        uint16_t y =0x1249; //0011001000011111;

        speck(x, y);

        return 0;
    }
