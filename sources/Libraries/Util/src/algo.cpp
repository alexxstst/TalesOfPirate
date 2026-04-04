
#include "util.h"
#include "algo.h"
#include "md5.h"

using namespace std;

#pragma warning(disable : 4018 4267)

#define INL inline
#define NKD __declspec(naked)


// base64 encode and decode
#define OK (0)
#define FAIL (-1)
#define BUFOVER (-2)
#define CHAR64(c) (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])
static char const basis_64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????";

static char const basis_128[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+|\0";


static char const index_64[128] =
    {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1};

int base64(char const* src, unsigned int src_len, char* dst,
           unsigned int dst_max_len, unsigned int* dst_len)
    {
    unsigned char const* in = (unsigned char const *)src;
    unsigned char* out = (unsigned char *)dst;
    unsigned char oval;
    unsigned int olen;
    char* blah;

    olen = (src_len + 2) / 3 * 4;
    if (dst_len != NULL) *dst_len = olen;
    if (dst_max_len < olen) return BUFOVER;

    blah = (char *) out;
    while (src_len >= 3)
        {
        *out++ = basis_64[in[0] >> 2];
        *out++ = basis_64[((in[0] << 4) & 0x30) | (in[1] >> 4)];
        *out++ = basis_64[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
        *out++ = basis_64[in[2] & 0x3f];
        in += 3;
        src_len -= 3;}

    if (src_len > 0)
        {
        *out++ = basis_64[in[0] >> 2];
        oval = (in[0] << 4) & 0x30;
        if (src_len > 1) oval |= in[1] >> 4;
        *out++ = basis_64[oval];
        *out++ = (src_len < 2) ? '=' : basis_64[(in[1] << 2) & 0x3c];
        *out++ = '=';}

    if (olen < dst_max_len) *out = '\0';
    return OK;}

int ibase64(char const* src, unsigned int src_len, char* dst, unsigned int* dst_len)
    {
    unsigned len = 0, lup;
    int c1, c2, c3, c4;

    if (dst == NULL) return FAIL;

    if (src[0] == '+' && src[1] == ' ') src += 2;
    if (*src == '\r') return FAIL;

    for (lup = 0; lup < src_len / 4; ++ lup)
        {
        c1 = src[0];
        if (CHAR64(c1) == -1) return FAIL;
        c2 = src[1];
        if (CHAR64(c2) == -1) return FAIL;
        c3 = src[2];
        if (c3 != '=' && CHAR64(c3) == -1) return FAIL; 
        c4 = src[3];
        if (c4 != '=' && CHAR64(c4) == -1) return FAIL;
        src += 4;
        *dst++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4);
        ++ len;
        if (c3 != '=')
            {
            *dst++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2);
            ++ len;
            if (c4 != '=')
                {
                *dst++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4);
                ++ len;}}} // end for

    //*dst = 0;
    if (dst_len != NULL) *dst_len = len;
    return OK;}

// SHA-1
unsigned long func_S(char n, unsigned long num)
    {return (num<<n) | (num >> (32 - n));}

unsigned long func_F(unsigned int t, unsigned long B, unsigned long C, unsigned long D)
    {
    if (t < 20) {return ((B & C) | ((~B) & D));}
    else if (t < 40) {return (B ^ C ^ D);}
    else if (t < 60) {return ((B & C) | (B & D) | (C & D));}
    else if (t < 80) {return (B ^ C ^ D);}
    else return 0;}

