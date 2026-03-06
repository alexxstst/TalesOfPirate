#pragma once


#ifndef _ALGO_H_
#define _ALGO_H_

int base64(char const* src, unsigned int src_len, char* dst,
           unsigned int dst_max_len, unsigned int* dst_len);
int ibase64(char const* src, unsigned int src_len, char* dst, unsigned int* dst_len);
unsigned char const* sha1(unsigned char const* msg,unsigned int len, unsigned char* md);

bool encrypt_A(char* out, char* in, long data_len, char const* key, int key_len, bool en = true);
bool encrypt_B(char* src, unsigned int src_len, char* key, unsigned int key_len, bool en = true);

bool dbpswd_out(char const* ctxt, int ctxt_len, std::string& pswd);
bool dbpswd_in(char const* pswd, int pswd_len, std::string& ctxt);

void md5(char const* msg, unsigned char dig[16]);
void md5string(char const* msg, char str[33]);


#endif
