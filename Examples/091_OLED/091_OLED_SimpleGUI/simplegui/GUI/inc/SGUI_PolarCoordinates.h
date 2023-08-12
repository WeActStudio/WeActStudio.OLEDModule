#ifndef _INCLUDE_SGUI_POLAR_COORDINATES_H_
#define _INCLUDE_SGUI_POLAR_COORDINATES_H_
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_Common.h"
#include "SGUI_Basic.h"

//=======================================================================//
//= User Macro definition.                                              =//
//=======================================================================//

//=======================================================================//
//= Public function declaration.                                        =//
//=======================================================================//
void SGUI_PolarCoord_GetPoint(SGUI_INT iPoleX, SGUI_INT iPoleY, SGUI_INT IAngle, SGUI_INT iModulus, SGUI_INT* piPx, SGUI_INT* piPy);
void SGUI_PolarCoord_DrawLine(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iPoleX, SGUI_INT iPoleY, SGUI_INT iAngle, SGUI_INT iStart, SGUI_INT iEnd, SGUI_COLOR eColor);
void SGUI_PolarCoord_DrawArc(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iCx, SGUI_INT iCy, SGUI_INT iRadius, SGUI_INT iStartAngle, SGUI_INT iEndAngle, SGUI_COLOR eColor);

#endif // _INCLUDE_SGUI_POLAR_COORDINATES_H_
