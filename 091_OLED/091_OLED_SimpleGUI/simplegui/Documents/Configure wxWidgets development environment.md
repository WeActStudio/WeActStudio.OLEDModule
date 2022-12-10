配置Codeblocks下的wxWidgets开发环境
---
## 1. 概述 

&emsp;&emsp;为了方便开发者在自己的项目中使用SimpleGUI部署、开发或试开发GUI模块，作者特设计此模拟环境，用于进行不依赖硬件平台的学习或开发。  
&emsp;&emsp;SimpleGUI Virtual SDK（以下简称SDK）使用C++语言开发，基于wxWidgets的GUI框架，工程建立于Code::Blocks集成开发环境中，可以于Windows XP或更高版本的操作系统中编译和运行。  

## 2. 基于GCC和Code::Blocks的SDK环境搭建 

#### 2.1. 下载Code::Blocks

&emsp;&emsp;请访问[Code::Blocks下载页面](http://www.codeblocks.org/downloads/binaries)下载Code::Blocks集成开发环境的可执行文件，截至本文档最后一次编辑，Code::Blocks集成开发环境的最新版本为17.12。  
&emsp;&emsp;由于Code::Blocks开发环境本身可以适配多种编译器，所以可下载选项有很多，如果对编译器并不了解，可以选择下载“codeblocks-17.12mingw-setup.exe”或“codeblocks-17.12mingw-nosetup.zip”，这两个版本自带TDM-GCC 5.1.0版本编译器，区别为前者为安装版，会自动创建文件类型关联，后者为绿色版，解压即用。  
&emsp;&emsp;后续讲解也以此编译器为例。
>![01-Codeblocks下载页面](https://images.gitee.com/uploads/images/2019/0117/131725_1b7479df_769424.png) 

#### 2.2. 部署wxWidgets

&emsp;&emsp;请访问[wxWidgets下载页面](http://www.wxwidgets.org/downloads/)，选择页面中“Latest Development Release: 3.1.2”或者“Latest Stable Release: 3.0.4”标题下“Binaries”中的“Download windows binaries”链接。
>![02-wxWidgets下载页面](https://images.gitee.com/uploads/images/2019/0117/131820_a6ed6bf6_769424.png)  

&emsp;&emsp;截至SimpleGUI最后一次更新，wxWidgets最新版本为3.0.4(稳定版)和3.1.2(开发版)，请各位根据自己的喜好下载并部署，由于3.0版本和3.1版本链接库的文件名不尽相同，所以SimpleGUI的VirtualSDK也准备了面向3.0版本和3.1版本的工程文件，请对应使用。  

&emsp;&emsp;为避免开发版本中存在的一些潜在问题，影响VirtualSDK的使用，在本说明中，将以wxWidgets的最新稳定版3.0.4为例进行说明。

&emsp;&emsp;在弹出的列表中下载对应的编译器使用的库，本例中使用Codeblocks自带的TDM-GCC 5.1.0本编译器。所以选择下载“MinGW-TDM5.10”下的头文件（Header Files）和开发用库文件（Development Files）。  

>![03-wxWidgets库的选择](https://images.gitee.com/uploads/images/2019/0117/232547_839319af_769424.png)   
> &emsp;&emsp;由于2018年12月10日wxWidgets3.1.2版本更新后，面向GCC编译器的Makefile无法在Windows环境下直接使用，所以从本次更新开始，仅讲解wxWidgets库文件的使用方法，不再详述wxWidgets库的编译过程，如果有这方面具体需求，请参考本人的博文[《wxWidgets3.1.2在Windows+TDM-GCC环境下编译错误的处理》](https://my.oschina.net/Polarix/blog/2996189)了解详细信息。  
> &emsp;&emsp;3.1.1及更早版本的源代码仍然可以直接在Windows下使用TDM-GCC编译。

&emsp;&emsp;下载后，请将下载的头文件和库文件解压在同一个文件夹中，解压的路径不宜太长，文件夹名中不要出现括号、下划线、空格等特殊字符，否则将可能导致程序编译时引用文件异常。本范例中部署路径为“E:\Workspace\Librarys\wxWidgets\ForTDM510”。解压后，请将库文件的目录名从“gccXXXXXX\_dll”修改为“gcc\_dll”。  
&emsp;&emsp;部署后的目录结构应为：  
```
    wxWidgets库目录  
      ├─ include  
      │  ├─ msvc  
      │  │  └─ wx  
      │  │     └─ *.h
      │  └─ wx  
      │     ├─ [DIR]
      │     └─ *.h
      └─ lib  
         └─ gcc_x86_dll(如果使用64位编译器，此文件夹名应为gcc_x64_dll)  
             ├─ mswu  
             └─ *.a(库文件) 
```
 

#### 2.3. 配置Code::Blocks下的wxWidgets开发环境

&emsp;&emsp;启动Code::Blocks，然后选择选择“Settings”（设置）菜单下的“Global variables”（全局变量）项目。  

>![04-Global variables选项](https://images.gitee.com/uploads/images/2019/0117/131842_f2e41830_769424.png)  

&emsp;&emsp;在打开的全局变量编辑窗口中，点击“New”按钮，新建一个全局变量“wxbin”。  

>![05-新建全局变量](https://images.gitee.com/uploads/images/2019/0117/232846_b419f48d_769424.png)  

&emsp;&emsp;然后将全局变量的根路径、包含路径和库路经分别设置为wxWidgets的根路径include路径和lib路径，其他留空即可，参考下图。
>全局变量配置  
>![06-全局变量配置](https://images.gitee.com/uploads/images/2019/0117/232905_babc192c_769424.png)  

&emsp;&emsp;然后点击“Close”关闭环境变量编辑窗口，至此，SimpleGUI模拟环境开发需要使用的Codeblocks+wxWidgets环境搭建完成。  

### 3. 编译和使用Virtual SDK
#### 3.1. 工程环境结构

&emsp;&emsp;您可以通过Git工具，从码云（Gitee）上将SimpleGUI的全部代码和资料同步到本地，如果您不想使用Git工具，也可以在[SimpleGUI工程页面](https://gitee.com/Polarix/simplegui)中点击“克隆/下载”按钮，在弹出的窗口中点击“下载ZIP”按钮下载整个工程的压缩包文件。
&emsp;&emsp;同步或下载解压SimpleGUI后，就可以使用SimpleGUI的Virtual SDK了，SimpleGUI的主目录结构和说明如下：  

|目录名|功能|  
|:- |:- |  
|DemoProc|SimpGUI的演示代码|  
|DemoProject|SimpleGUI的演示工程|  
|Documents|关于SimpleGUI的一些简要说明文档|  
|GUI|SimpleGUI的代码实现部分|  
|HMI|SimpleGUI的HMI模型实现部分|  
|VirtualSDK|Virtual SDK的工程及源码|  

#### 3.2. 编译Virtual SDK工程
&emsp;&emsp;进入VirtualSDK\Project\CodeBlocks目录，此目录中为SimpleGUI的VirtualSDK的工程文件，使用Code::Blocks打开，VirtualSDK_wx30.cbp对应使用wxWidgets的3.0.x版本库，VirtualSDK_wx31.cbp对应使用3.1.x版本库，请根据之前配置的wxWidgets版本选用。如果用户下载的是nosetup（绿色版）的Code::Blocks，需要手动建立关联才能直接用双击的方式打开cbp文件，否则，用户只能先打开Code::Blocks，然后从Code::Blocks中执行打开操作以打开工程文件。  
&emsp;&emsp;点击工具栏上的编译按钮或按快捷键Ctrl+F9，开始编译模拟环境演示工程。  
&emsp;&emsp;编译完成，显示无错误和警告，现在点击工具栏上的运行按钮或按快捷键Ctrl+F10就可以看到模拟环境的运行效果了。  
>![07-编译工程](https://images.gitee.com/uploads/images/2019/0117/131925_defb4a75_769424.png)  

>![08-运行效果](https://images.gitee.com/uploads/images/2019/0117/131942_a01e12c5_769424.png) 

#### 3.3. 配置Virtual SDK
&emsp;&emsp;默认情况下，SimpleGUI的Virtual SDK中虚拟显示屏幕的配色方案为黄底黑字的LCD点阵显示屏，为最大程度上模拟真实情况下的视觉效果，方便创建和调试GUI元素，模拟环境的虚拟LCD面板可以通过修改配置定义修改颜色和尺寸。
&emsp;&emsp;打开Virtual SDK工程，进入VirtualSDK\Common\inc文件夹（对应工程路径/SimpleGUI/Headers/VirtualSDK/Common/inc），打开Common.h文件，找到以下宏定义：  

|宏定义名|功能|    
|:- |:- |   
|LCD_COLOR_OBJ_PIX|逻辑有效像素点颜色（前景颜色）|  
|LCD_COLOR_OBJ_BKG|逻辑无效像素点颜色（背景颜色）|  
|LCD_COLOR_OBJ_GRID|网格颜色|  
|PARAM_DEFAULT_PIXEL_NUM_H|横向像素点数量|  
|PARAM_DEFAULT_PIXEL_NUM_V|纵向像素点数量|  
|PARAM_DEFAULT_PIXEL_WIDTH|屏幕像素单元宽度|  
|PARAM_DEFAULT_PIXEL_HEIGHT|屏幕像素单元高度|  

&emsp;&emsp;Virtual SDK默认状态下模拟的是黄底黑字的LCD显示屏，不显示像素网格。接下来以模拟黑底蓝字的OLED19264显示屏效果，简要介绍一下各个宏定义的使用方法：  
> - 使用一张目标屏幕的照片，使用取色工具获取屏幕背景、像素以及边框颜色的RGB值，比如淘宝上的照片，就可以直接使用。  
>![09-实物照片](https://images.gitee.com/uploads/images/2019/0117/233447_91a32ff3_769424.png) 
> - 将颜色的RGBA值分别更新入三个宏定义中，颜色使用32位无符号整数表示，从高至低四字节的意义分别为A、B、G、R。例如上图中屏幕背景色、像素色和边框色分别设定为0xFF070707、0xFFF1FA4F和0xFFC9652F，由于不需要显示网格，所以不需要修改网格颜色。
> - 分别修改PARAM\_DEFAULT\_PIXEL\_NUM\_H和PARAM\_DEFAULT\_PIXEL\_NUM\_V的值为192和64。
> - 修改\GUI\inc\SGUI_Common.h（对应工程路径/SimpleGUI/Headers/GUI/inc/SGUI_Common.h）文件中的SGUI_LCD_SIZE_WIDTH和SGUI_LCD_SIZE_HEIGHT的值为192和64。
> - 重新编译工程。  

&emsp;&emsp;重新编译后，即可看到效果，此时已经模拟为黑底蓝字的OLED显示屏。  
>![10-Virtual SDK模拟OLED显示屏](https://images.gitee.com/uploads/images/2019/0117/132051_954fd5c7_769424.png)  

&emsp;&emsp;上述的宏用于VirtualSDK\Common\src\Common.c（对应工程路径/SimpleGUI/Sources/VirtualSDK/Common/src/Common.c）文件中，载入设定参数的操作中。

    void SetDefaultParameterData(PixelPanelParameter* pstParameter)
    {
        if(NULL != pstParameter)
        {
            // Appearance
            pstParameter->HorizontalPixelNumber = PARAM_DEFAULT_PIXEL_NUM_H;
            pstParameter->VerticalPixelNumber = PARAM_DEFAULT_PIXEL_NUM_V;
            pstParameter->PixelUnitWidth = PARAM_DEFAULT_PIXEL_WIDTH;
		    pstParameter->PixelUnitHeight = PARAM_DEFAULT_PIXEL_HEIGHT;
            pstParameter->EdgeWidth = PARAM_DEFAULT_EDGE_WIDTH;
            pstParameter->EnableGrid = PARAM_DEFAULT_GRID_DISABLE;
            
            // ScreenColor
            pstParameter->PanelColor.RGBA = LCD_COLOR_OBJ_BKG;
            pstParameter->PixelColor.RGBA = LCD_COLOR_OBJ_PIX;
            pstParameter->EdgeColor.RGBA = LCD_COLOR_OBJ_EDGE;
            pstParameter->GridColor.RGBA = LCD_COLOR_OBJ_GRID;
        }
    }

&emsp;&emsp;此函数将在Virtual SDK启动时配置LCD面板的相关参数，如果想要使能像素网格，可以将：

            pstParameter->EnableGrid = PARAM_DEFAULT_GRID_DISABLE;

修改为：

            pstParameter->EnableGrid = PARAM_DEFAULT_GRID_ENABLE;
同时修改屏幕像素尺寸为4或4以上

            pstParameter->PixelSize = 4;
    
重新编译后，效果如下：  
>![11-Virtual SDK模拟OLED显示屏（带网格）](https://images.gitee.com/uploads/images/2019/0117/132125_7336e715_769424.png)   

&emsp;&emsp;需要注意的是，由于网格线显示时占用1像素，为了避免画面显示失调，SimpleGUI的Virtual  SDK仅在LCD像素点尺寸（也就是宏定义PARAM_DEFAULT_PIXEL_SIZE）的值大于4时，网格设定才会有效，在像素点尺寸小于4时，网格线将被强制关闭。  

#### 3.4. 使用Virtual SDK  
&emsp;&emsp;SimpleGUI提供了一个虚拟入口，用于模拟SimpleGUI在目标平台下的简易动作。  
&emsp;&emsp;Virtual SDK在VirtualSDK\Interface\src\SDKInterface.c（对应工程路径/SimpleGUI/Sources/VirtualSDK/Common/src/Common.c）文件中提供了以下接口。  

|接口函数|功能概要|    
|:--|:- |  
|SGUI_SDK_DummyMainProc|Virtual SDK的虚拟主入口函数，模拟Main函数。|   
|SGUI_SDK_SetEvnetSyncFlag|设定事件信号标记，通常由Virtual SDK自身调用，用于在模拟心跳定时器、RTC定时器或按键事件触发时设定标记用。|  
|SGUI_SDK_SyncKeyEventData|同步按键事件，用于保存消息触发时的键值。|   
|SGUI_SDK_GetEventSyncFlag|获取事件信号标记，通常由用户程序调用，用于读取和查询模拟心跳定时器、RTC定时器或按键是否触发。|   
|SGUI_SDK_GetKeyEventData|获取按键事件触发的键值，通常由用户程序调用。|  
|SGUI_SDK_Initialize|初始化Virtual SDK虚拟设备，通常由用户程序调用。|  
|SGUI_SDK_ConfigHearBeatTimer|配置虚拟心跳定时器，通常由用户程序调用，单位毫秒，参数为0时定时器停止。|  
|SGUI_SDK_EnableRTCInterrupt|配置虚拟RTC定时器使能。|  
|SGUI_SDK_SetPixel|写LCD像素值（仅显示缓存）|  
|SGUI_SDK_GetPixel|读LCD像素值（仅显示缓存）|  
|SGUI_SDK_RefreshDisplay|更新显示内容|  
|SGUI_SDK_ClearDisplay|清除屏幕显示|   

&emsp;&emsp;Virtual SDK中除了虚拟的显示屏外，还提供了连个定时器可供使用，分别为“模拟心跳定时器”和
“RTC”定时器。  

 - 模拟心跳定时器
&emsp;&emsp;受用户编写的模拟程序控制，以指定的时间间隔触发或停止，用于携带一些测试用模拟数据或实现简易动画效果等，例如演示历程起始画面中文字滚动的效果，以及实时曲线中曲线数据的携带都依赖此定时器实现。
 - 模拟RTC定时器
&emsp;&emsp;受用户编写的模拟程序控制，可启动或停止，每1秒触发一次，可用于秒级计时或更新系统时间用。

&emsp;&emsp;各函数的详细使用方法请参照演示历程。

### 4. 联系开发者
&emsp;&emsp;首先，感谢您对SimpleGUI的赏识与支持。
&emsp;&emsp;虽然最早仅仅作为一套GUI接口库使用，但我最终希望SimpleGUI能够为您提供一套完整的单色屏GUI及交互设计解决方案，如果您有新的需求、提议亦或想法，欢迎在以下地址留言，或加入[QQ交流群799501887](https://jq.qq.com/?_wv=1027&k=5ahGPvK)留言交流。  
>SimpleGUI@开源中国：https://www.oschina.net/p/simplegui  
>SimpleGUI@码云：https://gitee.com/Polarix/simplegui  

&emsp;&emsp;本人并不是全职的开源开发者，依然有工作及家庭的琐碎事务要处理，所以对于大家的需求和疑问反馈的可能并不及时，多有怠慢，敬请谅解。
&emsp;&emsp;最后，再次感谢您的支持。  
