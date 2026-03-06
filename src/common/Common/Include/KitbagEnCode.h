#pragma once

char g_key[9] = "19800216";
int Decrypt(char* buf, int len, const char* enc, int elen)
{
	int i;
	for(i = 0;i<elen;i++)
	{
		buf[i] = enc[i] - g_key[i%8];
	}
	return i;
}

int Encrypt(char* buf, int len, const char* pwd, int plen)
{
	int i;
	for(i = 0;i<plen;i++)
	{
		buf[i] = pwd[i] + g_key[i%8];
	}
	return i;
}
