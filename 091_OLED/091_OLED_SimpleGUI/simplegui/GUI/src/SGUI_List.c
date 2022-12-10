/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: SGUI_List.c                                               **/
/** Author: XuYulin                                                     **/
/** Description: Draw list interface.                                   **/
/*************************************************************************/
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_List.h"

//=======================================================================//
//= User Macro definition.                                              =//
//=======================================================================//
// User settings
#define     LIST_SCROLLBAR_WIDTH                        (5)
// Automatic calculation
#define     LIST_EDGE_SIZE                              1
#define     LIST_TITLE_HEIGHT(FONT)                     ((FONT)->iHeight+2)

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
/*************************************************************************/
/** Function Name:  SGUI_List_InitializeListData                        **/
/** Purpose:        Initialize a list control data.                     **/
/** Params:                                                             **/
/** @ pstObj[in]:       Pointer of list data will be initialized.       **/
/** @ pstFontRes[in]: Font resource object pointer.                     **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_List_Initialize(SGUI_LIST* pstObj, const SGUI_RECT* cpstLayout, const SGUI_FONT_RES* pstFontRes, SGUI_CSZSTR cszTitle, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_SCROLLBAR_PARAM    stScrollBarParam;
    SGUI_RECT               stSubLayout;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    SGUI_SystemIF_MemorySet(&stSubLayout, 0x00, sizeof(SGUI_RECT));

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // Copy layout.
    SGUI_SystemIF_MemoryCopy(&(pstObj->stLayout), cpstLayout, sizeof(SGUI_RECT));
    // Title
    pstObj->szTitle = cszTitle;
    // Initialize font resource.
    pstObj->pstFontRes = pstFontRes;
    // Initialize Items.
    stSubLayout.iX = pstObj->stLayout.iX+2;
    stSubLayout.iWidth = pstObj->stLayout.iWidth-4-LIST_SCROLLBAR_WIDTH;
    if(NULL == pstObj->szTitle)
    {
        stSubLayout.iY = pstObj->stLayout.iY+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-4;
    }
    else
    {
        stSubLayout.iY = stSubLayout.iY+LIST_TITLE_HEIGHT(pstFontRes)+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-LIST_TITLE_HEIGHT(pstFontRes)-3;
    }
    SGUI_ItemsBase_Initialize(&(pstObj->stItems), &stSubLayout, pstFontRes, pstItemsData, iItemsCount);
    // Initialize scroll bar.
    stScrollBarParam.eDirection = SGUI_SCROLLBAR_VERTICAL;
    stScrollBarParam.stLayout.iX = pstObj->stItems.stLayout.iX+pstObj->stItems.stLayout.iWidth+1;
    stScrollBarParam.stLayout.iY = pstObj->stItems.stLayout.iY;
    stScrollBarParam.stLayout.iWidth = LIST_SCROLLBAR_WIDTH;
    stScrollBarParam.stLayout.iHeight = pstObj->stItems.stLayout.iHeight;
    stScrollBarParam.sMaxValue = (pstObj->stItems.iCount > pstObj->stItems.iVisibleItems)?(pstObj->stItems.iCount - pstObj->stItems.iVisibleItems):0;
    SGUI_ScrollBar_Initialize(&(pstObj->stScrollBar), &stScrollBarParam);
}

/*************************************************************************/
/** Function Name:  SGUI_List_Repaint                                   **/
/** Purpose:        Refresh list display.                               **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ pstObj[in]:   Pointer of list data will be refreshed.             **/
/** Return:         None.                                               **/
/** Notice:         This function will refresh all list display on      **/
/**                 screen, include edge, items, title and scrollbar.   **/
/*************************************************************************/
void SGUI_List_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_LIST* pstObj)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_RECT               stTitleTextDisplayArea;
    SGUI_POINT              stInnerPos;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstObj)
    {
        // Clear list item display area.
        SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY, pstObj->stLayout.iWidth, pstObj->stLayout.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
        // Paint title.
        if(NULL != pstObj->szTitle)
        {
            stTitleTextDisplayArea.iX = pstObj->stLayout.iX+2;
            stTitleTextDisplayArea.iY = pstObj->stLayout.iY+2;
            stTitleTextDisplayArea.iWidth = pstObj->stLayout.iWidth-4;
            stTitleTextDisplayArea.iHeight = pstObj->pstFontRes->iHeight;
            stInnerPos.iX = 0;
            stInnerPos.iY = 0;
            SGUI_Text_DrawText(pstDeviceIF, pstObj->szTitle, pstObj->pstFontRes, &stTitleTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
            SGUI_Basic_DrawLine(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY+pstObj->pstFontRes->iHeight+3, pstObj->stLayout.iX+pstObj->stLayout.iWidth-1, pstObj->stLayout.iY+pstObj->pstFontRes->iHeight+3, SGUI_COLOR_FRGCLR);
        }
        // Paint items.
        SGUI_ItemsBase_Repaint(pstDeviceIF, &(pstObj->stItems));
        // Paint scroll bar.
        SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.stVisibleStart.iIndex);
        SGUI_ScrollBar_Repaint(pstDeviceIF, &(pstObj->stScrollBar));
    }
}

