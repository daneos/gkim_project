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
struct dictionary{
    int key;
    Uint8 huffmancode[4096];
    int code_length;
    SDL_Color colors;

};
//two function for huffman algorithm
/**
 * Compress bmp to huff file
 * @param  new_bmp structure which holds BMP essentials
 * @return true if all done correctly
 */
bool huffman_encoding(const conv_bmp* new_bmp);
/**
 * Deompress huff file to bmp
 * @param  new_bmp structure for BMP essentials from huff file
 * @return true if all done correctly
 */
bool  huffman_decoding(conv_bmp* new_bmp);
/**
 * Build huffman code for particular element of dictionary
 * @param  Node for huffman tree
 * @param  Array c to build huffman tree
 * @param  length of huffman code
 * @param  dictionary
 *
 */
void inorder(Node * n,Uint8 *c, int lenght, dictionary *tab);
/**
 * Search  code in dictionary and fit it to coded pixel
 * @param  array with code for search
 * @param  array with elements of dictionary
 * @param  length of huffman code for search
 * @param  dictionary size
 * @return index element of dictionary of particular pixel
 */
int search_dictionary(const Uint8 *search_code,const dictionary *elements_dictionary, int code_length, int dictionary_size);
/**
 * Delete dynamic allocated memory of huffman tree
 * @param Root node of huffman tree
 */
void postorder_node_delete(Node* n);
#endif  //HUFFMAN_H_INCLUDED
