SimpleGUI为了方便开发者进行开发或预开发作业，特设计了PC端模拟环境用以辅助开发，模拟环境使用Codeblocks构建，图形界面基于wxWidgets建立。。

2018年12月10日，wxWidgets更新了3.1.2版本，我也下载了最新的源码包准备重新编译更新一下wxWidgets库，但在这个过程中遇到了一些问题，解决后整理出来，与诸位共享。

在这次版本更新之前，根据Readme和百度必应查来的一些资料，wxWidgets的编译命令如下。

`mingw32-make -f makefile.gcc MONOLITHIC=0 SHARED=0 UNICODE=1 BUILD=release CXXFLAGS="-fno-keep-inline-dllexport -std=gnu++11" `

这是按照我个人的使用习惯的编译命令，选项为：

`MONOLITHIC=0：使用分立库`
`SHARED=0：使用静态库`
`UNICODE=1：使用Unicode编码`

这个命令我保存在一个.cmd文件里，使用很久了从3.0.3就开始，一直用到3.1.1版本。但是3.1.2的时候，出现问题了。

在这之前，有必要介绍一下我的系统环境，因为这个可能影响到这个问题发生的现象和结果。

出现问题的系统环境如下：

`OS：Windows7 Professional SP1`
`编译器：TDM-GCC 32Bit`

按照之前版本一样的操作，先解压缩wxWidgets，启动Windows的CMD，进入到wxWidgets的build目录，然后执行前文说过的编译命令。通常情况下，单线程编译一次大概需要30分钟左右。

但是这次，我没有等来编译完成的提示，而是得到了一条错误：

