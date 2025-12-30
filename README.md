WinInfo
=======

This tool provides window information similar to **Spy++** or the [AutoIt Window Information Tool](https://www.autoitscript.com/autoit3/docs/intro/au3spy.htm "AutoIt Window Information Tool")

Using WinInfo, move your mouse over the target window to see details about the control currently under the cursor.
You can also locate a control by browsing the tree in the *All Windows* tab.

![Main WinIfo window](/img/main_wnd.png)


Information that can be obtained includes:


| Property name | Description  |
| :------------ |:---------------|
| **Text**      | The text on a control, for example "&Next" on a button |
| **Class**     | The window class (see [this](http://msdn.microsoft.com/en-us/library/windows/desktop/ms633574%28v=vs.85%29.aspx)) |
| **ID**        | The child-window identifier (see description *hMenu* parameter in [CreateWindow](http://msdn.microsoft.com/en-us/library/windows/desktop/ms632679%28v=vs.85%29.aspx) function)|
| **Position**  | For the child-window this is a coordinates upper-left corner in root parent client area, for the root window this is a screen coordinates that are relative to the upper-left corner of the screen |
| **Size**      | The window size in pixels | 
| **Style**     | The window style (see [this](http://msdn.microsoft.com/en-us/library/windows/desktop/ms632600%28v=vs.85%29.aspx)) |
| **ExStyle**   | The extended window styles (see [this](http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543%28v=vs.85%29.aspx)) |
| **Handle**    | The window handle |

![Main WinIfo window](/img/main_wnd2.png)

