#include<iostream>
#include<fstream>
#include"huffman.h"

using namespace std;

/**
 * Compress bmp to huff file
 * @param  new_bmp structure which holds BMP essentials
 * @return true if all done correctly
 */
bool huffman_encoding(conv_bmp* new_bmp)
{
    int counter=-1;                   //counter for the number of colors
    int colors_frequency[4096]={0};     //frequency of particular colors
    SDL_Color temporary_color;        //temporary variable of colors
    SDL_Color temporary_array[4096];
    bool test;

    for(int y=0; y<new_bmp->height; y++){
        for(int x=0; x<new_bmp->width; x++)
        {
            temporary_color.r=new_bmp->red_color[y][x];
            temporary_color.g=new_bmp->green_color[y][x];
            temporary_color.b=new_bmp->blue_color[y][x];

            test=true;

            if(counter==-1)
            {
                temporary_array[0]=temporary_color;
                colors_frequency[0]++;
                counter=0;
                test=false;
            }
            else
            {
                for(int i=0; i<=counter; i++)
                {
                    if(temporary_color.r == temporary_array[i].r && temporary_color.g == temporary_array[i].g && temporary_color.b == temporary_array[i].b )
                    {
                        colors_frequency[i]+=1;
                        test=false;
                    }
                }
            }
            if(test == true)
            {
                temporary_array[counter+1]=temporary_color;
                colors_frequency[counter+1]+=1;
                counter+=1;
            }
        }
        }

PriorityQueueFreq Queue;

for(int i=0; i<=counter; i++)
{
        Node * p=new Node;
        p->parent=NULL;
        p->left=NULL;
        p->right=NULL;
        p->frequency=colors_frequency[i];
        p->key=i;
        Queue.push(p);
        colors_frequency[i]=0;
}

    Uint8 c[4096]={0};
    Node * temp_node1;
    Node * temp_node2;
    Node * new_root;
    int temp_frequency1=0;
    int temp_frequency2=0;
    dictionary* elements_of_dictionary=new dictionary[counter+1];

    while(!Queue.empty())
    {
        temp_node1=Queue.top();
        Queue.pop();
        temp_node2=Queue.top();
        Queue.pop();
        new_root=new Node;
        new_root->parent=NULL;
        new_root->frequency = temp_node1->frequency+temp_node2->frequency;
        new_root->left = temp_node1;
        new_root->right= temp_node2;
        if(Queue.empty())break;
        Queue.push(new_root);
    }

    inorder(new_root,c,0,elements_of_dictionary);

    for(int i=0; i<=counter; i++)
    {
        elements_of_dictionary[i].colors.r=temporary_array[i].r;
        elements_of_dictionary[i].colors.g=temporary_array[i].g;
        elements_of_dictionary[i].colors.b=temporary_array[i].b;
    }
 cout<<"Slownik do kodowania/dekodowania huffmana: " << endl;
    for(int i=0; i<=counter; i++)
    {
        cout << elements_of_dictionary[i].key <<"." ;
        cout <<"\tR- "<< (int)elements_of_dictionary[i].colors.r ;
        cout <<"\tG- "<< (int)elements_of_dictionary[i].colors.g ;
        cout <<"\tB- "<< (int)elements_of_dictionary[i].colors.b ;
         cout <<"\tD-" <<(int)elements_of_dictionary[i].code_length;
        cout <<"\tkod - ";
         for(int j=0; j<elements_of_dictionary[i].code_length; j++)
        cout << (int)elements_of_dictionary[i].huffmancode[j];

        cout <<endl;
    }
    int maximum=0;
    for(int i=0; i<=counter; i++)
    {
        if(maximum <elements_of_dictionary[i].code_length)
        {
            maximum=elements_of_dictionary[i].code_length;
        }
    }
    cout<<maximum<<endl;
    ofstream file;
    file.open("encoded.huff", ios::binary);
    Uint16 dictionary_size=0;
    Uint16  longest_code=0;
    dictionary_size=dictionary_size|(counter+1);
    longest_code=longest_code | maximum;
    file.write((const char *) &dictionary_size,sizeof(dictionary_size));
    file.write((const char *) &longest_code,sizeof(longest_code));
    Uint16 bmp_height;
    Uint16 bmp_width;
    bmp_height=new_bmp->height;
    bmp_width=new_bmp->width;
    file.write((const char*)&bmp_height,sizeof(Uint16));
    file.write((const char*)&bmp_width,sizeof(Uint16));
    Uint8 huffman_code=0;
    Uint8 length=0;
    for(int i=0; i<=counter; i++)
    {
        file.write((const char *) & elements_of_dictionary[i].colors.r,sizeof(Uint8));
        file.write((const char *) & elements_of_dictionary[i].colors.g,sizeof(Uint8));
        file.write((const char *) & elements_of_dictionary[i].colors.b,sizeof(Uint8));
        length=elements_of_dictionary[i].code_length|length;
        file.write((const char *) & length,sizeof(length));
        length=0;
       for(int j=0; j<elements_of_dictionary[i].code_length; j++)
        {
            huffman_code<<1;
            huffman_code=huffman_code|elements_of_dictionary[i].huffmancode[j];
            file.write((const char *)&huffman_code,sizeof(huffman_code));
            huffman_code=0;
        }
    }
    int counter2=0;
    SDL_Color temporary1;
    Uint8 temp1=0;

     for(int y=0; y<new_bmp->height;y++)
        {
       for(int x=0; x<new_bmp->width; x++)
        {
          temporary1.r=new_bmp->red_color[y][x];
          temporary1.g=new_bmp->green_color[y][x];
          temporary1.b=new_bmp->blue_color[y][x];
           for(int i=0; i<=counter;i++)
           {
               if(elements_of_dictionary[i].colors.r ==  temporary1.r && elements_of_dictionary[i].colors.g ==  temporary1.g  && elements_of_dictionary[i].colors.b ==  temporary1.b  )
                    {
                         for(int j=0; j<elements_of_dictionary[i].code_length; j++)
                            {
                                        if(counter2<8)
                                        {
                                        counter2++;
                                        temp1= temp1<<1;
                                        temp1=temp1 | elements_of_dictionary[i].huffmancode[j];
                                        }
                                        else
                                        {
                                           file.write((const char *)&temp1,sizeof(Uint8));
                                           temp1=0;
                                           counter2=1;
                                           temp1=temp1 | elements_of_dictionary[i].huffmancode[j];
                                        }
                            }
                   }

           }
        }
        }
        file.close();
}

