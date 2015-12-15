#ifndef BASKER_SSWRAPER_HPP
#define BASKER_SSWRAPPER_HPP

#include "basker_types.hpp"

#ifdef HAVE_AMESOS
#include "amesos_btf_decl.h"
#include "amesos_amd.h"
#endif


namespace BaskerNS
{

  template<class Int>
  class BaskerSSWrapper
  {
  public:
    
    //================Strong Connected==================//

    static 
    inline
    int my_strong_component
    (
     Int           &n,
     Int           *col_ptr,
     Int           *row_idx,
     Int           &nblks,
     Int           *perm,
     Int           *perm_in,
     Int           *CC
     )
    {
      return -1;
    }//end strong_component

    static
    inline
    int amd_order
    (
     Int          n,
     Int       *col_ptr,
     Int       *row_ptr,
     Int       *p
     )
    {
      return -1;
    }

   
  }; //end BaskerSSWrapper template <Int>

  template <>
  class BaskerSSWrapper <int>
  {
  public:

    //=========strong componenet===========
    static 
    inline
    int my_strong_component 
    (
     int           &n,
     int           *col_ptr,
     int           *row_idx,
     int           &nblks,
     int           *perm,
     int           *perm_in,
     int           *CC
     )
    {
      typedef int l_Int;
      
      l_Int *p = new l_Int[n];
      l_Int *r = new l_Int[n+1];
      //We will want to add option to use q in the future
      
      l_Int *work = new l_Int[n*4];
      
      //printf("before amesos call \n");
      /*
        nblks = amesos_btf_strongcomp(M.ncol,&(M.col_ptr[0]),
        &(M.row_idx[0]), 
        &(perm_in[0]), p, r, work);
      */
      nblks = amesos_btf_strongcomp(n, col_ptr,
				    row_idx, 
				    perm_in, p, r, work);
      //printf("after amesos call \n");
      
      
      
#ifdef BASKER_DEBUG_ORDER_BTF
      
      printf("\nBTF perm: \n");
      //for(Int i=0; i <M.nrow; i++)
      for(Int i=0; i < n; i++)
        {
          printf("%d, ", p[i]);
        }
      
      printf("\n\nBTF tabs: <right> \n");
      for(Int i=0; i < nblks+1; i++)
        {
          printf("%d, ", r[i]);
        }
      printf("\n");
#endif
      
      //BASKER_ASSERT(M.nrow > 0, "M.nrow btf");
      BASKER_ASSERT(n > 0, "M.nrow btf");
      //MALLOC_INT_1DARRAY(perm,M.nrow);
      // MALLOC_INT_1DARRAY(perm, n);
      for(l_Int i = 0; i < n; i++)
        {
          perm[p[i]] = i;
        }
      BASKER_ASSERT((nblks+1) > 0, "nblks+1 btf");
      //MALLOC_INT_1DARRAY(CC, nblks+1);
      for(l_Int i = 0; i < nblks+1; i++)
        {
          CC[i] = r[i];
        }
      

      delete [] p;
      delete [] r;
      delete [] work;

      return 0;
    }

    //=================amd=============

    static
    inline
    int amd_order
    (
     int n, 
     int *col_ptr,
     int *row_idx,
     int *p
     )
    {
      double Info[AMD_INFO];
      
      for(int i = 0; i < AMD_INFO; ++i)
	{Info[i] = 0;}
	

      //printf("n: %d \n", n);
      int ret = amesos_amd_order(n, col_ptr, row_idx, p, NULL, Info); 

      //if(ret == AMD_OK)
      //printf("OK\n");
      if(ret == AMD_OUT_OF_MEMORY)
	printf("Memory \n");
      if(ret == AMD_INVALID)
	printf("Invalid\n");
      if(ret == AMD_OK_BUT_JUMBLED)
	printf("Jumbled\n");


      return 0;
    }


  }; //end BaskerSSWraper template <int>

  template <>
  class BaskerSSWrapper <long>
  {
  public:
    static 
    inline
    int my_strong_component 
    (   
     long           &n,
     long           *col_ptr,
     long          *row_idx,
     long           &nblks,
     long           *perm,
     long           *perm_in,
     long          *CC
        )
    {
      typedef long  l_Int;
      
      //l_Int p[n]; //output row_per
      l_Int *p = new l_Int[n];
      //l_Int r[n+1]; //comp_tabs
      l_Int *r = new l_Int[n+1];
      //We will want to add option to use q in the future
      
      //l_Int work[n*4];
      l_Int *work = new l_Int[n*4];
      
      //printf("before amesos call \n");
      /*
        nblks = amesos_btf_l_strongcomp(M.ncol,&(M.col_ptr[0]),
        &(M.row_idx[0]), 
        &(perm_in[0]), p, r, work);
      */
      nblks = amesos_btf_l_strongcomp(n,
                                      col_ptr,
                                      row_idx, 
                                      perm_in, p, r, work);
      //printf("after amesos call \n");
      

      
#ifdef BASKER_DEBUG_ORDER_BTF
      
      printf("\nBTF perm: \n");
    for(Int i=0; i <n; i++)
      {
	printf("%d, ", p[i]);
      }
    
    printf("\n\nBTF tabs: <right> \n");
    for(l_Int i=0; i < nblks+1; i++)
      {
	printf("%d, ", r[i]);
      }
    printf("\n");
#endif

    BASKER_ASSERT(n > 0, "M.nrow btf");
    //MALLOC_INT_1DARRAY(perm,M.nrow);
    for(l_Int i = 0; i < n; i++)
      {
	perm[p[i]] = i;
      }
    BASKER_ASSERT((nblks+1) > 0, "nblks+1 btf");
    //MALLOC_INT_1DARRAY(CC, nblks+1);
    for(l_Int i = 0; i < nblks+1; i++)
      {
	CC[i] = r[i];
      }

    delete [] p;
    delete [] r;
    delete [] work;

    return 0;
  }//strong_component<long int, Entry, Exe_Space>

    //==========================AMD===================
    static
    inline
    int amd_order
    (
     long n, 
     long *col_ptr,
     long *row_idx,
     long *p
     )
    {
      double Info[AMD_INFO];
      for(long i = 0; i < AMD_INFO; ++i)
	{Info[i] = 0;}
      //printf("n: %d\n", n);
      long ret = amesos_amd_l_order(n, col_ptr, row_idx, p, NULL, Info);
      //if(ret == AMD_OK)
      //	printf("OK\n");
      if(ret == AMD_OUT_OF_MEMORY)
	printf("AMD Memory \n");
      if(ret == AMD_INVALID)
	printf("AMD Invalid\n");
      if(ret == AMD_OK_BUT_JUMBLED)
	printf("AMD Jumbled\n");
      
      return 0;
    }



  }; //end BaskerSSWrapper <long>


}//end BaskerNS
#endif