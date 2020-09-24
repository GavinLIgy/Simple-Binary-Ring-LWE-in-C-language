#ifndef _PARAMETER_H_
#define _PARAMETER_H_

#define RBinLWEENC1 0
#define RBinLWEENC2 0
#define RBinLWEENC3 0
#define RBinLWEENCTEST 1

//#define hybird_k 0

//------------Do Not Modify the codes below-----------------

#ifdef RBinLWEENC1 || (RBinLWEENC1 == 1)
    #define BRLWE_N 256
    #define BRLWE_Q 128
    #define BRLWE_logQ 7 // 1<<logQ = Q
#elif RBinLWEENC2 || (RBinLWEENC2 == 1)
    #define BRLWE_N 256
    #define BRLWE_Q 256
    #define BRLWE_logQ 8
#elif RBinLWEENC3 || (RBinLWEENC3 == 1)
    #define BRLWE_N 512
    #define BRLWE_Q 256
    #define BRLWE_logQ 8
#else // RBinLWEENCTEST || (RBinLWEENCTEST == 1)
    #define BRLWE_N 4 
    #define BRLWE_Q 256
    #define BRLWE_logQ 8
#endif

#endif