`ar rcu ..\..\lib\gcc_lib\libwxmsw31u_core.a gcc_mswu\corelib_dummy.o gcc_mswu\corelib_event.o gcc_mswu\corelib_fs_mem.o gcc_mswu\corelib_msgout.o gcc_mswu\corelib_utilscmn.o gcc_mswu\corelib_main.o gcc_mswu\corelib_volume.o gcc_mswu\corelib_activex.o gcc_mswu\corelib_app.o gcc_mswu\corelib_bitmap.o gcc_mswu\corelib_brush.o gcc_mswu\corelib_caret.o gcc_mswu\corelib_clipbrd.o gcc_mswu\corelib_colour.o gcc_mswu\corelib_cursor.o gcc_mswu\corelib_data.o gcc_mswu\corelib_dc.o gcc_mswu\corelib_dcclient.o gcc_mswu\corelib_dcmemory.o gcc_mswu\corelib_dcprint.o gcc_mswu\corelib_dcscreen.o gcc_mswu\corelib_dialup.o gcc_mswu\corelib_dib.o gcc_mswu\corelib_display.o gcc_mswu\corelib_enhmeta.o gcc_mswu\corelib_font.o gcc_mswu\corelib_fontenum.o gcc_mswu\corelib_fontutil.o gcc_mswu\corelib_gdiimage.o gcc_mswu\corelib_gdiobj.o gcc_mswu\corelib_gdiplus.o gcc_mswu\corelib_graphics.o gcc_mswu\corelib_icon.o gcc_mswu\corelib_imaglist.o gcc_mswu\corelib_minifram.o gcc_mswu\corelib_nonownedwnd.o gcc_mswu\corelib_comimpl.o gcc_mswu\corelib_dataobj.o gcc_mswu\corelib_dropsrc.o gcc_mswu\corelib_droptgt.o gcc_mswu\corelib_oleutils.o gcc_mswu\corelib_safearray.o gcc_mswu\corelib_palette.o gcc_mswu\corelib_pen.o gcc_mswu\corelib_popupwin.o gcc_mswu\corelib_printdlg.o gcc_mswu\corelib_printwin.o gcc_mswu\corelib_region.o gcc_mswu\corelib_renderer.o gcc_mswu\corelib_settings.o gcc_mswu\corelib_textmeasure.o gcc_mswu\corelib_tooltip.o gcc_mswu\corelib_toplevel.o gcc_mswu\corelib_uiaction.o gcc_mswu\corelib_utilsgui.o gcc_mswu\corelib_utilswin.o gcc_mswu\corelib_uxtheme.o gcc_mswu\corelib_window.o gcc_mswu\corelib_graphicsd2d.o gcc_mswu\corelib_utilsrt.o gcc_mswu\corelib_taskbarcmn.o gcc_mswu\corelib_helpchm.o gcc_mswu\corelib_taskbar.o gcc_mswu\corelib_helpwin.o gcc_mswu\corelib_joystick.o gcc_mswu\corelib_aboutdlg.o gcc_mswu\corelib_notifmsg.o gcc_mswu\corelib_richtooltip.o gcc_mswu\corelib_sound.o gcc_mswu\corelib_automtn.o gcc_mswu\corelib_notifmsgrt.o gcc_mswu\corelib_uuid.o gcc_mswu\corelib_evtloop.o gcc_mswu\corelib_clrpickerg.o gcc_mswu\corelib_collpaneg.o gcc_mswu\corelib_filepickerg.o gcc_mswu\corelib_fontpickerg.o gcc_mswu\corelib_generic_statusbr.o gcc_mswu\corelib_prntdlgg.o gcc_mswu\corelib_msw_accel.o gcc_mswu\corelib_msw_anybutton.o gcc_mswu\corelib_appprogress.o gcc_mswu\corelib_artmsw.o gcc_mswu\corelib_msw_bmpbuttn.o gcc_mswu\corelib_msw_button.o gcc_mswu\corelib_msw_checkbox.o gcc_mswu\corelib_msw_choice.o gcc_mswu\corelib_colordlg.o gcc_mswu\corelib_combo.o gcc_mswu\corelib_msw_combobox.o gcc_mswu\corelib_msw_control.o gcc_mswu\corelib_msw_dialog.o gcc_mswu\corelib_dirdlg.o gcc_mswu\corelib_dragimag.o gcc_mswu\corelib_filedlg.o gcc_mswu\corelib_frame.o gcc_mswu\corelib_msw_gauge.o gcc_mswu\corelib_headerctrl.o gcc_mswu\corelib_iniconf.o gcc_mswu\corelib_msw_listbox.o gcc_mswu\corelib_msw_listctrl.o gcc_mswu\corelib_mdi.o gcc_mswu\corelib_msw_menu.o gcc_mswu\corelib_menuitem.o gcc_mswu\corelib_metafile.o gcc_mswu\corelib_msgdlg.o gcc_mswu\corelib_nativdlg.o gcc_mswu\corelib_nativewin.o gcc_mswu\corelib_msw_notebook.o gcc_mswu\corelib_access.o gcc_mswu\corelib_ownerdrw.o gcc_mswu\corelib_progdlg.o gcc_mswu\corelib_msw_radiobox.o gcc_mswu\corelib_msw_radiobut.o gcc_mswu\corelib_richmsgdlg.o gcc_mswu\corelib_msw_scrolbar.o gcc_mswu\corelib_msw_slider.o gcc_mswu\corelib_msw_spinbutt.o gcc_mswu\corelib_spinctrl.o gcc_mswu\corelib_msw_statbmp.o gcc_mswu\corelib_msw_statbox.o gcc_mswu\corelib_statusbar.o gcc_mswu\corelib_msw_statline.o gcc_mswu\corelib_msw_stattext.o gcc_mswu\corelib_taskbarbutton.o gcc_mswu\corelib_msw_toolbar.o gcc_mswu\corelib_msw_textctrl.o gcc_mswu\corelib_textentry.o gcc_mswu\corelib_msw_tglbtn.o gcc_mswu\corelib_treectrl.o gcc_mswu\corelib_systhemectrl.o gcc_mswu\corelib_customdraw.o gcc_mswu\corelib_animateg.o gcc_mswu\corelib_commandlinkbutton.o gcc_mswu\corelib_bmpcbox.o gcc_mswu\corelib_hyperlink.o gcc_mswu\corelib_calctrl.o gcc_mswu\corelib_datectrl.o gcc_mswu\corelib_datetimectrl.o gcc_mswu\corelib_timectrl.o gcc_mswu\corelib_datecontrols.o gcc_mswu\corelib_activityindicator.o gcc_mswu\corelib_msw_checklst.o gcc_mswu\corelib_msw_fdrepdlg.o gcc_mswu\corelib_fontdlg.o gcc_mswu\corelib_accelcmn.o gcc_mswu\corelib_accesscmn.o gcc_mswu\corelib_anidecod.o gcc_mswu\corelib_affinematrix2d.o gcc_mswu\corelib_appcmn.o gcc_mswu\corelib_artprov.o gcc_mswu\corelib_artstd.o gcc_mswu\corelib_arttango.o gcc_mswu\corelib_bmpbase.o gcc_mswu\corelib_bmpbtncmn.o gcc_mswu\corelib_bookctrl.o gcc_mswu\corelib_btncmn.o gcc_mswu\corelib_cairo.o gcc_mswu\corelib_checkboxcmn.o gcc_mswu\corelib_checklstcmn.o gcc_mswu\corelib_choiccmn.o gcc_mswu\corelib_clipcmn.o gcc_mswu\corelib_clrpickercmn.o gcc_mswu\corelib_colourcmn.o gcc_mswu\corelib_colourdata.o gcc_mswu\corelib_combocmn.o gcc_mswu\corelib_cmdproc.o gcc_mswu\corelib_cmndata.o gcc_mswu\corelib_containr.o gcc_mswu\corelib_cshelp.o gcc_mswu\corelib_ctrlcmn.o gcc_mswu\corelib_ctrlsub.o gcc_mswu\corelib_dcbase.o gcc_mswu\corelib_dcbufcmn.o gcc_mswu\corelib_dcgraph.o gcc_mswu\corelib_dcsvg.o gcc_mswu\corelib_dirctrlcmn.o gcc_mswu\corelib_dlgcmn.o gcc_mswu\corelib_dndcmn.o gcc_mswu\corelib_dobjcmn.o gcc_mswu\corelib_docmdi.o gcc_mswu\corelib_docview.o gcc_mswu\corelib_dpycmn.o gcc_mswu\corelib_dseldlg.o gcc_mswu\corelib_effects.o gcc_mswu\corelib_fddlgcmn.o gcc_mswu\corelib_filectrlcmn.o gcc_mswu\corelib_filehistorycmn.o gcc_mswu\corelib_filepickercmn.o gcc_mswu\corelib_fontpickercmn.o gcc_mswu\corelib_fldlgcmn.o gcc_mswu\corelib_fontcmn.o gcc_mswu\corelib_fontdata.o gcc_mswu\corelib_graphicc.o gcc_mswu\corelib_fontenumcmn.o gcc_mswu\corelib_fontmap.o gcc_mswu\corelib_fontutilcmn.o gcc_mswu\corelib_framecmn.o gcc_mswu\corelib_gaugecmn.o gcc_mswu\corelib_gbsizer.o gcc_mswu\corelib_gdicmn.o gcc_mswu\corelib_geometry.o gcc_mswu\corelib_gifdecod.o gcc_mswu\corelib_graphcmn.o gcc_mswu\corelib_headercolcmn.o gcc_mswu\corelib_headerctrlcmn.o gcc_mswu\corelib_helpbase.o gcc_mswu\corelib_iconbndl.o gcc_mswu\corelib_imagall.o gcc_mswu\corelib_imagbmp.o gcc_mswu\corelib_image.o gcc_mswu\corelib_imagfill.o gcc_mswu\corelib_imaggif.o gcc_mswu\corelib_imagiff.o gcc_mswu\corelib_imagjpeg.o gcc_mswu\corelib_imagpcx.o gcc_mswu\corelib_imagpng.o gcc_mswu\corelib_imagpnm.o gcc_mswu\corelib_imagtga.o gcc_mswu\corelib_imagtiff.o gcc_mswu\corelib_imagxpm.o gcc_mswu\corelib_layout.o gcc_mswu\corelib_lboxcmn.o gcc_mswu\corelib_listctrlcmn.o gcc_mswu\corelib_markupparser.o gcc_mswu\corelib_matrix.o gcc_mswu\corelib_menucmn.o gcc_mswu\corelib_modalhook.o gcc_mswu\corelib_mousemanager.o gcc_mswu\corelib_nbkbase.o gcc_mswu\corelib_overlaycmn.o gcc_mswu\corelib_ownerdrwcmn.o gcc_mswu\corelib_paper.o gcc_mswu\corelib_panelcmn.o gcc_mswu\corelib_persist.o gcc_mswu\corelib_pickerbase.o gcc_mswu\corelib_popupcmn.o gcc_mswu\corelib_preferencescmn.o gcc_mswu\corelib_prntbase.o gcc_mswu\corelib_quantize.o gcc_mswu\corelib_radiobtncmn.o gcc_mswu\corelib_radiocmn.o gcc_mswu\corelib_rearrangectrl.o gcc_mswu\corelib_rendcmn.o gcc_mswu\corelib_rgncmn.o gcc_mswu\corelib_scrolbarcmn.o gcc_mswu\corelib_settcmn.o gcc_mswu\corelib_sizer.o gcc_mswu\corelib_slidercmn.o gcc_mswu\corelib_spinbtncmn.o gcc_mswu\corelib_spinctrlcmn.o gcc_mswu\corelib_srchcmn.o gcc_mswu\corelib_statbar.o gcc_mswu\corelib_statbmpcmn.o gcc_mswu\corelib_statboxcmn.o gcc_mswu\corelib_statlinecmn.o gcc_mswu\corelib_stattextcmn.o gcc_mswu\corelib_stockitem.o gcc_mswu\corelib_tbarbase.o gcc_mswu\corelib_textcmn.o gcc_mswu\corelib_textentrycmn.o gcc_mswu\corelib_textmeasurecmn.o gcc_mswu\corelib_toplvcmn.o gcc_mswu\corelib_treebase.o gcc_mswu\corelib_uiactioncmn.o gcc_mswu\corelib_valgen.o gcc_mswu\corelib_validate.o gcc_mswu\corelib_valtext.o gcc_mswu\corelib_valnum.o gcc_mswu\corelib_wincmn.o gcc_mswu\corelib_windowid.o gcc_mswu\corelib_wrapsizer.o gcc_mswu\corelib_xpmdecod.o gcc_mswu\corelib_busyinfo.o gcc_mswu\corelib_buttonbar.o gcc_mswu\corelib_choicdgg.o gcc_mswu\corelib_choicbkg.o gcc_mswu\corelib_collheaderctrlg.o gcc_mswu\corelib_combog.o gcc_mswu\corelib_dcpsg.o gcc_mswu\corelib_dirctrlg.o gcc_mswu\corelib_dragimgg.o gcc_mswu\corelib_filectrlg.o gcc_mswu\corelib_headerctrlg.o gcc_mswu\corelib_infobar.o gcc_mswu\corelib_listbkg.o gcc_mswu\corelib_logg.o gcc_mswu\corelib_markuptext.o gcc_mswu\corelib_msgdlgg.o gcc_mswu\corelib_numdlgg.o gcc_mswu\corelib_progdlgg.o gcc_mswu\corelib_preferencesg.o gcc_mswu\corelib_printps.o gcc_mswu\corelib_renderg.o gcc_mswu\corelib_richmsgdlgg.o gcc_mswu\corelib_scrlwing.o gcc_mswu\corelib_selstore.o gcc_mswu\corelib_spinctlg.o gcc_mswu\corelib_splitter.o gcc_mswu\corelib_srchctlg.o gcc_mswu\corelib_statbmpg.o gcc_mswu\corelib_stattextg.o gcc_mswu\corelib_textdlgg.o gcc_mswu\corelib_tipwin.o gcc_mswu\corelib_toolbkg.o gcc_mswu\corelib_treectlg.o gcc_mswu\corelib_treebkg.o gcc_mswu\corelib_vlbox.o gcc_mswu\corelib_vscroll.o gcc_mswu\corelib_xmlreshandler.o gcc_mswu\corelib_splash.o gcc_mswu\corelib_notifmsgg.o gcc_mswu\corelib_odcombo.o gcc_mswu\corelib_calctrlcmn.o gcc_mswu\corelib_grideditors.o gcc_mswu\corelib_bmpcboxcmn.o gcc_mswu\corelib_grid.o gcc_mswu\corelib_gridctrl.o gcc_mswu\corelib_hyperlinkg.o gcc_mswu\corelib_helpext.o gcc_mswu\corelib_sashwin.o gcc_mswu\corelib_gridsel.o gcc_mswu\corelib_addremovectrl.o gcc_mswu\corelib_tipdlg.o gcc_mswu\corelib_aboutdlgg.o gcc_mswu\corelib_gridcmn.o gcc_mswu\corelib_richtooltipcmn.o gcc_mswu\corelib_datectlg.o gcc_mswu\corelib_bannerwindow.o gcc_mswu\corelib_treelist.o gcc_mswu\corelib_datavcmn.o gcc_mswu\corelib_animatecmn.o gcc_mswu\corelib_odcombocmn.o gcc_mswu\corelib_hyperlnkcmn.o gcc_mswu\corelib_propdlg.o gcc_mswu\corelib_bmpcboxg.o gcc_mswu\corelib_richtooltipg.o gcc_mswu\corelib_timectrlg.o gcc_mswu\corelib_commandlinkbuttong.o gcc_mswu\corelib_notifmsgcmn.o gcc_mswu\corelib_wizard.o gcc_mswu\corelib_datavgen.o gcc_mswu\corelib_editlbox.o gcc_mswu\corelib_laywin.o gcc_mswu\corelib_calctrlg.o`
`makefile.gcc:5358: recipe for target '..\..\lib\gcc_lib\libwxmsw31u_core.a' failed`
`ar: gcc_mswu\corelib_makuptext.o: No such file or directory`
`mingw32-make: *** [..\..\lib\gcc_lib\libwxmsw31u_core.a] Error 1`

