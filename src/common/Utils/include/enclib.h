#pragma once


////////////////////////////////////////
//
//      /
//      by jampe
//      2006/5/26
//
////////////////////////////////////////

#if !defined __ENC_LIB__
#define __ENC_LIB__


typedef unsigned char       __byte;


//  
//  key 8
extern int SetEncKey(const __byte* key);


//  
//  buf 
//  len 
//  pwd 
//  plen 
//  
extern int Encrypt(__byte* buf, int len, const __byte* pwd, int plen);


//  
//  buf 
//  len 
//  enc 
//  elen 
//  
extern int Decrypt(__byte* buf, int len, const __byte* enc, int elen);


#endif  /*__ENC_LIB__*/
