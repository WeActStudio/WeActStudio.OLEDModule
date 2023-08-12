<h1 id="0">目录</h1>

* [1. 概述](#1) 
* [2. GUI](#2) 
* [3. HMI](#3) 
* [4. 相关配置](#4) 
* [5. 其他应用技巧](#5) 
* [6. 使用限制](#6) 
* [7. 联系开发者](#7) 
-------
<h2 id="1">1. 概述</h2>  

&emsp;&emsp;本文将详细介绍SimpleGUI各个机能模块的组成、依赖关系与实现原理，方便用户根据自己的具体需求对SimpleGUI进行必要的修改、剪裁或二次开发。  
&emsp;&emsp;SimpleGUI分为GUI和HMI两部分，GUI部分负责显示和绘图控制，HMI则提供了一种构件和管理界面的逻辑关系的手段，方便开发者分离业务逻辑和画面显示，更有利于编写和维护。如果不需要，HMI可不使用。  

<返回到[目录](#0)/[标题](#1)>

<h2 id="2">2. GUI</h2>  

### 2.1. 基本数据类型   
&emsp;&emsp;为避免在因不同平台、不同编译器下对不同基本数据类型的定义不同导致的可能潜在的问题，SimpleGUI单独使用了一个名为SGUI_Typedef.h的文件，对SimpleGUI的数据类型进行了重新封装，以避免因为不同数据类型的表达范围、指针等问题导致难以调查的系统异常。  

### 2.2. 设备驱动  
&emsp;&emsp;SimpleGUI对显示设备和资源的使用是通过SGUI_SCR_DEV(SimpleGUI Screen Device)结构体实例来实现的，结构体的定义为：  

```c++
typedef struct
{
    //Screen display area size in pixel.
    SGUI_AREA_SIZE              stSize;
    //Bitmap data buffer.
    SGUI_BYTE                   arrBmpDataBuffer[SGUI_BMP_DATA_BUFFER_SIZE];
    //Engine & device initialize function.
    SGUI_FN_IF_INITIALIZE       fnInitialize;
    //Clear screen function.
    SGUI_FN_IF_CLEAR            fnClear;
    //Set pixel value function.
    SGUI_FN_IF_SET_POINT        fnSetPixel;
    //Get pixel value function.
    SGUI_FN_IF_GET_POINT        fnGetPixel;
    // Refresh screen display.
    SGUI_FN_IF_REFRESH          fnSyncBuffer;
}SGUI_SCR_DEV;
```
&emsp;&emsp;此数据结构的定义位于SGUI_Typedef.h文件中，包含若干函数指针，用户需要声明一个此类型的结构体，然后按照结构体中函数指针的数据类型设计和实现相应功能的设备驱动函数，并将对应的函数指针注册到结构体实例，然后在调用SimpleGUI绘图函数时，将该结构体的指针作为参数传入函数（通常是参数表的第一个参数）。  
&emsp;&emsp;结构体各成员的功能描述如下：
|指针|函数功能|参数表|返回值|  
|:--|:--|:--|:--|  
|stSize|保存屏幕设备的现实尺寸，单位为像素。|-|-|  
|arrBmpDataBuffer|字节数组，用于在位图绘制（包括文字绘制）时读取位图数据。|-|-| 
|fnInitialize|设备初始化，通常由HMI部分调用。|void|SGUI_INT|  
|fnClear|清除屏幕显示，如果使用了显示缓存，同时清除缓存。|void|void|  
|fnSetPixel|设定像素值，0为灭1为亮。|SGUI_INT, SGUI_INT, SGUI_INT|void|  
|fnGetPixel|读取像素值，0为灭1为亮。|SGUI_INT, SGUI_INT|SGUI_INT| 
|fnSyncBuffer|更新屏幕显示，通常指将显示缓存的数据同步到设备。|void|void|  

&emsp;&emsp;具体的数据类型定义请参考SGUI_Typedef.h文件的内容。  
&emsp;&emsp;在使用SimpleGUI之前，首先需要您实现您目标平台上对屏幕设备的操作函数。实现内容至少包括设备初始化、读像素和写像素三个接口。  
&emsp;&emsp;如果不考虑执行效率，以上三个函数实现后就，SimpleGUi就可以工作了，当然，如果想更有效的绘图，还需根据驱动程序的逻辑结构、缓存的数据结构、硬件特性等因素实现其他的接口函数，如清除屏幕、更新屏幕、水平线绘制、垂直线绘制等，如果使用了显示缓存，那么还有必要实现缓存同步函数，以实现将缓存内容同步到屏幕的功能。  
&emsp;&emsp;以下为演示例程中对该结构体实例的声明和使用。  
```c++
SGUI_SCR_DEV g_stDeviceInterface;

SGUI_SystemIF_MemorySet(&g_stDeviceInterface, 0x00, sizeof(SGUI_SCR_DEV));
/* Initialize display size. */
g_stDeviceInterface.stSize.Width = 128;
g_stDeviceInterface.stSize.Height = 64;
/* Initialize interface object. */
g_stDeviceInterface.stActions.fnSetPixel = OLED_SetPixel;
g_stDeviceInterface.stActions.fnGetPixel = OLED_GetPixel;
g_stDeviceInterface.stActions.fnClearScreen = OLED_ClearDisplay;
g_stDeviceInterface.stActions.fnRefreshScreen = OLED_RefreshScreen;
```   

### 2.3. 功能函数  

&emsp;&emsp;SimpleGUI的所有功能函数可以简单的划分为“平台接口”、“共通机能”、“基础绘图”和“控件”四部分。  
#### &emsp;&emsp;平台接口  
&emsp;&emsp;此部分接口的实现位于SGUI_Interface.c文件中，封装或实现包括内存块复制(memcpy)、赋值(memset)、申请与释放(malloc/free)字符串的复制(strcpy/strncpy)、长度测量(strlen)以及时间(RTC时间)的获取、Flash(包括内部和外部)中数据的获取等依赖于系统平台的机能实现。  
#### &emsp;&emsp;共通机能
&emsp;&emsp;此部分接口的实现位于SGUI_Common.c文件中，封装或实现了包括数字与字符串之间的格式化转换、以及小数点的插入等。由于SimpleGUI中全程避免使用浮点数，所以小数点基本上都是通过格式化字符串得来，请悉知。  
#### &emsp;&emsp;基础绘图
&emsp;&emsp;此部分接口的实现位于SGUI_Basic.c文件中，封装或实现了点、线、圆形、矩形以及位图的绘制，以及区域反色、屏幕内容的更新(仅在使用显示缓存时有用)与清除等。  
#### &emsp;&emsp;控件
&emsp;&emsp;此部分也可被称为“组件”，实现于多个文件中，提供了多种方便使用的函数。  

|文件名|功能概要|    
|:--|:-- |  
|SGUI_Text.c|提供封装好的文字绘制功能、包括单行、多行以及绘制文字前需要的部分预演算等。|   
|SGUI_Frame.c|边框绘制功能，用于绘制屏幕边框，例如列表和实时曲线的窗口边框等。|  
|SGUI_Notice.c|用于显示消息提示框，提示一些文字内容，可带图标，类似Windows中的MessageBox。|  
|SGUI_ScrollBar.c|提供显示水平或垂直滚动条的功能。|   
|SGUI_ProcessBar.c|提供进度条的显示功能，支持上、下、左、右四个生长方向。|   
|SGUI_List|提供列表显示功能，可以显示一个列表，列表项目可以为枚举字符串型、整数型、小数型或不带参数的普通型。|  
|SGUI_VariableBox.c|提供数值输入和字符串输入两种输入框接口。|  
|SGUI_RealtimeGraph.c|提供封装好的实时曲线界面接口，可以在屏幕上根据用户输入的点数据绘制和现实一条实时曲线。|  

&emsp;&emsp;以上文件均提供同名的头文件(.h)，可以直接包含和调用。 

&emsp;&emsp;所有的控件的实例在显示和操作中，均使用一个结构体进行数据和参数的保存，同时，所有的控件均提供画面更新接口(SGUI_XXX_Repaint函数)，用以在任何需要的时候恢复控件在屏幕设备上的显示。此方法的设计意在在绝大多数情况下可以取代图层的概念。

### 2.4. 依赖关系  

&emsp;&emsp;SimpleGUI为了可以在一定程度上实现剪裁，将代码设计为前文描述的四部分，四部分从高到低，大体呈依赖关系，例如控件的实现依赖基础绘图、基础绘图的实现依赖共通机能、共通机能的实现一定程度上依赖平台接口。以最常用的列表为例，列表的实现、依赖Text、ScrollBar两个控件、Text中的文本绘制功能、以及基础绘图的区域反色功能。  
&emsp;&emsp;截至目前，SimpleGUI的所有功能模块之间的依赖关系请参照“Dependencies.xlsx”文件。在开发时可依照此文档对SimpleGUI进行适当剪裁以避免资源浪费。  

<返回到[目录](#0)/[标题](#2)>


<h2 id="3">3. HMI</h2>  

### 3.1 HMI概述
&emsp;&emsp;HMI模型作为SimpleGUI的附加组件存在，意在使用相对固定的数据格式及代码逻辑，帮助开发者方便的编写和完成便于维护和拓展的用户交互动作。  
&emsp;&emsp;HMI主要分为“画面”和“交互引擎”两部分，交互引擎负责外来事件、数据的组织与预处理、并调用用户预先准备好的屏幕绘制函数更新屏幕显示。而屏幕显示的操作，则交由画面部分完成。  

### 3.2 画面  
&emsp;&emsp;HMI模型在在设计之初，虽然使用C语言的语法，但是使用了C++中“类”和“对象”的概念进行构建和管理，每一个画面都是一个对象，都具有画面动作，概念上类似于类的成员方法，包含“初始化”、“准备”、“事件处理”、“画面更新”、“后处理”五个部分，以下称为画面动作对象。  
&emsp;&emsp;画面动作对象的数据格式定义如下：  

```c++
// Screen action interface function pointer structure.
typedef struct
{
	// Initialize screen data and parameter.
    HMI_ENGINE_RESULT   (*Initialize) (SGUI_SCR_DEV* Interface);
    // Do some thing before current screen display.
	HMI_ENGINE_RESULT   (*Prepare) (SGUI_SCR_DEV* Interface, const void* pstParameters);
	// Repaint screen if needed.
	HMI_ENGINE_RESULT   (*Repaint) (SGUI_SCR_DEV* Interface, const void* pstParameters);
	// Process event.
	HMI_ENGINE_RESULT   (*ProcessEvent) (SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
	// Post process.
	HMI_ENGINE_RESULT   (*PostProcess) (SGUI_SCR_DEV* Interface, SGUI_INT iActionID);
}HMI_SCREEN_ACTION;
```  

&emsp;&emsp;各个成员的功能说明如下：

|变量名|功能概要|    
|:--|:-- |  
|Initialize|函数指针，指向画面的初始化函数。|   
|Prepare|函数指针，指向画面对象的准备函数，通常用于画面显示前或事件处理前的一些预处理。|  
|Repaint|函数指针，指向画面更新函数，意在必要时更新画面显示。|  
|ProcessEvent|函数指针，指向画面的事件处理函数，用户的操作或设备、外部中断的信息在有必要更新显示时，通过此函数通知到画面并做相应处理。|   
|PostProcess|函数指针，消息处理后的后续处理，例如需要变更到其他画面显示、返回等。|   

&emsp;&emsp;而在一个系统中，不可能只有一个画面，所以画面对象除了包含画面动作对象外，还包含前指针以及画面ID的记录。画面对象数据格式的定义如下：  

```c++
// Screen data structure.
typedef struct _T_HMI_SCREEN_OBJECT_
{
	SGUI_INT						iScreenID;
	HMI_SCREEN_ACTION*  			pstActions;
	struct _T_HMI_SCREEN_OBJECT_*	pstPrevious;
}HMI_SCREEN_OBJECT;
```  

&emsp;&emsp;各个成员的功能说明如下：

|变量名|功能概要|    
|:--|:-- |  
|iScreenID|画面ID，每一个画面应该有一个专用的、不重复的ID用于索引对象本身。|   
|pstActions|画面动作对象指针，指向画面动作处理函数对象。|  
|pstPrevious|前画面指针，默认情况下为NULL，当画面显示时，此指针将被修改为指向前一个画面的对象。|   

&emsp;&emsp;此结构体才是用来操作“画面”的对象，所有的画面对象都以此格式声明和定义，使画面数据的操作、事件的响应统一化，进而方便画面的管理与扩充。在设计画面时，为了代码维护方便，推荐每个画面对应的处理都实现在单独的文件中，这样一来，每一个文件就被抽象成了一个“画面对象”，类似C++中类的实例。    
&emsp;&emsp;接下来以实时曲线为例，简要说明一下画面的组织与构成。具体逻辑可以参照DemoProc文件夹下的src/RealtimeGraph.c文件。为讲解方便，讲解代码与演示工程的实例有所出入，请悉知。    

&emsp;&emsp;在RealtimeGraph.c文件中，对Action对象的声明和定义如下：  
```c++
static HMI_SCREEN_ACTION s_stDemoRealtimeGraphActions = {
    HMI_DemoRealGraph_Initialize,
    HMI_DemoRealGraph_Prepare,
    HMI_DemoRealGraph_RefreshScreen,
    HMI_DemoRealGraph_ProcessEvent,
    HMI_DemoRealGraph_PostProcess,
};
```  

&emsp;&emsp;对应的五个函数的原型声明如下：  
```c++
static HMI_DemoRealGraph_Initialize(SGUI_SCR_DEV* pstIFObj);
static HMI_ENGINE_RESULT HMI_DemoRealGraph_Prepare(SGUI_SCR_DEV* pstIFObj, const void* pstParameters);
static HMI_ENGINE_RESULT HMI_DemoRealGraph_RefreshScreen(SGUI_SCR_DEV* pstIFObj, const void* pstParameters);
static HMI_ENGINE_RESULT HMI_DemoRealGraph_ProcessEvent(SGUI_SCR_DEV* pstIFObj, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT HMI_DemoRealGraph_PostProcess(SGUI_SCR_DEV* pstIFObj, SGUI_INT iActionID);
```  

&emsp;&emsp; 有了上述的函数和结构体对象定义，接下来，就可以定义画面对象了：  

```c++
HMI_SCREEN_OBJECT g_stHMI_DemoRealtimeGraph = {	    
    HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH,
    &s_stDemoRealtimeGraphActions
};
```  

&emsp;&emsp;HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH为实时曲线画面对象的唯一ID，此对象需要全局引用，故此，在DemoProc.hw文件中，做了全局声明：  

```c++
extern HMI_SCREEN_OBJECT g_stHMI_DemoRealtimeGraph;
```  

&emsp;&emsp;此时，就可以对这个画面对象进行操作了，例如在系统启动，对该画面对象进行初始化时，就可以这样进行操作：

```c++
g_stHMI_DemoRealtimeGraph.pstActions->Initialize(&g_stDeviceInterface);
```  

&emsp;&emsp;同理，需要显示前准备或者画面更新时也可以这样操作：

```c++
g_stHMI_DemoRealtimeGraph.pstActions->Prepare(&g_stDeviceInterface, pstData);
g_stHMI_DemoRealtimeGraph.pstActions->Repaint(&g_stDeviceInterface, NULL);
``` 

### 3.3 事件  

&emsp;&emsp;SimpleGUI的HMI模型中，画面对外部的响应是以一种被称为“事件”的处理方式进行的。画面对事件的响应，是通过画面动作对象的ProcessEvent指针指向的函数进行处理与响应的。每一个事件都含有一个事件头，事件头中包含了事件的Type、ID和Size三种信息，Type和ID均由用户自由定义，并在事件的发送和画面对象的事件处理过程中使用。HMI模型不参与Type与ID的定义、修改与判断。    
&emsp;&emsp;事件头的数据结构定义如下。    

```c++
typedef struct
{
    HMI_EVENT_TYPE      eType;
    SGUI_INT            iID;
    SGUI_INT            iSize;
}HMI_EVENT_BASE;
```

&emsp;&emsp;HMI模型的事件分为两种，一种是通用事件，这种事件只有一个头部，包含着Type和ID信息，通常用来通知一些周期性事件，例如定时器、闹钟、固定的按键等等。    
&emsp;&emsp;通用事件的数据结构定义如下。   

```c++
typedef struct
{
    HMI_EVENT_BASE      Head;
}HMI_GENERAL_EVENT;
```   

&emsp;&emsp;另外一种叫做自定义事件，自定义事件除了普通的事件头信息外，还可以夹带一些用户自定义的数据，为了达成这种设计目的而不消耗过多内存，HMI提供了一下宏定义方便用户声明自定义的事件类型。    

```c++
#define HMI_EVENT_TYPE_DECLARE(NAME, DATA) \
typedef struct \
{ \
	HMI_EVENT_BASE Head; \
	DATA Data; \
}NAME;
```   

&emsp;&emsp;其中NAME为新的事件类型名，DATA为新事件需要夹带的数据结构定义。
&emsp;&emsp;这样的好处就是，可以使用类似于C++中“继承”的概念。在画面对象的消息处理中，所有事件均可以先以事件头（HMI_EVENT_BASE）的格式进行解析，在正确的识别Type和ID后，再使用强制类型转换，对用户自定义的数据类型进行读取。    
&emsp;&emsp;接下来依然以实时曲线为例，简要说明事件的使用方法。曲线数据来自一个定时器的周期性触发，每触发一次，都产生一个随机数（在VirtualSDK中），然后将这个数字作为数据发送到实时曲线画面对象。详细处理请参照DemoProc文件夹下的DemoProc.c和src/在RealtimeGraph.c文件。为讲解方便，讲解代码与演示工程的实例有所出入，请悉知。    

&emsp;&emsp;实时曲线数据事件类型定义如下：    
```c++
typedef struct
{
	SGUI_INT		iValue;
}DUMMY_DATA_EVENT_DATA;

HMI_EVENT_TYPE_DECLARE(DATA_EVENT, DUMMY_DATA_EVENT_DATA);
```    
&emsp;&emsp;定时器触发的处理如下：    
```c++
DATA_EVENT				stEvent;
SGUI_INT                iAction;
HMI_EVENT_INIT(stEvent);

stEvent.Head.eType = HMI_ENGINE_EVENT_DATA;
stEvent.Head.iID = EVENT_ID_TIMER;
stEvent.Data.iValue = (rand() % 200)-100;

g_stHMI_DemoRealtimeGraph.pstActions->ProcessEvent(&g_stDeviceInterface, (HMI_EVENT_BASE*)&stEvent, &iAction);

g_stDeviceInterface->fnRefreshScreen();
```   
&emsp;&emsp;在实时曲线的画面对象中，事件处理如下：    
```c++
HMI_ENGINE_RESULT HMI_DemoRealGraph_ProcessEvent(SGUI_SCR_DEV* pstIFObj, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    DATA_EVENT*					pstDataEvent;

    if((EVENT_ID_TIMER == pstEvent->iID) && (HMI_ENGINE_EVENT_DATA == pstEvent->eType))
    {
        pstDataEvent = (DATA_EVENT*)pstEvent;
		SGUI_RealtimeGraph_AppendValue(pstIFObj, &s_stRealtimeGraph, pstDataEvent->Data.iValue);
		HMI_DemoRealGraph_RefreshScreen(pstIFObj, NULL);
    }
}
```  
&emsp;&emsp;这样，就完成了画面对于特定事件的响应，响应内容可以是用户操作、也可以是传感器或设备数据的更新。    

### 3.4 HMI引擎    

&emsp;&emsp;在一个系统中往往有多个画面，尤其是一些控制系统，通常会包含一些列表之类。
以列表为例，按照通常的逻辑，系统中的列表需要这样处理：    
```
选择操作()
{
    选择项目 = 显示列表();
    switch(选择项目)
    {
        case 0:
         ...
         break;
        case 1:
         ...
         break;
         ...
        default:
         ...
    }
}
```   
&emsp;&emsp;这看起来没什么问题，但是如果存在多级列表呢？
```
选择操作()
{
    选择项目 = 显示列表();
    switch(选择项目)
    {
        case 0:
         显示二级列表1();
         break;
        case 1:
         显示二级列表2();
         break;
         ...
        default:
         ...
    }
}
```  
&emsp;&emsp;这看起来也没有什么问题，但是仔细想一下，一个列表选择完成后，如果选择操作不结束，那么这个函数永远都在运行中，如果是多级列表，那么未完成操作的函数都将驻留内存，这对内存资源，尤其是栈资源是一种极大的消耗。    
&emsp;&emsp;解决多级画面内存开销大、方便多画面管理、尽可能隔离画面显示业务与数据处理业务，解决这三大问题才是SimpleGUI的HMI模型存在的真正意义。  
&emsp;&emsp;以下HMI模型的设计逻辑：    

  * 同一时刻，活动在内存中的画面对象只有一个。    
  * 所有画面的逻辑级别一致，不存在层级区别。    
  * 事件的投送不区别画面逻辑，统一发送给当前画面，是否予以响应由画面对象的事件处理函数决定。    

&emsp;&emsp;由于HMI模型中大量使用函数指针进行操作，理解起来可能会比较晦涩，但是如果充分理解以上三点，HMI模型的处理将会变得非常简单。    
&emsp;&emsp;接下来将简要剖析VirtualSDK中的演示程序中对HMI模型的使用，以此讲解HMI模型的使用方法。    
&emsp;&emsp;首先是HMI引擎的类型声明：    
```c++
typedef struct
{
    HMI_SCREEN_OBJECT** ScreenObjPtr;
    SGUI_INT            ScreenCount;
    HMI_SCREEN_OBJECT*  CurrentScreenObject;
    SGUI_SCR_DEV*       Interface;
}HMI_ENGINE_OBJECT;
```   

&emsp;&emsp;由上可知，一个HMI模型中包含以下四部分：    

  * 画面对象列表。    
  * 画面对象总数。    
  * 当前活动画面对象指针。
  * 设备驱动对象指针。    

&emsp;&emsp;然后是DemoProc.c文件中对HMI引擎实例的声明，由于HMI引擎中也包含其他数据结构，所以所有HMI对象相关的声明都在这里：    

```c++
SGUI_SCR_DEV g_stDeviceInterface;
HMI_SCREEN_OBJECT* g_arrpstScreenObjs[] =
{
    &g_stHMIDemo_ScrollingText,
    &g_stHMIDemo_List,
    &g_stHMIDemo_TextNotice,
    &g_stHMIDemo_RTCNotice,
    &g_stHMIDemo_VariableBox,
    &g_stHMI_DemoRealtimeGraph,
};
HMI_ENGINE_OBJECT g_stDemoEngine;
```    

&emsp;&emsp;接下来是DemoProc.c文件中的DemoMainProcess函数，该函数用于模拟在目标平台上的main函数，是演示历程的起点。    

```c++
int DemoMainProcess(void)
{
    // Initialize Engine.
    InitializeEngine();
    while(1)
    {
        // Check and process heart-beat timer event.
        if(true == SGUI_SDK_GetEventSyncFlag(ENV_FLAG_IDX_SDK_TIM_EVENT))
        {
            SysTickTimerEventProc();
            SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_TIM_EVENT, false);
        }
        // Check and process key press event.
        if(true == SGUI_SDK_GetEventSyncFlag(ENV_FLAG_IDX_SDK_KEY_EVENT))
        {
            KeyPressEventProc();
            SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_KEY_EVENT, false);
        }
        // Check and process RTC event.
        if(true == SGUI_SDK_GetEventSyncFlag(ENV_FLAG_IDX_SDK_RTC_EVENT))
        {
            RTCEventProc();
            SGUI_SDK_SetEvnetSyncFlag(ENV_FLAG_IDX_SDK_RTC_EVENT, false);
        }
    }
	return 0;
}
```    

&emsp;&emsp;然后进入InitializeEngine函数，此函数用于HMI引擎对象的初始化。之前声明的HMI引擎实例和相关的其他实例都将在这里初始化。首先是设备驱动对象：    

```c++
SGUI_SystemIF_MemorySet(&g_stDeviceInterface, 0x00, sizeof(SGUI_SCR_DEV));
/* Initialize display size. */
g_stDeviceInterface.stSize.Width = 128;
g_stDeviceInterface.stSize.Height = 64;
/* Initialize interface object. */
g_stDeviceInterface.fnSetPixel = SGUI_SDK_SetPixel;
g_stDeviceInterface.fnGetPixel = SGUI_SDK_GetPixel;
g_stDeviceInterface.fnClearScreen = SGUI_SDK_ClearDisplay;
g_stDeviceInterface.fnRefreshScreen = SGUI_SDK_RefreshDisplay;
```    

&emsp;&emsp;接下来是画面对象：    

```c++
SGUI_SystemIF_MemorySet(&g_stDemoEngine, 0x00, sizeof(HMI_ENGINE_OBJECT));
/* Prepare HMI engine object. */
g_stDemoEngine.ScreenCount = sizeof(g_arrpstScreenObjs)/sizeof(*g_arrpstScreenObjs);
g_stDemoEngine.ScreenObjPtr = g_arrpstScreenObjs;
g_stDemoEngine.Interface = &g_stDeviceInterface;
```    

&emsp;&emsp;至此，HMI模型实例的相关初始化都已经完成，接下来，需要对所有的画面对象进行初始化，初始化方法区别于各个画面业务的不同有所不同，但都可以通过画面动作对象中的Initialize函数指针调用：    

```c++
for(iIndex=0; iIndex<g_stDemoEngine.ScreenCount; iIndex++)
{
    if( (NULL != g_stDemoEngine.ScreenObjPtr[iIndex])
        && (NULL != g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions)
        && (g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions->Initialize)
        )
    {
        g_stDemoEngine.ScreenObjPtr[iIndex]->pstActions->Initialize(&g_stDeviceInterface);
        g_stDemoEngine.ScreenObjPtr[iIndex]->pstPrevious = NULL;
    }
}   
```    

&emsp;&emsp;这里有一个细节需要注意，每一个画面的pstPrevious成员都需要初始化为NULL。在画面业务处理中，此指针指向业务逻辑中当前画面的前一画面。如果此指针不为空，那么将不能切换至该画面（不可重入）。    
&emsp;&emsp;这样设计是因为，如果画面迁移顺序为A->B->C->B->D->E，那么根据SimpleGUI的设计特性，每一个画面都有专门的对象记录相关数据，那么第二次进入B画面时，B画面的数据就会被修改，而返回上一画面时，数据也只能保留最后一次修改的数据。所以，在HMI中要求，只要业务逻辑不同，就不能使用同一个画面对象，即使这两个对象的显示效果完全一致。
&emsp;&emsp;在HMI引擎和所有画面对象都初始化完成后，就可以调用HMI_ActiveEngine函数登录并激活HMI引擎对象。    

```c++
/* Active engine object. */
eProcessResult = HMI_ActiveEngine(&g_stDemoEngine, HMI_SCREEN_ID_DEMO_SCROLLING_TEXT);
```   

&emsp;&emsp;激活时，可以通过参数指定哪一个画面为起始画面，如果指定的画面找不到，那么HMI将会以画面对象列表中的第一个画面对象为起始画面，同时，HMI引擎对象的CurrentScreenObject指针将会被设定，指向起始画面。    
&emsp;&emsp;以上所有动作都完成后，调用HMI_StartEngine函数开始显示起始画面，如果画面显示时需要其他数据作为参量，可以通过参数将参量数据的指针传递至画面对象。    

```c++
/* Start engine process. */
eProcessResult = HMI_StartEngine(NULL);
```    

&emsp;&emsp;至此，如果设备驱动程序正常，屏幕上将会显示第一个画面了。    
&emsp;&emsp;由于之前HMI引擎的CurrentScreenObject成员已经在HMI_ActiveEngine函数中被设定，此后，所有的消息处理都将无视画面实例，直接操作这个指针成员。例程中的列表演示为例，假设我们当前正在列表画面中，这时键盘上的回车键按下，程序将会进入到KeyPressEventProc函数中。

```c++
void KeyPressEventProc(void)
{
	KEY_PRESS_EVENT		stEvent;
	HMI_EVENT_INIT(stEvent);

	stEvent.Head.iType = EVENT_TYPE_ACTION;
	stEvent.Head.iID = EVENT_ID_KEY_PRESS;
	stEvent.Data.uiKeyValue = SGUI_SDK_GetKeyEventData();
	HMI_ProcessEvent((HMI_EVENT_BASE*)(&stEvent));
}
```    

&emsp;&emsp;此函数将调用HMI_ProcessEvent函数，将按键事件传递到CurrentScreenObject指针指向的画面实例中。    

```c++
HMI_ENGINE_RESULT HMI_ProcessEvent(const HMI_EVENT_BASE* pstEvent)
{
    ......
    eProcessResult = pstCurrentScreen->pstActions->ProcessEvent(g_pstActivedEngineObject->Interface, pstEvent, &iActionID);
    ......
    eProcessResult = pstCurrentScreen->pstActions->PostProcess(g_pstActivedEngineObject->Interface, eProcessResult, iActionID);
    ......
    if( (NULL != g_pstActivedEngineObject->Interface) &&
        (NULL != g_pstActivedEngineObject->Interface->fnRefreshScreen))
    {
        g_pstActivedEngineObject->Interface->fnRefreshScreen();
    }
    ......
    return eProcessResult;
}
```    

&emsp;&emsp;上述代码只保留了关键部分，详细请参考HMI_Engine.c文件中HMI_ProcessEvent函数的实现。    
&emsp;&emsp;由上述代码段可见，事件实例以指针的形式传递到了CurrentScreenObject指针指向的画面实例中，在事件处理函数ProcessEvent执行完毕后，又调用了PostProcess函数进行“后处理”，然后调用设备实例的fnRefreshScreen函数更新画面显示（更新画面显示的操作通常在显示设备使用显示缓存的情况下才需要实现并调用）。具体的事件响应处理，请参照前文针对事件的讲解，参考DemoProc文件夹中的src/List.c文件中事件处理函数HMI_DemoList_ProcessEventh的实现。    
&emsp;&emsp;在事件处理完成后，HMI引擎同时调用了PostProcess指针，执行了画面的“后处理函数”。所谓“后处理”，就是根据事件的响应结果，执行一些h后续需要的处理，如画面切换、数据更新等等。    
&emsp;&emsp;下面是事件处理和后处理函数的原型声明：

```c++
HMI_ENGINE_RESULT (*ProcessEvent)(SGUI_SCR_DEV* Interface, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);

HMI_ENGINE_RESULT (*PostProcess)(SGUI_SCR_DEV* Interface, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)；
```    

&emsp;&emsp;后处理函数的第一个参数为显示设备对象，不做过多解释。第二个参数eProcResult为事件处理函数的执行结果，如果没有发生异常，通常为HMI_RET_NORMAL, 第三个参数iActionID为用户自由使用的参数，来源为事件处理函数的最后一个出口参数piActionID，用来传递一些事件处理函数产生的结果。    
&emsp;&emsp;以列表的演示代码为例，HMI_DemoList_PostProcessh函数的处理如下：    

```c++
HMI_ENGINE_RESULT HMI_DemoList_PostProcess(SGUI_SCR_DEV* pstIFObj, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    ......
    if(HMI_DEMO_PROC_CONFIRM == iActionID)
    {
        uiSelectListIndex = s_stDemoListObject.ControlVariable.SelectIndex;
        switch(SGUI_List_GetListItemPtr(&s_stDemoListObject, uiSelectListIndex)->ItemID)
        {
            case 1:
            {
                // Show notice.
                pstSelectedItem = SGUI_List_GetListItemPtr(&s_stDemoListObject, uiSelectListIndex);
                iListItemParameterValue = pstSelectedItem->Valid.Value;
                if(0 == iListItemParameterValue)
                {
                    sprintf(s_szNoticeTextBuffer, DEMO_LIST_NOTICE_TEXT_FMT, uiSelectListIndex);
                    HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_TEXT_NOTICE, s_szNoticeTextBuffer);
                }
                else
                {
                    HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_RTC_NOTICE, NULL);
                }
                break;
            }
            case 5:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_VARIABLE_BOX, NULL);
                break;
			}
            case 6:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH, NULL);
                break;
            }
            default:
            {
                sprintf(s_szNoticeTextBuffer, DEMO_LIST_NOTICE_TEXT_FMT, uiSelectListIndex);
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_TEXT_NOTICE, s_szNoticeTextBuffer);
                break;
            }
        }
    }
    else if(HMI_DEMO_PROC_CANCEL == iActionID)
    {
        HMI_GoBack(NULL);
    }

    return HMI_RET_NORMAL;
}
```    

&emsp;&emsp;由上可以看出，列表演示程序的后处理中，根据用户选择的列表项的不同，会调用HMI引擎的HMI_SwitchScreen函数切换到相应的画面显示，或调用HMI_GoBack函数返回之前的画面。    
&emsp;&emsp;HMI_SwitchScreen函数的主要实现如下：
```c++
HMI_ENGINE_RESULT HMI_SwitchScreen(SGUI_INT iDestScreenID, const void* pstParameters)
{
    ......
    pstDestScreen = HMI_GetScreenObjectInEngine(g_pstActivedEngineObject, iDestScreenID);
    ......
    pstDestScreen->pstPrevious = g_pstActivedEngineObject->CurrentScreenObject;
    ......
    g_pstActivedEngineObject->CurrentScreenObject = pstDestScreen;
    ......
    eProcessResult = pstDestScreen->pstActions->Prepare(g_pstActivedEngineObject->Interface, pstParameters);
	......
    return eProcessResult;
}
```    

&emsp;&emsp;HMI_SwitchScreen函数将根据用户指定的ID，查找相应的画面对象，将找到的画面对象的前画面指针指向当前画面，并将HMI实例的CurrentScreenObject指向新的画面实例，然后调用新实例的Prepare函数完成画面准备与切换，HMI_GoBack函数原理类似，不同点在于画面实例的指针来自于当前画面的pstPrevious成员而非通过ID查找新实例。    
&emsp;&emsp;以上就是HMI引擎的工作原理，SimpleGUI的HMI引擎的特点就是画面的所有操作无需等待，运行后立即退出，不长期占用栈空间，对内存栈资源的开销相对较小。同时可以便捷的通过HMI引擎对画面和功能进行扩充，可维护性较强。    
&emsp;&emsp;其他详细的函数处理与实现，请参考源代码。    

<返回到[目录](#0)/[标题](#3)>

<h2 id="4">4. SimpleGUI配置</h2>   

&emsp;&emsp;SimpleGUI的全局配置依赖一些宏定义开关来实现，这些宏定义集中在SGUI_Config.h文件中，分为“VirtualSDK用”、“SimpleGUI接口用”和“演示程序用”三大部分。  

### 4.1 VirtualSDK用    
* \_SIMPLE\_GUI\_ENCODE\_TEXT\_    
&emsp;&emsp;此宏定义生效时，将启用ICONV转码接口。由于代码文件均使用UTF-8编码保存，所以在编码时若使用了基本ASCII以外的字符时，就需要在文字显示接口中对应相应的解码方法，已获取对应文字的缩写。    
&emsp;&emsp;以中文为例，通常使用的时GB2312或GB10801编码（SimpleGUI默认情况下对非ASCII的解析就是以GB2312编码方式为准），而代码文件由于是UTF-8格式，所以在编译时也是以UTF-8格式的字节序列编译的，同样，在运行时也是以UTF-8编码使用的，这是如果还使用GB2312方式进行解码，就会出现乱码。启用此宏定义后，SimpleGUI的API在显示文字前将先对文字进行转码，然后再从字库中索引文字数据。    
* \_SIMPLE\_GUI\_ENCODE\_TEXT\_SRC\_    
&emsp;&emsp;此宏定义了ICONV编码转换的源编码，默认为UTF-8格式。仅在\_SIMPLE\_GUI\_ENCODE\_TEXT\_生效的前提下有效。    
* \_SIMPLE\_GUI\_ENCODE\_TEXT\_DEST\_    
&emsp;&emsp;此宏定义了ICONV编码转换的目标编码，默认为GB2312格式。仅在\_SIMPLE\_GUI\_ENCODE\_TEXT\_生效的前提下有效。    
* \_SIMPLE\_GUI\_IN\_VIRTUAL\_SDK\_    
&emsp;&emsp;此宏定义的声明意味着程序将运行于SimpleGUI的VirtualSDK中，在此情况下将可以使用一些VirtualSDK提供的一些模拟硬件资源（中断、用户输入等）的接口。    
&emsp;&emsp;此宏定义的生效与否请更具实际情况谨慎设定，否则可能导致运行时异常。    

### 4.2 SimpleGUI接口用    
* \_SIMPLE\_GUI\_ENABLE\_DYNAMIC\_MEMORY\_    
&emsp;&emsp;此宏定义的使能需要内存池动态管理的支持。当此宏定义使能时，意味着SGUI_SystemIF_Allocate和SGUI_SystemIF_Freeh函数可以使用，对应malloc和free函数，可以动态的申请和释放堆上内存。  
&emsp;&emsp;此宏定义使能后，将SimpleGUI中依赖动态内存的接口和功能将可以使用。目前只有列表项目的动态添加和删除。    
* \_SIMPLE\_GUI\_USE\_SYS\_PF\_FUNCTIONS\_    
&emsp;&emsp;此宏定义被使能时，SimpleGUI再使用的一些系统函数时将直接调用对应的系统函数，例如SGUI_SystemIF_StringLengthCopy对应strncpy，SGUI_SystemIF_StringCopy对应strcpy，SGUI_SystemIF_GetNowTime中需要使用localtimeh函数等，如果此宏没有被定义，则响应的功能需要使用者自行实现。    

### 4.3 演示程序用     
&emsp;&emsp;此部分宏定义仅在Demo中使用，用户在开发中可以无视此部分的宏定义。
* \_SIMPLE\_GUI\_NON\_ASCII\_    
&emsp;&emsp;SimpleGUI内部包含一个演示用的字库，其中包含6\*8像素的ASCII字符、6\*12像素的ASCII和GB2312字符以及8\*16像素的ASCII和GB2312字符。为了节约Flash空间，使Demo程序可以运行在Flash空间不足的芯片中，只有当此宏被定义时，才能使用ASCII以外的字符。


<h2 id="5">5. 其他应用技巧</h2> 

### 5.1 HMI引擎切换  
&emsp;&emsp;在使用SimpleGUI的HMI引擎时，如果有必要在系统中依据不同的条件进入不同的交互系统时，可以创建多个HMI对象，并使用HMI_ActiveEngineh函数激活对应的HMI对象。    
&emsp;&emsp;本质上，HMI引擎在HMI_Engine.c文件中声明了一个指向HMI_ENGINE_OBJECT类型实例的内部指针变量，指向的HMI引擎对象就是当前被激活的HMI引擎，通过HMI_ActiveEngine函数可以将这个指针重新设定并调用新指定HMI引擎中当前活动画面的Prepare方法。进行切换后的初次显示。    

### 5.2 多显示设备  
&emsp;&emsp;如果系统中需要支持多个显示屏时，可以针对不同的显示屏定义多个设备驱动对象。在需要切换不同的显示设备进行相应控制时，可以使用HMI引擎提供的HMI_SetDeviceObject函数为当前活动的HMI引擎设定新的设备驱动对象，从而实现在多屏环境下的区别控制。

<返回到[目录](#0)/[标题](#4)>

<h2 id="6">6. 使用限制</h2> 

&emsp;&emsp;SimpleGUI的GUI部分和HMI部分目前均没有进行线程安全设计，在使用中强烈建议将SimpleGUi的额画面处理放在同一个线程中进行，避免多线程或在中断函数中调用GUI或HMI接口函数，除非在设计期间您可以严格保证使用SimpleGUI处理部分的绝对线程安全。


<返回到[目录](#0)/[标题](#5)>

<h2 id="7">7. 联系开发者</h2> 

&emsp;&emsp;首先，感谢您对SimpleGUI的赏识与支持。
&emsp;&emsp;虽然最早仅仅作为一套GUI接口库使用，但我最终希望SimpleGUI能够为您提供一套完整的单色屏GUI及交互设计解决方案，如果您有新的需求、提议亦或想法，欢迎在以下地址留言，或加入[QQ交流群799501887](https://jq.qq.com/?_wv=1027&k=5ahGPvK)留言交流。  
>SimpleGUI@开源中国：https://www.oschina.net/p/simplegui  
>SimpleGUI@码云：https://gitee.com/Polarix/simplegui  

&emsp;&emsp;本人并不是全职的开源开发者，依然有工作及家庭的琐碎事务要处理，所以对于大家的需求和疑问反馈的可能并不及时，多有怠慢，敬请谅解。    
&emsp;&emsp;最后，再次感谢您的支持。
