/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: SGUI_Interface.c                                          **/
/** Author: Polarix                                                     **/
/** Description: System platform interface.                             **/
/*************************************************************************/
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_Interface.h"

// SDK string encode convert library interface
#ifdef _SIMPLE_GUI_ENCODE_TEXT_
 #ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
  #include <iconv.h>
 #endif
#endif

// System RTC interface switch.
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
#include <windows.h>
#include <time.h>
#else
    // Include platform RTC interface declare here.
//#include "RTC.h"
#endif

// Dynamic memory operation switch.
#ifdef _SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_
 #ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
  #include <malloc.h>
  #include <string.h>
 #endif
#else
    // Include MMU interface header file here.
#include <string.h>
#endif

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
 #ifdef _SIMPLE_GUI_ENCODE_TEXT_
  static SGUI_CHAR  g_arrcEncodeBuffer[SGUI_ENCODE_BUFFER_SIZE];
 #endif
#endif

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
#ifdef _SIMPLE_GUI_ENCODE_TEXT_
/*************************************************************************/
/** Function Name:  SGUI_SystemIF_EncodeConvert                         **/
/** Purpose:        Convert string encode.                              **/
/** Resources:      None.                                               **/
/** Params:                                                             **/
/** @ szSourceEncode[in]: Source encoder name.                          **/
/** @ szDestinationEncode[in]: Destination encoder name.                **/
/** @ szSource[in]:     String will converted.                          **/
/** Return:         String after convert.                               **/
/*************************************************************************/
SGUI_SZSTR SGUI_SystemIF_EncodeConvert(SGUI_CSZSTR szSourceEncode, SGUI_SZSTR szDestinationEncode, SGUI_SZSTR szSource)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_SZSTR                  pszResultPtr;
    SGUI_SIZE                   uiSourceLength, uiOutputBufferSize;
    SGUI_SIZE                   uiEncoderResult;
    iconv_t                     pIconv;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    uiEncoderResult         = -1;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pIconv = iconv_open(szDestinationEncode, szSourceEncode);
    if((iconv_t)-1 != pIconv)
    {
        uiSourceLength = SGUI_SystemIF_StringLength(szSource) + 1;
        uiOutputBufferSize = SGUI_ENCODE_BUFFER_SIZE;
        pszResultPtr = g_arrcEncodeBuffer;
        uiEncoderResult = iconv(pIconv, &szSource, &uiSourceLength, &pszResultPtr, &uiOutputBufferSize);
        if (uiEncoderResult == (SGUI_SIZE)-1)
        {
            pszResultPtr = NULL;
        }
        else
        {
            pszResultPtr = g_arrcEncodeBuffer;
        }
    }
    iconv_close(pIconv);
    return pszResultPtr;
}
#endif

#ifdef _SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_
/*************************************************************************/
/** Function Name:  SGUI_SystemIF_Allocate                              **/
/** Purpose:        Allocate a memory block.                            **/
/** Resources:      None.                                               **/
/** Params:                                                             **/
/** @ uiSize[in]:       Allocated memory size.                          **/
/** Return:         Allocated memory block started address, same as STD **/
/**                 malloc interface.                                   **/
/*************************************************************************/
void* SGUI_SystemIF_Allocate(SGUI_SIZE uiSize)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    void                        *pAllocatedMemory;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    pAllocatedMemory =      NULL;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
    pAllocatedMemory = malloc(uiSize);
#else
    // Add allocate memory function here;
    pAllocatedMemory = malloc(uiSize);
#endif
    return pAllocatedMemory;
}

/*************************************************************************/
/** Function Name:  SGUI_SystemIF_Free                                  **/
/** Purpose:        Free a memory block.                                **/
/** Resources:      None.                                               **/
/** Params:                                                             **/
/** @ pFreePointer[in]: Free memory pointer                             **/
/** Return:         None.                                               **/
/*************************************************************************/
void SGUI_SystemIF_Free(void* pFreePointer)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pFreePointer)
    {
#ifdef _SIMPLE_GUI_USE_SYS_PF_FUNCTIONS_
        free(pFreePointer);
#else
        // Add allocate memory function here;
        free(pFreePointer);
#endif
    }
}
#endif

