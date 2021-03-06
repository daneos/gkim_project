/**
 * @package LZ77 encoder/decoder
 * @file Declaration file
 * Projekt GKIM
 * Grzegorz Kowalski
 * Bartosz Zielnik
 * Piotr Mańkowski
 * Dariusz Szyszlak
 * version 1 | 01.2016
 */

#if !defined(__LZ77_H__)
#	define __LZ77_H___

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bmptool.h"

#define LZ77_DEBUG_MODE		0

/**
 * @struct Symbol used in encoding
 */ 
typedef struct _symbol {
	uint8_t P;		//!< Position from which symbol starts
	uint8_t C;		//!< Number of bytes
	uint8_t S;		//!< Next symbol
} symbol;

uint8_t *prepare(uint8_t *buf, int size, int k, int n);
int lz77_compress(uint8_t *in, uint8_t *out, int size, int k, int n);
void lz77_decompress(uint8_t *in, uint8_t *out, int csize, int size, int k);

#endif /* __LZ77_H___ */