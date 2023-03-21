#include "AdptArray.h"
#include "book.h"
#include "Person.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct PAdptArray
{
   COPY_FUNC copy_func;
   DEL_FUNC del_func;
   PRINT_FUNC print_func;
   int length;    // Number of elements
   int size;      // Size in memory
   int *indexarr; // Array that keeps all used inexes
   PElement *arr;
} PAdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func)
{
   struct PAdptArray parr;
   parr.copy_func = copy_func;
   parr.length = 0;
   parr.size = 20;
   parr.indexarr = malloc(sizeof(int));
   parr.arr = malloc(20 * sizeof(PElement)); // Starting size of 20
   // if(sizeof(parr.arr) != 20*sizeof(PElement)){
   //    return  NULL;
   // }
   return parr;
}
void DeleteAdptArray(PAdptArray parr)
{
   for (int i = 0; i < parr.length; i++)
   {
      parr.del_func(parr.arr[parr.indexarr[i]]); // Delete the element at the index kept by the index array at i
      free(parr.arr[i]);
      free(parr.indexarr[i]);
   }
   free(parr.arr);
   free(parr.copy_func);
   free(parr.del_func);
   free(parr.indexarr);
   free(parr.print_func);
   free(&parr);
}
int Index_In_Parr(PAdptArray parr, int index){ // Function used to check if a certain index is occupied in parr, and if so to return its index in indexarr
   int res = -1;
   for(int i=0; i< parr.length; i++){
      if(index == parr.indexarr[i]){
         res = i;
      }
   }
   return res;
}

Result SetAdptArrayAt(PAdptArray parr, int index, PElement pel){
   int index_in_parr = Index_In_Parr(parr, index);
   PElement new_pel = &pel;
   if(index_in_parr == -1){ // Index is not used in parr
      parr.length++;
      parr.indexarr = realloc(parr.indexarr, parr.length * sizeof(int));
      parr.indexarr[parr.length] = index;
      if(index > parr.size){
         parr.size = index;
         parr.arr = realloc(parr.arr, index * sizeof(PElement));
      }
      parr.arr[index] = new_pel;
   }
   else{ // Index is used in parr
      parr.indexarr[index_in_parr] = index;
      parr.del_func(parr.arr[index_in_parr]);
      parr.arr[index] = new_pel;
   }
}