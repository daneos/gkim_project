/**
 * @package huffman encoder/decoder
 * @file Declaration file
 * Projekt GKIM
 * Grzegorz Kowalski
 * Bartosz Zielnik
 * Piotr Mańkowski
 * Dariusz Szyszlak
 * version 1 | 01.2016
 */

#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED

#include <queue>
#include "bmptool.h"
#include <sys/types.h>
#include <SDL/SDL.h>

//nodes for huffman tree
struct Node
{
	Node * parent, * left, * right;
	int frequency;
	int key;
};

struct compare_frequency
{
	bool operator() (const Node* nod1, const Node* nod2)
	{
		return nod1->frequency > nod2->frequency;
	}
};

typedef std::priority_queue < Node*, std::vector < Node* >, compare_frequency > PriorityQueueFreq;

//dictionary for huffman encoding/decoding
struct dictionary {
	int key;
	Uint8 huffmancode[4096];
	int code_length;
	SDL_Color colors;
};

int huffman_encoding(const conv_bmp* new_bmp,Uint8* out,Uint16  &dictionary_size,Uint16 &longest_code,dictionary * elements_of_dictionary);
conv_bmp* huffman_decoding(Uint16 bmp_height ,Uint16 bmp_width, Uint16 dictionary_size,Uint16 the_longest_code,dictionary *elements_of_dictionary, Uint8 *in, int csize);
void inorder(Node * n,Uint8 *c, int lenght, dictionary *tab);
int search_dictionary(const Uint8 *search_code,const dictionary *elements_dictionary, int code_length, int dictionary_size);
void postorder_node_delete(Node* n);

#endif  //HUFFMAN_H_INCLUDED
