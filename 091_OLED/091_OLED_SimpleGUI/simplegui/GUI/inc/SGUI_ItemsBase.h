#ifndef _INCLUDE_SGUI_ITEMS_BASE_H_
#define _INCLUDE_SGUI_ITEMS_BASE_H_
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"
#include "SGUI_Text.h"

//=======================================================================//
//= User Macro definition.                                              =//
//=======================================================================//
#define     ITEM_EDGE_WIDTH                     (1)
#define     ITEM_HEIGHT(FONT)                   ((FONT)->iHeight+(ITEM_EDGE_WIDTH*2))

//=======================================================================//
//= Data type definition.                                               =//
//=======================================================================//
typedef struct _ST_SGUI_ITEMS_ITEM_
{
    SGUI_CSZSTR                     cszLabelText;               // This pointer points to a character array constant
    SGUI_SZSTR                      szVariableText;             // This pointer points to a character array variable, this member will be used first if existed.
    struct _ST_SGUI_ITEMS_ITEM_*    pstPrev;
    struct _ST_SGUI_ITEMS_ITEM_*    pstNext;
}SGUI_ITEMS_ITEM;

typedef struct
{
    SGUI_INT                        iIndex;
    SGUI_ITEMS_ITEM*                pstItem;
}SGUI_ITEM_SELECTION;

typedef struct
{
    SGUI_RECT                       stLayout;
    SGUI_ITEMS_ITEM*                pstFirstItem;               // Read only
    SGUI_ITEMS_ITEM*                pstLastItem;
    SGUI_INT                        iCount;
    SGUI_ITEM_SELECTION             stSelection;                // Read only
    SGUI_ITEM_SELECTION             stVisibleStart;             // Read only
    SGUI_ITEM_SELECTION             stVisibleEnd;               // Read only
    SGUI_INT                        iVisibleItems;
    SGUI_INT                        iItemPaintOffset;
    const SGUI_FONT_RES*            pstFontRes;
}SGUI_ITEMS_BASE;

//=======================================================================//
//= Public function declaration.                                        =//
//=======================================================================//
void                    SGUI_ItemsBase_Initialize(SGUI_ITEMS_BASE* pstObj, const SGUI_RECT* cpstLayout, const SGUI_FONT_RES* pstFontRes, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount);
SGUI_ITEMS_ITEM*        SGUI_ItemsBase_JumpItem(SGUI_ITEMS_ITEM* pstBaseItem, SGUI_INT iSteps);
void                    SGUI_ItemsBase_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_ITEMS_BASE* pstObj);
SGUI_ITEMS_ITEM*        SGUI_ItemsBase_GetItem(SGUI_ITEMS_BASE* pstObj, SGUI_INT iSelection);
void                    SGUI_ItemsBase_GetItemExtent(SGUI_ITEMS_BASE* pstObj, SGUI_INT iSelection, SGUI_RECT* pstItemExtent);
void                    SGUI_ItemsBase_Resize(SGUI_ITEMS_BASE* pstObj, const SGUI_RECT* cpstNewLayout);
#define                 SGUI_ItemsBase_Count(/* SGUI_ITEMS_BASE */OBJ) \
                            ((OBJ)->iCount)
void                    SGUI_ItemsBase_Selecte(SGUI_ITEMS_BASE* pstObj, SGUI_INT iIndex);
#define                 SGUI_ItemsBase_SelecteFirst(/* SGUI_ITEMS_BASE */OBJ) \
                            {   SGUI_ItemsBase_GetSelection(OBJ)->iIndex = 0; \
                                SGUI_ItemsBase_GetSelection(OBJ)->pstItem = (OBJ)->pstFirstItem;}
#define                 SGUI_ItemsBase_SelecteLast(/* SGUI_ITEMS_BASE */OBJ) \
                            {   SGUI_ItemsBase_GetSelection(OBJ)->iIndex = (OBJ)->iCount - 1; \
                                SGUI_ItemsBase_GetSelection(OBJ)->pstItem = (OBJ)->pstLastItem;}
#define                 SGUI_ItemsBase_GetSelection(/* SGUI_ITEMS_BASE */OBJ) \
                            (&((OBJ)->stSelection))
#define                 SGUI_ItemsBase_GetFirstVisible(/* SGUI_ITEMS_BASE */OBJ) \
                            (&((OBJ)->stVisibleStart))
#define                 SGUI_ItemsBase_GetLastVisible(/* SGUI_ITEMS_BASE */OBJ) \
                            (&((OBJ)->stVisibleEnd))
SGUI_ITEMS_ITEM*        SGUI_ItemsBase_RemoveItem(SGUI_ITEMS_BASE* pstObj, SGUI_INT iRemoveIndex);
SGUI_ITEMS_ITEM*        SGUI_ItemsBase_InsertItem(SGUI_ITEMS_BASE* pstObj, SGUI_ITEMS_ITEM* pstNewItem, SGUI_INT iIndex);
SGUI_BOOL               SGUI_ItemsBase_CanScrollUp(SGUI_ITEMS_BASE* pstObj);
SGUI_BOOL               SGUI_ItemsBase_CanScrollDown(SGUI_ITEMS_BASE* pstObj);

#endif // _INCLUDE_SGUI_ITEMS_BASE_H_