unsigned char const* sha1(unsigned char const* msg,unsigned int len, unsigned char* md) 
    {
    unsigned long message_length;
    unsigned char i, ctemp, index;
    unsigned char hasAdd1 = 0;
    unsigned char hasAddLen = 0;
    unsigned long W[16];
    unsigned long H[5];
    unsigned long A, B, C, D, E, s;
    unsigned long MASK = 0x0000000F;
    unsigned long K[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
    unsigned long TEMP;
    unsigned int t;

    message_length = len;
    H[0] = 0x67452301;
    H[1] = 0xEFCDAB89;
    H[2] = 0x98BADCFE;
    H[3] = 0x10325476;
    H[4] = 0xC3D2E1F0;
    index = 0;

    while (!hasAddLen)
        {
        for(t = 0;t < 16; ++ t)
            {
            W[t] = 0;
            for (i = 0;i < 4; ++ i)
                {
                if (!hasAdd1)
                    {
                    if (index >= len)
                        {
                        ctemp = 0x80;
                        hasAdd1 = 1;}
                    else ctemp = msg[index ++];}
                else ctemp = 0x00;

                W[t] = W[t] * 256 + ctemp;}} // end for

        if (hasAdd1)
            if ((W[14] == 0x00) && (W[15] == 0x00))
                {
                W[15] = message_length;
                for (t = 0; t < 3; ++ t)
                    {
                    if (W[15] & 0x80000000) W[14] = W[14] * 2 + 1;
                    W[15] = W[15] * 2;}

                hasAddLen = 1;}

            A = H[0]; B = H[1]; C = H[2]; D = H[3]; E = H[4];
            for (t = 0; t < 80; ++ t)
                {
                s = t & MASK;
                if (t >= 16)
                    {
                    W[s] = W[(s + 13) & MASK] ^ W[(s + 8) & MASK]
                ^ W[(s + 2) & MASK] ^ W[s];
                W[s] = func_S(1, W[s]);}
                TEMP = func_S(5, A) + func_F(t, B, C, D) + E + W[s] + K[t / 20];
                E = D; D = C; C = func_S(30, B); B = A; A = TEMP;}

            H[0] += A; H[1] += B; H[2] += C; H[3] += D; H[4] += E;} // end while

    md[0] = (unsigned char)((H[0] >> 24) & 0xFF);
    md[1] = (unsigned char)((H[0] >> 16) & 0xFF);
    md[2] = (unsigned char)((H[0] >> 8) & 0xFF);
    md[3] = (unsigned char)((H[0]) & 0xFF);
    md[4 + 0] = (unsigned char)((H[1] >> 24) & 0xFF);
    md[4 + 1] = (unsigned char)((H[1] >> 16) & 0xFF);
    md[4 + 2] = (unsigned char)((H[1] >> 8) & 0xFF);
    md[4 + 3] = (unsigned char)((H[1]) & 0xFF);
    md[8 + 0] = (unsigned char)((H[2] >> 24) & 0xFF);
    md[8 + 1] = (unsigned char)((H[2] >> 16) & 0xFF);
    md[8 + 2] = (unsigned char)((H[2] >> 8) & 0xFF);
    md[8 + 3] = (unsigned char)((H[2]) & 0xFF);
    md[12 + 0] = (unsigned char)((H[3] >> 24) & 0xFF);
    md[12 + 1] = (unsigned char)((H[3] >> 16) & 0xFF);
    md[12 + 2] = (unsigned char)((H[3] >> 8) & 0xFF);
    md[12 + 3] = (unsigned char)((H[3]) & 0xFF);
    md[16 + 0] = (unsigned char)((H[4] >> 24) & 0xFF);
    md[16 + 1] = (unsigned char)((H[4] >> 16) & 0xFF);
    md[16 + 2] = (unsigned char)((H[4] >> 8) & 0xFF);
    md[16 + 3] = (unsigned char)((H[4]) & 0xFF);

    return md;}

#if 0
bool NKD encrypt_B(char* src, unsigned int src_len, char* key,
                   unsigned int key_len, bool en /* = true */)
    { // 'B' algorithm, src_len < 64K, key_len < 256
    __asm {
        push ebp
        push eax
        push ebx
        push ecx
        push edx
        push esi
        push edi

        mov eax, [esp + 36] ;src_len
        mov edx, [esp + 44] ;key_len
        div dl
        mov dx, ax
        mov eax, [esp + 44] ;key_len
        mul dl
        push edx
        xor ecx, ecx
        mov cx, ax
        mov ebx, [esp + 36] ;src
        xor esi, esi
        mov edx, [esp + 44] ;key
        xor edi, edi
        mov eax, [esp + 52] ;en
        cmp eax, 0
        jz DECRYPT

EN_LOOP1:
        mov ah, byte ptr [ebx + esi]
        mov al, byte ptr [edx + edi]
        xor ah, al
        mov ebp, eax
        xor ah, ah
        push edx
        mov edx, [esp + 52] ;key_len
        div dl
        pop edx
        push ecx
        mov cl, ah
        inc cl
        mov eax, ebp
        rol ah, cl
        mov byte ptr [ebx + esi], ah
        pop ecx
        inc esi
        inc edi
        push edx
        mov edx, [esp + 52] ;key_len
        mov eax, edi
        div dl
        mov dl, ah
        xor eax, eax
        mov al, dl
        pop edx
        mov edi, eax
        loop EN_LOOP1

        pop ecx
        mov al, ch
        xor ecx, ecx
        mov cl, al
        xor edi, edi
EN_LOOP2:
        mov ah, byte ptr [ebx + esi]
        mov al, byte ptr [edx + edi]
        xor ah, al
        mov ebp, eax
        xor ah, ah
        push edx
        mov edx, [esp + 48] ;key_len
        div dl
        pop edx
        push ecx
        mov cl, ah
        inc cl
        mov eax, ebp
        rol ah, cl
        mov byte ptr [ebx + esi], ah
        pop ecx
        inc esi
        inc edi
        loop EN_LOOP2
        jmp EXIT

DECRYPT:
DE_LOOP1:
        mov al, byte ptr [edx + edi]
        xor ah, ah
        push edx
        mov edx, [esp + 52] ;key_len
        div dl
        pop edx
        push ecx
        mov cl, ah
        inc cl
        mov ah, byte ptr [ebx + esi]
        ror ah, cl
        mov al, byte ptr [edx + edi]
        xor ah, al
        mov byte ptr [ebx + esi], ah
        pop ecx
        inc esi
        inc edi
        push edx
        mov edx, [esp + 52] ;key_len
        mov eax, edi
        div dl
        mov dl, ah
        xor eax, eax
        mov al, dl
        pop edx
        mov edi, eax
        loop DE_LOOP1

        pop ecx
        mov al, ch
        xor ecx, ecx
        mov cl, al
        xor edi, edi

DE_LOOP2:
        mov al, byte ptr [edx + edi]
        xor ah, ah
        push edx
        mov edx, [esp + 48] ;key_len
        div dl
        pop edx
        push ecx
        mov cl, ah
        inc cl
        mov ah, byte ptr [ebx + esi]
        ror ah, cl
        mov al, byte ptr [edx + edi]
        xor ah, al
        mov byte ptr [ebx + esi], ah
        pop ecx
        inc esi
        inc edi
        loop DE_LOOP2

EXIT:
        pop edi
        pop esi
        pop edx
        pop ecx
        pop ebx
        pop eax
        pop ebp
        mov eax, 1
        ret}}
#endif

bool INL NKD rol_byte_off(unsigned char* pb, unsigned int offset, unsigned char bits)
    {
    __asm {
        push ebx
        push ecx
        push esi
        mov ecx, [esp + 24] ;bits
        cmp cl, 8
        jl L1
        xor eax, eax
        jmp EXIT

L1:
        mov ebx, [esp + 16] ;pb
        mov esi, [esp + 20] ;offset
        mov al, byte ptr [ebx + esi] ;*(pb + offset)
        rol al, cl
        mov byte ptr [ebx + esi], al
        mov eax, 1

EXIT:
        pop esi
        pop ecx
        pop ebx
        ret
        }}

bool INL NKD ror_byte_off(unsigned char* pb, unsigned int offset, unsigned char bits)
    {
    __asm {
        push ebx
        push ecx
        push esi
        mov ecx, [esp + 24] ;bits
        cmp cl, 8
        jl L1
        xor eax, eax
        jmp EXIT

L1:
        mov ebx, [esp + 16] ;pb
        mov esi, [esp + 20] ;offset
        mov al, byte ptr [ebx + esi] ;*(pb + offset)
        ror al, cl
        mov byte ptr [ebx + esi], al
        mov eax, 1

EXIT:
        pop esi
        pop ecx
        pop ebx
        ret
        }}

bool INL NKD xor_byte_off(unsigned char* pb, unsigned int offset, unsigned char mask)
    { // xor a byte from a pb and a offset
    __asm {
        push ebx
        push ecx
        push esi
        mov ebx, [esp + 16] ;pb
        mov esi, [esp + 20] ;offset
        mov al, byte ptr [ebx + esi] ;*(pb + offset)
        mov ecx, [esp + 24] ;mask
        xor al, cl
        mov byte ptr [ebx + esi], al
        pop esi
        pop ecx
        pop ebx
        mov eax, 1
        ret
        }}


#if 1
bool encrypt_B(char* src, unsigned int src_len, char* key,
               unsigned int key_len, bool en /* = true */)
    { // 'B' encrypt/decrypt algorithm
    unsigned int loop = src_len / key_len;
    unsigned int rcnt = src_len % key_len;
    char* p = NULL;
    unsigned int i, j;

    if (en)
        {
        p = src;
        for (j = 0; j < loop; ++ j)
            for (i = 0; i < key_len; ++ i)
                {
                xor_byte_off((unsigned char*)p, j * key_len + i, key[i]);
                rol_byte_off((unsigned char*)p, j * key_len + i, key[i] % key_len + 1);
                }
            for (i = 0; i < rcnt; ++ i)
                {
                xor_byte_off((unsigned char*)p, loop * key_len + i, key[i]);
                rol_byte_off((unsigned char*)p, loop * key_len + i, key[i] % key_len + 1);
                }}
    else{
        p = src;
        for (j = 0; j < loop; ++ j)
            for (i = 0; i < key_len; ++ i)
                {
                ror_byte_off((unsigned char*)p, j * key_len + i, key[i] % key_len + 1);
                xor_byte_off((unsigned char*)p, j * key_len + i, key[i]);}
            for (i = 0; i < rcnt; ++ i)
                {
                ror_byte_off((unsigned char*)p, loop * key_len + i, key[i] % key_len + 1);
                xor_byte_off((unsigned char*)p, loop * key_len + i, key[i]);}}
    return true;}
#endif

#if 0
bool encrypt_B(char* src, unsigned int src_len, char* key,
               unsigned int key_len, bool en /* = true */)
    {
    for (unsigned int i = 0; i < src_len; ++ i) src[i] ^= 0x58;
    return true;}
#endif

// 
static char const ip_table[64] =
    {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};
static char const ipr_table[64] =
    {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25};
static char const e_table[48] =
    {
    32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1};
static char const p_table[32] =
    {
    16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
    2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25};
static char const pc1_table[56] =
    {
    57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4};
static char const pc2_table[48] =
    {
    14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
static char const loop_table[16] =
    {
    1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
static char const s_box[8][4][16] =
    {
    14,	 4,	13,	 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
    0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
    4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,

    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
    3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
    0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,

    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
    1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,

    7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
    3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,

    2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
    4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,

    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
    9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
    4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,

    4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
    1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
    6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,

    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
    1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
    7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
    2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};

typedef bool (*psubkey)[16][48];
static bool sub_key[2][16][48];
static bool is_3A;
static char tmp[256], des_key[16];
void transform(bool* out, bool* in, char const* table, int len)
    {
    for (int i = 0; i < len; ++ i)
        tmp[i] = in[table[i] - 1];
    memcpy(out, tmp, len);}
void xor_bool(bool* inA, bool const* inB, int len)
    {
    for (int i = 0; i < len; ++ i)
        inA[i] ^= inB[i];}
void rotate_L(bool* in, int len, int loop)
    {
    memcpy(tmp, in, loop);
    memcpy(in, in + loop, len - loop);
    memcpy(in + len - loop, tmp, loop);}
void byte2bit(bool* out, char const* in, int bits)
    {
    for (int i = 0; i < bits; ++ i)
        out[i] = (in[i >> 3] >> (i & 7)) & 1;
    }
void bit2byte(char* out, const bool* in, int bits)
    {
    memset(out, 0, bits >> 3);
    for (int i = 0; i < bits; ++ i)
        out[i >> 3] |= in[i] << (i & 7);
    }
void set_sub_key(psubkey pSubkey, char const key[8])
    {
    static bool k[64], *kl=&k[0], *kr=&k[28];
    byte2bit(k, key, 64);
    transform(k, k, pc1_table, 56);
    for (int i = 0; i < 16; ++ i)
        {
        rotate_L(kl, 28, loop_table[i]);
        rotate_L(kr, 28, loop_table[i]);
        transform((*pSubkey)[i], k, pc2_table, 48);}
    }
void set_key(char const* key, int len)
    {
    memset(des_key, 0, 16);
    memcpy(des_key, key, (len > 16) ? 16 : len);
    set_sub_key(&sub_key[0], &des_key[0]);
    is_3A = (len > 8) ? (set_sub_key(&sub_key[1], &des_key[8]), true) : false;
    }
void S_func(bool out[32], bool const in[48])
    {
    for (char i = 0, j, k; i < 8; ++ i, in += 6, out += 4)
        {
        j = (in[0] << 1) + in[5];
        k = (in[1] << 3) + (in[2] << 2) + (in[3] << 1) + in[4];
        byte2bit(out, &s_box[i][j][k], 4);}
    }
void F_func(bool in[32], bool const ki[48])
    {
    static bool mr[48];
    transform(mr, in, e_table, 48);
    xor_bool(mr, ki, 48); S_func(in, mr);
    transform(in, in, p_table, 32);}
void base_A(char out[8], char in[8], psubkey const pSubKey, bool encrypt)
    {
    static bool m[64], tmp[32], *li=&m[0], *ri=&m[32];
    byte2bit(m, in, 64);
    transform(m, m, ip_table, 64);
    if (encrypt)
        {
        for (int i = 0; i < 16; ++ i)
            {
            memcpy(tmp, ri, 32);
            F_func(ri, (*pSubKey)[i]);
            xor_bool(ri, li, 32);
            memcpy(li, tmp, 32);}
        }
    else {
        for (int i = 15; i >= 0; -- i)
            {
            memcpy(tmp, li, 32);
            F_func(li, (*pSubKey)[i]);
            xor_bool(li, ri, 32);
            memcpy(ri, tmp, 32);}
        }
    transform(m, m, ipr_table, 64);
    bit2byte(out, m, 64);}
bool algo_A(char* out, char* in, long data_len, char const* key,
            int key_len, bool encrypt)
    {
    if (!(out && in && key && (data_len = (data_len + 7) & 0xfffffff8)))
        return false;
    set_key(key, key_len);
    if (!is_3A)
        {
        for (long i = 0, j = data_len >> 3; i < j; ++ i, out += 8, in += 8)
            base_A(out, in, &sub_key[0], encrypt);
        }
    else {
        for (long i = 0, j = data_len >> 3; i < j; ++ i, out += 8, in += 8)
            {
            base_A(out, in,  &sub_key[0], encrypt);
            base_A(out, out, &sub_key[1], !encrypt);
            base_A(out, out, &sub_key[0], encrypt);}
        }
    return true;}
bool encrypt_A(char* out, char* in, long data_len, char const* key,
               int key_len, bool en /* = true */)
    {return algo_A(out, in, data_len, key, key_len, en);}


//////////////////////////////////////////////////////////////////////////
//
// Message Digest 5
//
void md5(char const* msg, unsigned char dig[16])
{
    MD5_CTX context;
    unsigned int len = (unsigned int)strlen(msg);

    MD5Init(&context);
    MD5Update(&context, (unsigned char*)msg, len);
    MD5Final(dig, &context);
}

void md5string(char const* msg, char str[33])
{
    unsigned char dig[16];
    md5(msg, dig);

    char tmp[3];
    str[0] = 0, str[32] = 0;
    for (int i = 0; i < 16; ++ i)
    {
        sprintf(tmp, "%02X", dig[i]);
        strcat(str, tmp);
    }
}
