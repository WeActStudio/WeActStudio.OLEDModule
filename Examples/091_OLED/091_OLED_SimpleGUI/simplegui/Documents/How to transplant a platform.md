<h1 id="0">目录</h1>

* [1. 概述](#1) 
* [2. 向STM32F103平台上移植Demo程序](#2) 
* [3. 从“Hello world”开始](#3) 
* [4. HMI交互模型](#4) 
* [5. 联系开发者](#5)  
---
<h2 id="1">1. 概述</h2>

&emsp;&emsp;为了方便大家学习和使用SimpleGUI，我对Demo程序向STM32F103芯片上进行了移植并保留了工程代码，本文将演示如何搭建并使用这个工程。  

<返回到[目录](#0)/[标题](#1)>    

<h2 id="2">2. 向STM32F103平台上移植Demo程序</h2> 

#### 2.1. 概述。    

&emsp;&emsp;本Demo程序本身并没有什么意义，单纯的就是使用SimpleGUI的绝大多数接口，写的一个样例，用于让大家了解SimpleGUI的运行效果和大概的使用方法。    
&emsp;&emsp;由于演示程序需要操作交互，在VirtualSDK中可以直接响应键盘操作，但是大家使用的开发板各有差异，有没有按键、有几个按键都是未知数，所以最终Demo程序设计使用串口模拟按键交互动作。至于屏幕硬件则是选用了目前网上保有量较多，而且也容易购买和获取开发资料的12864 OLED显示屏，主控制器为SSD1306，近期又出现了新的SSD1385主控，软件上是与SSD1306兼容的，可直接替换。    
&emsp;&emsp;Demo程序基于STM32编写，除了OLED显示屏外没有使用任何其他的外部硬件，片内资源使用了串口控制器、基础定时器和RTC时钟，如果使用其他型号的芯片，需要根据片上资源和配置做出相应修改。

#### 2.2. 准备工程模板    

&emsp;&emsp;在移植和使用Demo程序之前，请先依照个人习惯，创建一个空白的STM32F103工程。工程中请不要包含诸如串口重定向等的代码实现，以避免不必要的编译错误。

&emsp;&emsp;由于演示工程中包含GB2312的字库，所以编译后体积较大，请大家在移植时，最好使用STM32F103VE/ZE芯片，以确保Flash资源的充足。当然，这并不是硬性需求，如果您确实需要在少资源的芯片中运行，后续将有介绍如何使Demo运行于诸如STM32F103C8这样的资源相对较少的芯片，毕竟SimpleGUI本身对系统资源的消耗并不高。

#### 2.3. 移植Demo程序。    

&emsp;&emsp;首先，确认硬件连接，演示工程使用SPI接口与OLED显示屏进行通讯，接口定义如下：    
```  
OLED_SCLK--------PB13
OLED_MOSI--------PB15
OLED_CS----------PB12
OLED_DC----------PB11
OLED_RST---------PB10
```    
&emsp;&emsp;如果需要修改引脚定义，请进入SSD1306_SPI.c文件中进行修改，引脚定义的映射位于文件头部。为方便随意更改GPIO，驱动程序使用软件SPI编写。
&emsp;&emsp;接下来将SimpleGUI根目录下的GUI、HMI、DemoProc和Transplant文件夹下的STM32F1文件夹复制到准备好的空白工程中，然后打开工程。    

>![01-准备工程文件](https://images.gitee.com/uploads/images/2019/0605/225932_b7cbc466_769424.png "01-准备工程文件.png")

&emsp;&emsp;接下来，将工程中的main函数删除，因为Demo程序中已经写好了main函数。
&emsp;&emsp;然后将GUI目录、HMI目录、DemoProc目录和STM32F1目录下src文件夹中所有的c文件添加到工程。  

>![02-添加文件到工程](https://images.gitee.com/uploads/images/2019/0605/230006_7301f48b_769424.png "02-添加文件到工程.png")

&emsp;&emsp;至此，所有文件添加完成，上图中的User组中的内容就是STM32F1文件夹中的文件。
&emsp;&emsp;然后将GUI目录、HMI目录、DemoProc目录和STM32F1目录下inc文件夹添加到工程的包含路径(Include path)中。
    
>![03-添加包含路径](https://images.gitee.com/uploads/images/2019/0605/230028_179a33b0_769424.png "03-添加包含路径.png")  

&emsp;&emsp;接下来打开GUI\\inc目录下的SGUI_Config.h文件，大致内容如下：    

```c++  
//=======================================================================//
//= Used for SimpleGUI virtual SDK.									    =//
//=======================================================================//
#define _SIMPLE_GUI_ENCODE_TEXT_
#ifdef _SIMPLE_GUI_ENCODE_TEXT_
 #define _SIMPLE_GUI_ENCODE_TEXT_SRC_		("UTF-8")
 #define _SIMPLE_GUI_ENCODE_TEXT_DEST_		("GB2312")
#endif // _SIMPLE_GUI_ENCODE_TEXT_
#define _SIMPLE_GUI_IN_VIRTUAL_SDK_

//=======================================================================//
//= Used for SimpleGUI interface.									    =//
//=======================================================================//
#define _SIMPLE_GUI_ENABLE_DYNAMIC_MEMORY_
#define _SIMPLE_GUI_ENABLE_BASIC_FONT_

//=======================================================================//
//= Used for SimpleGUI demo process.								    =//
//=======================================================================//
#define _SIMPLE_GUI_NON_ASCII_
```
&emsp;&emsp;此文件中定义了SimpleGUI中用于全局配置的宏定义，默认状态下是为了在VirtualSDK环境下运行而配置的，所以直接编译会产生一些错误。为了适配STM32F1平台，我们对文件项目做如下修改：

 - 将宏\_SIMPLE\_GUI\_ENCODE\_TEXT\_的定义注释掉。    
 - 将宏\_SIMPLE\_GUI\_IN\_VIRTUAL\_SDK\_的定义注释掉。    
 - 将宏\_SIMPLE\_GUI\_ENABLE\_DYNAMIC\_MEMORY\_的定义注释掉。    

&emsp;&emsp;此修改完成如下图：    
>![04-编辑Config文件](https://images.gitee.com/uploads/images/2019/0605/230044_80806f21_769424.png "04-编辑Config文件.png")

&emsp;&emsp;然后保存、重新编译工程。  

>![05-编译](https://images.gitee.com/uploads/images/2019/0605/230107_a423aca4_769424.png "05-编译.png")

&emsp;&emsp;由于包含有12像素和16像素的中文字库，所以编译的程序比较大，这也就是为什么选用STM32F103VE/ZE这样的大容量芯片运行Demo的原因了。接下来将程序下载到芯片，下载完成后，OLED显示屏上就可以看到显示内容了。  

>![06-演示程序运行](https://images.gitee.com/uploads/images/2019/0605/230139_ee01cc19_769424.png "06-运行.png")

#### 2.4. Demo程序的交互。    

&emsp;&emsp;为了确保所有尝试使用和学习SimpleGUI的朋友都能以最快速，最便捷的方式体验和使用SimpleGUI，Demo程序使用串口来模拟键盘进行Demo程序的交互，以此来避免在使用不同开发板或最小系统时，因为硬件原因导致交互实现起来困难的尴尬。    
&emsp;&emsp;完成前文描述的移植操作并成功烧录到芯片中运行，在确保芯片的串口正确连接到电脑后，就可以尝试与SimpleGUI的Demo程序进行交互了。在此之前，请准保好一个您习惯使用的串口调试软件，但此软件需要支持发送十六进制数据。
&emsp;&emsp;基于前文的操作，确保芯片硬件系统处于上电运行状态，显示屏上滚动显示关于SimpleGUI的简介内容。此时使用串口发送十六进制数据0x0020模拟空格按键操作，此时何以看到，SimpleGUI的Demo程序已经显示列表画面了。  

>![07-列表画面](https://images.gitee.com/uploads/images/2019/0605/230152_82b96512_769424.png "07-列表画面.png")

&emsp;&emsp;其他的案件动作，请参考演示工程中的DemoActions.h文件，文件中对一些常用的按键功能进行了定义，这些定义是与在VirtualSDK环境中运行时一致的，只不过是用串口来代替键盘输入而已，每次发送两个字节。    

```c++  
#define		KEY_VALUE_NONE									(0x0000)
#define		KEY_VALUE_TAB									(0x0009)
#define		KEY_VALUE_ENTER									(0x000D)
#define		KEY_VALUE_ESC									(0x001B)
#define		KEY_VALUE_SPACE									(0x0020)
#define		KEY_VALUE_LEFT									(0x013A)
#define		KEY_VALUE_UP									(0x013B)
#define		KEY_VALUE_RIGHT									(0x013C)
#define		KEY_VALUE_DOWN									(0x013D)
#define		KEY_VALUE_F1									(0x0154)
#define		KEY_VALUE_F2									(0x0155)
#define		KEY_VALUE_F3									(0x0156)
#define		KEY_VALUE_F4									(0x0157)
#define		KEY_VALUE_F5									(0x0158)
#define		KEY_VALUE_F6									(0x0159)
#define		KEY_VALUE_F7									(0x015A)
#define		KEY_VALUE_F8									(0x015B)
#define		KEY_VALUE_F9									(0x015C)
#define		KEY_VALUE_F10									(0x015D)
#define		KEY_VALUE_F11									(0x015E)
#define		KEY_VALUE_F12									(0x015F)
#define		KEY_VALUE_ENTER_PAD								(0x0172)
// User option flag value define
#define		KEY_OPTION_CTRL									(0x1000)
#define		KEY_OPTION_ALT									(0x2000)
#define		KEY_OPTION_SHIFT								(0x4000)

#define		KEY_CODE_VALUE(CODE)							(CODE & 0x0FFF)
#define		KEY_CODE_OPT(CODE)								(CODE & 0xF000)
```

&emsp;&emsp;需要注意的是，由于SimpleGUI的列表每一个项目都可以带有自己的参数，且类别可以不同，所以操作时需要配合Alt、Ctrl、Shift这样的功能键，每一次按键操作的两字节（16位）数据中，低13位代表按键码，高3位代表功能键码，例如针对列表中带有小数时，Demo程序可以通过Shift+左右箭头移动小数点，例如向左移动小数点，需要模拟Shift+左箭头动作，则发送KEY_VALUE_LEFT|KEY_OPTION_SHIFT(0x4000|0x013A)即0x413A即可。    
&emsp;&emsp;同理，如果想发送Alt+Shift+回车，则发送KEY_OPTION_ALT|KEY_OPTION_SHIFT|KEY_VALUE_ENTER(0x2000|0x4000|0x000D)即0x600D即可。    

#### 2.5在资源较少的芯片上运行。    
&emsp;&emsp;SimpleGUI的Demo程序对资源的消耗主要体现在中文字库对片内Flash的消耗上，如果您手上暂时没有类似STM32F103ZE/VE这样的大容量芯片，可以考虑精简掉一部分字库，以实现在容量较小的芯片上运行。      
&emsp;&emsp;打开之前操作过的SGUI\_Config.h文件，将宏定义\_SIMPLE\_GUI\_NON\_ASCII\_的定义注释掉，就可以时中文字库的代码定义失能，此时再次编译整个工程，可以发现，资源消耗明显减少。    
  
>![08-精简编译](https://images.gitee.com/uploads/images/2019/0605/230212_403c143d_769424.png "08-精简编译.png")

&emsp;&emsp;当然，精简掉中文字库，也就不可能再显示中文文字了，所以编译时Demo程序绘制用另外一个资源文件，运行时界面上也只显示英语。    

>![09-精简编译的运行效果](https://images.gitee.com/uploads/images/2019/0605/230225_5f7263b0_769424.png "09-精简代码.png")

&emsp;&emsp;以下是相同的代码在STM32F103C8T6上的运行效果。

>![10-相同的代码在C8T6上运行](https://images.gitee.com/uploads/images/2019/0605/230244_a3bd94d4_769424.png "10-精简代码在C8T6上运行.png")

&emsp;&emsp;为了兼容在VirtualSDK中运行、片上全字库运行和片上精简字库运行等多种情况，Demo程序中使用了DemoResource_UTF8.h、DemoResource_GB2312.h和DemoResource_ASCII.h三个文件作为显示用的资源文件，Demo程序中显示的文字均为同名不同内容的宏定义，在不同环境下根据不同的宏开关、引用不同的文件来实现切换不同显示内容的效果。    
&emsp;&emsp;为了避免不同情况下的乱码，在VirtualSDK中运行时，使用的资源文件是DemoResource_UTF8.h，运行时通过iconv转换库将字符串转换为GB2312编码再进行解码和显示。 而由于在芯片上运行全字库时，由于没有足够的资源保存转码库的资源，所以使用DemoResource_GB2312.h文件，此文件本身就以GB2312形式保存，无需转码。而使用精简字库时，由于只有ASCII文字，所以使用DemoResource_ASCII.h文件，使界面显示英语，不使用中文字库。   

<返回到[目录](#0)/[标题](#2)>    

<h2 id="3">3. 从“Hello world”开始</h2>     

#### 3.1准备驱动程序    
&emsp;&emsp;由于SimpleGUI本身不提供屏幕设备的驱动程序，而是借助既有的硬件设备接口组织基础的绘制操作，所以在使用SimpleGUI之前，需要开发者自行准备好屏幕设备的驱动函数。至少准备好“初始化”、“读像素”和“写像素”两个函数，函数的原型需要严格按照以下形式声明和定义。    
```
void        OLED_Initialize(void);
void        OLED_SetPixel(int iPosX, int iPosY, int iColor);
int			OLED_GetPixel(int iPosX, int iPosY);
```
&emsp;&emsp;
   
<返回到[目录](#0)/[标题](#3)>

<h2 id="3">4. HMI交互模型</h2>    

<返回到[目录](#0)/[标题](#4)>

<h2 id="3">5. 联系开发者</h2>    

&emsp;&emsp;首先，感谢您对SimpleGUI的赏识与支持。
&emsp;&emsp;虽然最早仅仅作为一套GUI接口库使用，但我最终希望SimpleGUI能够为您提供一套完整的单色屏GUI及交互设计解决方案，如果您有新的需求、提议亦或想法，欢迎在以下地址留言，或加入[QQ交流群799501887](https://jq.qq.com/?_wv=1027&k=5ahGPvK)留言交流。  
>SimpleGUI@开源中国：https://www.oschina.net/p/simplegui  
>SimpleGUI@码云：https://gitee.com/Polarix/simplegui  

&emsp;&emsp;本人并不是全职的开源开发者，依然有工作及家庭的琐碎事务要处理，所以对于大家的需求和疑问反馈的可能并不及时，多有怠慢，敬请谅解。    
&emsp;&emsp;最后，再次感谢您的支持。     