错误很简单，但是也很奇怪，错误提示是找不到“gcc_mswu\corelib_makuptext.o”文件，应该是目录或者文件名不对，奇怪也奇怪在这个文件，不论是编译的.o文件，还是Makefile中的记录，都找不到一个名字为corelib_makuptext的文件，唯一比较相近的就是markuptext.cpp和corelib_markuptext.o这种文件名缺个字母的事儿，我还真的第一次遇到。

我寻思在网上找一下，看有没有人遇到过类似的错误，但是很遗憾，没找到……

不过也算是有了意外收获，因为在一篇发布于2015年的博客里提到了一个错误，是因为命令行太长，博客的地址如下，有兴趣的可自行阅读或参考：[MinGW编译wxWidgets中的问题及解决方法](https://www.cnblogs.com/tr0217/p/4804943.html)

上网查一下，控制台命令参数的最大程度为8192，排除掉结尾字符也就是8191字，回过头再看看控制台回显，这个命令行确实不短，复制出来一看，总长度9834……

我觉得我可能找到问题了，再重新定位一下，第8191个字符刚好在corelib_markuptext.o文件名中。

我将上面的一长串命令单独执行一下，发生了同样的错误！

知道了问题的可能所在，就该着手解决了，我写了一个简单的测试用.cmd文件，将上面的一长串命令拆解为两个命令：

`ar rcu [一部分.o文件]`
`ar ru [另一部分.o文件]`

至于为什么第一句的参数是rcu，第二句是ru，请自行查询ar的参数含义。

然后执行这个.cmd文件，编译成功了，libwxmsw31u_core.a被正确生成了！

至此，已经能明确知道，这个错误就是因为命令参数过长导致的，至于GCC的ar命令是如何读取参数字符串的我没有仔细考证，但解决方法就是将命令拆开使用。

针对此问题，分析Makefile如下：

此错误发生在编译libwxmsw31u_core.a时，此编译对象被分为两个部分，一部分为“CORELIB_OBJECTS”自身的.o文件，共7个。另外还包含”____CORE_SRC_FILENAMES_3_OBJECTS“编译对象，视WXUNIV使能控制为338个或322个。

而静态库文件在编译时，直接使用了以下命令：

`ar rcu $@ $(CORELIB_OBJECTS)`

导致命令行超长，编译失败。

所以，针对此问题，makefile的修改如下：

1、新建“CORE_BUILD_OBJ_PART_0”，定义，包含“CORELIB_OBJECTS”中“CORE_SRC_FILENAMES_3_OBJECTS”以外的所有内容。

``CORE_BUILD_OBJ_PART_0 =  \`
​	`$(OBJS)\corelib_dummy.o \`
​	`$(OBJS)\corelib_event.o \`
​	`$(OBJS)\corelib_fs_mem.o \`
​	`$(OBJS)\corelib_msgout.o \`
​	`$(OBJS)\corelib_utilscmn.o \`
​	`$(OBJS)\corelib_main.o \`
​	`$(OBJS)\corelib_volume.o``

2、新建"CORE_SRC_FILENAMES_3_OBJECTS_PART_0"和"CORE_SRC_FILENAMES_3_OBJECTS_PART_1"将原有“CORE_SRC_FILENAMES_3_OBJECTS”变量中的文件分别包含，大概五五开的样子就可以。

3、清空“CORE_SRC_FILENAMES_3_OBJECTS”变量中的内容，包含刚才新建的"CORE_SRC_FILENAMES_3_OBJECTS_PART_0"和"CORE_SRC_FILENAMES_3_OBJECTS_PART_1"两个变量。

`____CORE_SRC_FILENAMES_3_OBJECTS =  \`
​	`$(CORE_SRC_FILENAMES_3_OBJECTS_PART_0) \`
​	`$(CORE_SRC_FILENAMES_3_OBJECTS_PART_1)` 

4、清空“CORELIB_OBJECTS”中的内容，包含前文新建的“CORE_BUILD_OBJ_PART_0”、"CORE_SRC_FILENAMES_3_OBJECTS_PART_0"和"CORE_SRC_FILENAMES_3_OBJECTS_PART_1"三个变量。

`CORELIB_OBJECTS =  \`
​	`$(CORE_BUILD_OBJ_PART_0) \`
​	`$(CORE_SRC_FILENAMES_3_OBJECTS_PART_0) \`
​	`$(CORE_SRC_FILENAMES_3_OBJECTS_PART_1)`

5、修改编译命令为：

`ar rcu $@ $(CORE_BUILD_OBJ_PART_0)`
`ar ru $@ $(CORE_SRC_FILENAMES_3_OBJECTS_PART_0)`
`ar ru $@ $(CORE_SRC_FILENAMES_3_OBJECTS_PART_1)` 

 完成如上操作后，重新编译，编译动作顺利完成。

另外，如果编译的命令参数MONOLITHIC为1的时候，在编译最终的链接库时候，也会因为命令行过长出现类似问题，由于使用的编译模式不同，目前只能用“gcc_mswudll\monodll\*.o”替代“$(MONODLL_OBJECTS)”临时解决，暂时还没想到更好的办法，修改Makefile的话，修改量可能会很大。其实帖子开头的问题也可以用类似的办法搞定，把“$(CORELIB_OBJECTS)”替换为“gcc_mswu\corelib\*.o”就好，而且我也确认过makefile的内容，这样改没有问题，但是还是会担心漏掉什么，所以才有了上文的该法。

虽然问题解决了，但是我还有个问题，像这种维护了很长时间而且比较有影响力的开源项目，不会这么不负责任把有问题的环境放上来，那么他们是怎么编译过的呢？而且官网也有编译好的二进制文件放出，不应该有问题。Windows的命令行参数长度固定为8192，超过这个值一定会出问题，那么一定是他们的发布并不是在Windows上验证的，很有可能是在Linux上用Mingw编译。

于是我把我装有Ubuntu Core开发板上电点亮，控制台上输入如下命令：

`getconf ARG_MAX`

然后我看到了一个丧病的数字：

`2097152`

好吧，真相就是这样了，如果大大们有更好的解决方案，烦请告知。