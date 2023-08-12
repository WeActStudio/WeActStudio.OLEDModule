/*************************************************************************/
/** Copyright.                                                          **/
/** FileName: SGUI_PolarCoordinates.c                                   **/
/** Author: XuYulin                                                     **/
/** Description: Notice box display interface.                          **/
/*************************************************************************/

//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "SGUI_PolarCoordinates.h"

//=======================================================================//
//= User Macro definition.                                              =//
//=======================================================================//
#define SIN_MAP_SHIFT 15
#define DEGREE_MIN 0
#define DEGREE_MAX 90
#define SIN_MAP_SIZE                            (DEGREE_MAX - DEGREE_MIN + 1)
#define SGUI_SINE(ACCURACY, ANGLE)              ((ACCURACY * s_arriSineTable[ANGLE]) >> SIN_MAP_SHIFT)
#define SGUI_COSINE(ACCURACY, ANGLE)            ((ACCURACY * s_arriSineTable[DEGREE_MAX-ANGLE]) >> SIN_MAP_SHIFT)
#define SGUI_TRIG_ACCURACY_BITS					(10)
#define SGUI_TRIG_ACCURACY                      (0x01 << SGUI_TRIG_ACCURACY_BITS)

#define SGUI_PolarCoord_CosAng(ACCURACY, ANG)   SGUI_PolarCoord_SinAng(ACCURACY, (ANG+90))

//=======================================================================//
//= Static variable define.                                             =//
//=======================================================================//
static const SGUI_INT s_arriSineTable[SIN_MAP_SIZE] =
{
/*     0 */ 0,
/*  1-10 */ 572,   1144,  1715,  2286,  2856,  3425,  3993,  4560,  5126,  5690,
/* 11-20 */ 6252,  6813,  7371,  7927,  8481,  9032,  9580,  10126, 10668, 11207,
/* 21-30 */ 11743, 12275, 12803, 13328, 13848, 14365, 14876, 15384, 15886, 16384,
/* 31-40 */ 16877, 17364, 17847, 18324, 18795, 19261, 19720, 20174, 20622, 21063,
/* 41-50 */ 21498, 21926, 22348, 22763, 23170, 23571, 23965, 24351, 24730, 25102,
/* 51-60 */ 25466, 25822, 26170, 26510, 26842, 27166, 27482, 27789, 28088, 28378,
/* 61-70 */ 28660, 28932, 29197, 29452, 29698, 29935, 30163, 30382, 30592, 30792,
/* 71-80 */ 30983, 31164, 31336, 31499, 31651, 31795, 31928, 32052, 32166, 32270,
/* 81-90 */ 32365, 32449, 32524, 32588, 32643, 32688, 32723, 32748, 32763, 32768
};
//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//


//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
/*************************************************************************/
/** Function Name:  SGUI_PolarCoord_SinAng                              **/
/** Purpose:        Calculate the sine of an angle.                     **/
/** Params:                                                             **/
/** @ iAccuracy[in]: Calculation accuracy.                              **/
/** @ iAngle[in]:     Angle in degrees.                                 **/
/** Return:         The sine of the angle.                              **/
/** Notice:         The angle parameter(iAngle) is in degrees, and only **/
/**                 integers are accepted. return value will be multip- **/
/**                 lied by the accuracy(iAccuracy), integer part will	**/
/**                 be returned.                                        **/
/*************************************************************************/
SGUI_INT SGUI_PolarCoord_SinAng(SGUI_INT iAccuracy, SGUI_INT iAngle)
{
	/*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT			iTransedAng = iAngle % 360;
	SGUI_INT            iReturn;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    iAngle =            iAngle % 360;
    iTransedAng =       iAngle % 180;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(iTransedAng < 0)
	{
		iTransedAng = -iTransedAng;
	}
	if(iTransedAng > 90)
	{
		iTransedAng = 180 - iTransedAng;
	}

	iReturn = SGUI_SINE(iAccuracy, iTransedAng);

	if(iAngle >180)
	{
		iReturn = -iReturn;
	}

	return iReturn;
}

/*************************************************************************/
/** Function Name:  SGUI_PolarCoord_GetPoint                            **/
/** Purpose:        Calculate the screen coordinates based on the polar **/
/**                 coordinates of the point .                          **/
/** Params:                                                             **/
/** @ iPoleX[in]:   Pole Y coordinate.                                  **/
/** @ iPoleY[in]:   Pole Y coordinate.                                  **/
/** @ iModulus[in]: The distance between two points.                    **/
/** @ piPx[out]:    The X coordinates of the desired point.             **/
/** @ piPy[out]:    The Y coordinates of the desired point.             **/
/** Return:         None.                                               **/
/** Notice:         This function will calculate the coordinates of a   **/
/**                 point in the screen coordinate system.              **/
/*************************************************************************/
void SGUI_PolarCoord_GetPoint(SGUI_INT iPoleX, SGUI_INT iPoleY, SGUI_INT IAngle, SGUI_INT iModulus, SGUI_INT* piPx, SGUI_INT* piPy)
{
	/*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT			iEndX = iPoleX + ((SGUI_PolarCoord_CosAng(SGUI_TRIG_ACCURACY, IAngle)*iModulus) >> SGUI_TRIG_ACCURACY_BITS);
	SGUI_INT            iEndY = iPoleY - ((SGUI_PolarCoord_SinAng(SGUI_TRIG_ACCURACY, IAngle)*iModulus) >> SGUI_TRIG_ACCURACY_BITS);

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    *piPx = iEndX;
    *piPy = iEndY;
}

/*************************************************************************/
/** Function Name:  SGUI_PolarCoord_DrawLine                            **/
/** Purpose:        Draw a line according to given polar coordinates.   **/
/** Params:                                                             **/
/** @ pstDeviceIF[in]: Device driver object pointer.                    **/
/** @ iPoleX[in]:   Pole Y coordinate.                                  **/
/** @ iPoleY[in]:   Pole Y coordinate.                                  **/
/** @ iAngle[in]:     Polar angle in degrees.                           **/
/** @ iStart[in]:   Distance from pole to starting point.               **/
/** @ iEnd[in]:     Distance from pole to ending point.                 **/
/** @ eColor[in]:   Line color.                                         **/
/** Return:         None.                                               **/
/** Notice:         This function will calculate the coordinates of a   **/
/**                 point in the screen coordinate system.              **/
/*************************************************************************/
void SGUI_PolarCoord_DrawLine(SGUI_SCR_DEV* pstDeviceIF, SGUI_INT iPoleX, SGUI_INT iPoleY, SGUI_INT iAngle, SGUI_INT iStart, SGUI_INT iEnd, SGUI_COLOR eColor)
{
	/*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_INT			iStartX;
	SGUI_INT            iStartY;
    SGUI_INT			iEndX;
	SGUI_INT            iEndY;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
	/* Get start point coordinate. */
	if(0 == iStart)
	{
		iStartX = iPoleX;
		iStartY = iPoleY;
	}
	else
	{
		SGUI_PolarCoord_GetPoint(iPoleX, iPoleY, iAngle, iStart, &iStartX, &iStartY);
	}

    /* Get ending point coordinate. */
    SGUI_PolarCoord_GetPoint(iPoleX, iPoleY, iAngle, iEnd, &iEndX, &iEndY);

    /* Draw line. */
    SGUI_Basic_DrawLine(pstDeviceIF, iStartX, iStartY, iEndX, iEndY, eColor);
}

