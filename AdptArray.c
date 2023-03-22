#include "AdptArray.h"
#include "book.h"
#include "Person.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


typedef struct AdptArray_ *PAdptArray;

struct AdptArray_
{
   COPY_FUNC copy_func;
   DEL_FUNC del_func;
   PRINT_FUNC print_func;
   int length;    // Number of elements
   int size;      // Size in memory
   int *indexarr; // Array that keeps all used indexes
   PElement *arr;
};

PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func)
{
   struct AdptArray_ *parr = malloc(sizeof(struct AdptArray_*));
   printf("HERE\n");
   parr->print_func = print_func;
   parr->del_func = del_func;
   parr->copy_func = copy_func;
   parr->length = 0;
   parr->size = 1;
   parr->indexarr = malloc(sizeof(int));
   parr->arr = malloc(sizeof(PElement)); // Starting size of 1
   if (parr -> indexarr == NULL || parr -> arr == NULL)
   {
      parr->length = -1;
      parr->size = -1;
      printf("Memory allocation failure\n");
      return NULL;
   }
   return parr;
}
void DeleteAdptArray(PAdptArray parr)
{
   for (int i = 0; i < parr->length; i++)
   {
      parr->del_func(parr->arr[parr->indexarr[i]]); // Delete the element at the index kept by the index array at i
      free(parr->arr[i]);
   }  
   free(parr->arr);
   free(parr->indexarr);
   free(parr);
}
int Index_In_Parr(PAdptArray parr, int index)
{ // Function used to check if a certain index is occupied in parr, and if so to return its index in indexarr
   int res = -1;
   for (int i = 0; i < parr->length; i++)
   {
      if (index == parr->indexarr[i])
      {
         res = i;
      }
   }
   return res;
}

Result SetAdptArrayAt(PAdptArray parr, int index, PElement pel)
{
   int index_in_parr = Index_In_Parr(parr, index);
   PElement new_pel = parr->copy_func(pel);
   if (index_in_parr == -1)
   { // Index is not used in parr
      if (index > parr->size)
      { // Index given is larger than the size of arr
         parr->size = index;
         printf("A\n");
         printf("%ld\n", index * sizeof(PElement));
         parr->arr = realloc(parr->arr, index * sizeof(PElement));
         printf("B\n");
         if (parr->arr == NULL)
         {
            return FAIL;
         }
      }
      parr->arr[index] = new_pel;
      parr->length++;
      printf("parr length: %d\n", parr->length);
      parr->indexarr = realloc(parr->indexarr, sizeof(int));
      printf("D\n");
      if (parr->indexarr == NULL)
      {
         return FAIL;
      }
      parr->indexarr[parr->length] = index;
      printf("Added to index array\n");
   }
   else
   { // Index is used in parr
      parr->indexarr[index_in_parr] = index;
      parr->del_func(parr->arr[index]);
      parr->arr[index] = new_pel;
   }
   return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray parr, int index)
{
   if (Index_In_Parr(parr, index) == -1)
   {
      return NULL;
   }
   return parr->copy_func(parr->arr[index]);
}

int GetAdptArraySize(PAdptArray parr)
{
   return parr->length;
}

void PrintDB(PAdptArray parr)
{
   for (int i = 0; i < parr->length; i++)
   {
      parr->print_func(parr->arr[parr->indexarr[i]]);
   }
}
