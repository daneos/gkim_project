/**
 * @package LZ77 encoder/decoder
 * @file Implementation file
 * Projekt GKIM
 * Grzegorz Kowalski
 * Bartosz Zielnik
 * Piotr Mańkowski
 * Dariusz Szyszlak
 * version 1 | 01.2016
 */

#include "lz77.h"

/**
 * Write symbol to an output and move indexes
 * @param buf output array
 * @param i pointer to output index
 * @param s symbol to write
 */
void output(uint8_t *buf, int *i, symbol s)
{
	buf[*i] = s.P;
	buf[(*i)+1] = s.C;
	buf[(*i)+2] = s.S;
	*i += 3;
}

/**
 * Search for next symbol
 * @param buf input array
 * @param i current input index
 * @param k dictionary size
 * @param n lookahead buffer size
 * @return found symbol
 */
symbol search(uint8_t *buf, int i, int k, int n)
{
	symbol s = {0,0,0};
	uint8_t *dict = buf+i;
	uint8_t *lookahead = buf+i+k;

	int len = 0;
	int max = 0;
	int max_si = 0;
	for(int si=0; si < k; si++)
	{
		if(dict[si] == lookahead[len])
		{
			for(; dict[si+len] == lookahead[len] && len < n; len++);
			if(len > max)
			{
				max = len;
				max_si = si;
			}
			len = 0;
		}
	}

	s.P = max_si;
	s.C = max;
	s.S = lookahead[max];

	return s;
}

/**
 * Prepare buffer (extend to match parameters)
 * @param buf buffer to extend
 * @param size amount of data
 * @param k dictionary size
 * @param n lookahead buffer size
 * @return extended array
 */
uint8_t *prepare(uint8_t *buf, int size, int k, int n)
{
	uint8_t *ebuf = malloc((size+k+n)*sizeof(uint8_t));
	memset(ebuf, buf[0], k);			// copy first symbol through entire dictionary
	memcpy(ebuf+k, buf, size);		// copy input after dict
	memset(ebuf+k+size, 0, n);		// fill additional space with 0s
	return ebuf;
}

/**
 * Free memory used by an extended buffer
 * @param buf buffer
 */
void clean(uint8_t *buf)
{
	free(buf);
}

/**
 * Print fragment of a buffer
 * @param buf buffer
 * @param start starting index
 * @param end ending index
 */
void printpart(uint8_t *buf, int start, int end)
{
	for(int i = start; i < end; i++)
		printf("%3d, ", buf[i]);
}

/**
 * LZ77 Compressor function
 * @param in input array
 * @param out output array
 * @param size amount of input data
 * @param k dictionary size
 * @param n lookahead buffer size
 * @return length of compressed data
 */
int lz77_compress(uint8_t *in, uint8_t *out, int size, int k, int n)
{
	in = prepare(in, size, k, n);		// adjust buffers
	int iindex = 1;						// initialize indexes
	int oindex = 1;
	out[0] = in[0];						// output first symbol

	while(iindex < size)				// while there are more symbols (+n)
	{
		symbol s = search(in, iindex, k, n);		// find next symbol

		if(LZ77_DEBUG_MODE)
		{
			// print debug information in format
			// [ dictionary ] | [ lookahead ] | [ symbol ]
			printpart(in, iindex, iindex+k);
			printf(" | ");
			printpart(in, iindex+k, iindex+k+n);
			printf(" | P=%3d  C=%3d  S=%3d\n", s.P, s.C, s.S);
		}

		output(out, &oindex, s);					// output found symbol
		iindex += s.C+1;							// move working window
	}

	clean(in);							// clean buffers
	return oindex;						// return size of compressed data
}

/**
 * LZ77 Decompressor function
 * @param in input array
 * @param out output array
 * @param csize amount of compressed data
 * @param size amount of uncompressed data
 * @param k dictionary size
 */
void lz77_decompress(uint8_t *in, uint8_t *out, int csize, int size, int k)
{
	out[0] = in[0];						// output first symbol
	uint8_t *dict = prepare(out, size, k, 0);	// prepare working window
	int iindex = 1;						// initialize indexes
	int oindex = 1;

	while(iindex < csize)
	{
		symbol s = { in[iindex], in[iindex+1], in[iindex+2] };		// get symbol

		if(LZ77_DEBUG_MODE)
		{
			// print debug information in format
			// [ dictionary ] | [ symbol ]
			printpart(dict, oindex, oindex+k);
			printf(" | P=%3d  C=%3d  S=%3d\n", s.P, s.C, s.S);
		}

		for(int i=0; i < s.C; i++, oindex++)		// copy data from dictionary
			dict[k+oindex] = dict[s.P+oindex];

		dict[k+oindex] = s.S;		// append next symbol
		oindex++;					// move indexes
		iindex += 3;
	}

	memcpy(out, dict+k, size);		// copy data from dict to output
}