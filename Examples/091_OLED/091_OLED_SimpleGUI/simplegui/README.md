# SimpleGUI

---

#### 简介
SimpleGUI是一款针对单色显示屏设计的GUI接口库。
>![SimpleGUI在SSD1306主控制器的OLED显示屏上的显示效果](https://images.gitee.com/uploads/images/2018/0725/220230_cfc1d5f8_769424.jpeg "SimpleGUI_1.jpg")

关于SimpleGUI的故事，请参见：
>https://gitee.com/Polarix/simplegui/blob/master/Introduction.md

#### 设计目标
在尽可能减小资源消耗的前提下，提供以下功能：

1. 点、线、基本几何图形、单色位图、文字等的绘制功能。
2. 列表、进度条、滚动条、提示框、曲线图等部件的设置元显示功能。
3. 单色显示屏模拟环境，方便脱离硬件平台进行部分GUI开发。

#### 部署模拟器
SimpleGUI提供了一个单色显示屏的模拟器以方便开发者进行基于SimpleGUI的开发，模拟器的详细的使用方法请参照Documents文件夹下的“模拟器环境配置.pdf”文件。
>预览效果
>
>![模拟器预览1](https://images.gitee.com/uploads/images/2018/0725/220159_f29eefb9_769424.jpeg "SimpleGUI_3.jpg")
>![模拟器预览2](https://images.gitee.com/uploads/images/2018/0725/220220_d506c7bd_769424.jpeg "SimpleGUI_2.jpg")

#### 移植

SimpleGUI的移植方法，可以参照Documents文件夹下的“移植范例.pdf”文件，里面详细阐述了SimpleGUI在MDK5环境下移植到STM32F1平台下的方法和移植要点，其他平台也可参照。

#### 关于SimpleGUI
SimpleGUI发布已经有很长时间了，在发布后很多人都来询问一些技术细节以及提出一些宝贵的意见与建议，但是很多建议和SimpleGUI的设计初衷相悖，所以我并没有采纳，关于这一点，还请大家谅解。所以在这里，详细阐述一下SimpleGUI的设计理念和主导思想。

首先，SimpleGUI面向的是单色显示屏，目前市面上的单色显示屏一般12864和19264居多，最大的应该也就240160的样子，但不太常见。这种屏幕因为像素密度不高，单屏幕能显示的内容有限，所以，面向此类平台的SimpleGUI就舍弃了诸如图层、窗口、阴影等的实现，希望能用尽可能少的元素，兼顾美观的同时在屏幕上表现尽可能多的内容。

其次，SimpleGUI的另一个目标就是轻量化，希望能尽可能减少对资源的消耗，所以暂时没有引入对文件字库、非等宽字体等特性的支持。作为SimpleGUI的设计和开发者，我的初衷是希望尽可能减少开发人员在一些简单地事情上浪费时间，转而投入到更有意义的事情上去。

另外，我正在致力于开发一款文字取模的工具，类似PCtoLCD2002那种，以方便开发者可以定制最符合自身需求的字库。

#### 视频讲解  

为了方便大家进一步了解SimpleGUI并快速上手，我录制了一些视频供大家参考。  

|内容|地址|  
|:- |----|  
|01 SimpleGUI概述|https://www.bilibili.com/video/av86593220/|  
|02 基础绘图|https://www.bilibili.com/video/av86890300/|  
|03 文本文字|https://www.bilibili.com/video/av87098997/|  
|04 拓展组件概述|https://www.bilibili.com/video/av87432375/|  
|05 交互引擎HMI|https://www.bilibili.com/video/av87530421/|  
|06 VirtualSDK概述|https://www.bilibili.com/video/av87713369/|  
|07 基于VirtualSDK的GUI开发|https://www.bilibili.com/video/BV1qz4y12771/| 

#### 参与讨论
如果您有意参与SimpleGUI的讨论与改进，可以在以下地方留言：

1. 开源中国SimpleGUI页面：https://www.oschina.net/p/simplegui
2. 码云页面：https://gitee.com/Polarix/simplegui
3. QQ交流群：799501887