/**
 * Deompress huff file to bmp
 * @param  new_bmp structure for BMP essentials from huff file
 * @return true if all done correctly
 */
bool huffman_decoding(conv_bmp* new_bmp)
{
   Uint16 dictionary_size=0;
   Uint16 the_longest_code=0;
   Uint8 code_huff=0;
   Uint8 code_length=0;
   cout<<"Odczytano z pliku:"<<endl;
   ifstream file;
   file.open("encoded.huff",ios::binary);
   file.read((char *) & dictionary_size ,sizeof(Uint16));
   file.read((char *) & the_longest_code ,sizeof(Uint16));
   dictionary *elements_of_dictionary=new dictionary[dictionary_size];

   Uint16 bmp_heigth;
   Uint16 bmp_width;

   file.read(( char*) & bmp_heigth,sizeof(Uint16));
   file.read(( char*) & bmp_width,sizeof(Uint16));

   new_bmp->height=bmp_heigth;
   new_bmp->width=bmp_width;

   for(int i=0; i<dictionary_size; i++)
   {
       elements_of_dictionary[i].key=i;
       file.read((char *)&elements_of_dictionary[i].colors.r,sizeof(Uint8));
       file.read((char *)&elements_of_dictionary[i].colors.g,sizeof(Uint8));
       file.read((char *)&elements_of_dictionary[i].colors.b,sizeof(Uint8));
       file.read((char *) & code_length,sizeof(Uint8));

       elements_of_dictionary[i].code_length=code_length;

        for(int j=0; j<elements_of_dictionary[i].code_length; j++)
        {
             file.read((char *) & code_huff,sizeof(Uint8));
             elements_of_dictionary[i].huffmancode[j]=code_huff;
             code_huff=0;
        }
   }
    cout<<dictionary_size<<endl;
   for(int i=0; i<dictionary_size; i++)
    {
        cout << elements_of_dictionary[i].key <<"." ;
        cout <<"\tR- "<< (int)elements_of_dictionary[i].colors.r ;
        cout <<"\tG- "<< (int)elements_of_dictionary[i].colors.g ;
        cout <<"\tB- "<< (int)elements_of_dictionary[i].colors.b ;
        cout <<"\tD-" <<(int)elements_of_dictionary[i].code_length;
        cout <<"\tkod - ";
        for(int j=0; j<elements_of_dictionary[i].code_length; j++)
         cout << (int)elements_of_dictionary[i].huffmancode[j];

        cout <<endl;
    }
    Uint8 *search_array = new Uint8[the_longest_code];
    code_huff=0;
    int code_length_search=1;
    int height=0;
    int width=0;
    int index=0;
    Uint8 mask=0;


    while(!file.eof())
        {

       file.read((char *) & code_huff,sizeof(Uint8));

       for(int i=7; i>=0; i--)
       {

            mask=1;
            mask=mask<<i;
            mask = code_huff & mask;
            mask= mask>>i;

            search_array[code_length_search-1]=mask;

            index=search_dictionary(search_array,elements_of_dictionary,code_length_search,dictionary_size);
            if(index>=0)
            {
                if(height<new_bmp->height){
                 new_bmp->red_color[height][width]=elements_of_dictionary[index].colors.r;
                 new_bmp->green_color[height][width]=elements_of_dictionary[index].colors.g;
                 new_bmp->blue_color[height][width]=elements_of_dictionary[index].colors.b;
                }
                for(int c=0; c<=the_longest_code; c++)
                {
                    search_array[c]=0;
                }
                code_length_search=1;

                width++;
                if(width==new_bmp->width)
                {
                    height++;
                    width=0;
                }

            }
            else
            {
            code_length_search++;
            }
       }
        }
        file.close();
}
/**
 * Build huffman code for particular element of dictionary
 * @param  Node for huffman tree
 * @param  Array c to build huffman tree
 * @param  length of huffman code
 * @param  dictionary
 *
 */
 void inorder(Node * n, Uint8 *c, int length ,dictionary *tab)
{
  if(!(n->left))
  {
    for(int i = 0; i <length; i++)
     {
           tab[n->key].huffmancode[i]=c[i];

     }
    tab[n->key].code_length=length;
    tab[n->key].key=n->key;
  }
  else
  {
    c[length] = 0; inorder(n->left,c,length + 1,tab);
    c[length] = 1; inorder(n->right,c,length + 1,tab);
  }
}
/**
 * Search  code in dictionary and fit it to coded pixel
 * @param  array with code for search
 * @param  array with elements of dictionary
 * @param  length of huffman code for search
 * @param  dictionary size
 * @return index element of dictionary of particular pixel
 */
int search_dictionary(Uint8 *search_code, dictionary *elements_dictionary, int code_length, int dictionary_size)
{
    bool flag=true;
    int dictionary_index=-1;
    for(int i=0; i<dictionary_size; i++)
    {
        flag=true;
       if(elements_dictionary[i].code_length==code_length)
       {
            for(int j=0; j<code_length; j++)
            {
                if(elements_dictionary[i].huffmancode[j]!=search_code[j])
                {
                    flag=false;
                }

            }
            if(flag)
            {
                dictionary_index=i;
                break;

            }
       }
    }
     return dictionary_index;
}