/*************************************************************************/
/** Function Name:  SGUI_List_Resize                                    **/
/** Purpose:        Resize and calculate layout parameter for display   **/
/**                 changed.                                            **/
/** Params:                                                             **/
/** @ pstObj[in]:   Pointer of list object will be paint.               **/
/** @ cpstNewLayout[in]: New position and size.                         **/
/** Return:         None.                                               **/
/** Notice:         None.                                               **/
/*************************************************************************/
void SGUI_List_Resize(SGUI_LIST* pstObj, const SGUI_RECT* pstNewLayout)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_RECT               stSubLayout;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    SGUI_SystemIF_MemorySet(&stSubLayout, 0x00, sizeof(SGUI_RECT));

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // Copy layout.
    SGUI_SystemIF_MemoryCopy(&(pstObj->stLayout), pstNewLayout, sizeof(SGUI_RECT));
    // Resize Items.
    stSubLayout.iX = pstObj->stLayout.iX+2;
    stSubLayout.iWidth = pstObj->stLayout.iWidth-4-LIST_SCROLLBAR_WIDTH;
    if(NULL == pstObj->szTitle)
    {
        stSubLayout.iY = pstObj->stLayout.iY+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-4;
    }
    else
    {
        stSubLayout.iY = stSubLayout.iY+LIST_TITLE_HEIGHT(pstObj->pstFontRes)+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-LIST_TITLE_HEIGHT(pstObj->pstFontRes)-3;
    }
    SGUI_ItemsBase_Resize(&(pstObj->stItems), &stSubLayout);
    // Resize scroll bar.
    pstObj->stScrollBar.stParam.stLayout.iX = pstObj->stItems.stLayout.iX+pstObj->stItems.stLayout.iWidth+1;
    pstObj->stScrollBar.stParam.stLayout.iY = pstObj->stItems.stLayout.iY;
    pstObj->stScrollBar.stParam.stLayout.iWidth = LIST_SCROLLBAR_WIDTH;
    pstObj->stScrollBar.stParam.stLayout.iHeight = pstObj->stItems.stLayout.iHeight;
}

/*************************************************************************/
/** Function Name:  SGUI_List_RemoveItem                                **/
/** Purpose:        Remove a item from list.                            **/
/** Params:                                                             **/
/** @ pstObj[in]:   Pointer of list object.                             **/
/** @ iRemoveIndex[in]: Index of item will be removed.                  **/
/** Return:         Removed item object pointer.                        **/
/** Notice:         if memory of item is dynamically allocated, Please  **/
/**                 release MANUALLY.                                   **/
/*************************************************************************/
SGUI_ITEMS_ITEM* SGUI_List_RemoveItem(SGUI_LIST* pstObj, SGUI_INT iRemoveIndex)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_ITEMS_ITEM*        pstRemovedItem;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pstRemovedItem = SGUI_ItemsBase_RemoveItem(&(pstObj->stItems), iRemoveIndex);
    // Paint scroll bar.
    SGUI_ScrollBar_SetMax(&(pstObj->stScrollBar), (pstObj->stItems.iCount > pstObj->stItems.iVisibleItems)?(pstObj->stItems.iCount - pstObj->stItems.iVisibleItems):0)
    SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.stVisibleStart.iIndex);

    return pstRemovedItem;
}

/*************************************************************************/
/** Function Name:  SGUI_List_InsertItem                                **/
/** Purpose:        Insert a new item before item with the index.       **/
/** Params:                                                             **/
/** @ pstObj[in]:   Pointer of items-base object.                       **/
/** @ pstNewItem[in]: Inserted new item object.                         **/
/** @ iIndex[in]:   Index of new inserted item .                        **/
/** Return:         Inserted item object pointer.                       **/
/** Notice:         Return NULL when insert failed.                     **/
/*************************************************************************/
SGUI_ITEMS_ITEM* SGUI_List_InsertItem(SGUI_LIST* pstObj, SGUI_ITEMS_ITEM* pstNewItem, SGUI_INT iIndex)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_ITEMS_ITEM*        pstInsertedItem;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    pstInsertedItem = SGUI_ItemsBase_InsertItem(&(pstObj->stItems), pstNewItem, iIndex);

    // Paint scroll bar.
    SGUI_ScrollBar_SetMax(&(pstObj->stScrollBar), (pstObj->stItems.iCount > pstObj->stItems.iVisibleItems)?(pstObj->stItems.iCount - pstObj->stItems.iVisibleItems):0)
    SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.stVisibleStart.iIndex);

    return pstInsertedItem;
}
