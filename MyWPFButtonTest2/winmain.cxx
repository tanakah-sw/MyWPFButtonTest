// winmain.cxx

#include <tchar.h>
#include <stdio.h>
#define _USE_MATH_DEFINES // M_PIを有効に
#include <math.h>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <windows.h>

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#pragma comment(linker, "\"/manifestdependency:type='Win32' "\
    "name='Microsoft.Windows.Common-Controls' version='6.0.0.0' "\
    "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma warning(disable: 4996) // warning safe_stdio
#pragma warning(disable: 4100) // 引数は関数の本体部で1度も参照されません。
#pragma warning(disable: 4102) // ラベルは1度も参照されません。

#include "mainwin.h"

[System::STAThreadAttribute]
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
  INITCOMMONCONTROLSEX icex;
  icex.dwSize=sizeof(INITCOMMONCONTROLSEX); icex.dwICC=ICC_COOL_CLASSES|ICC_WIN95_CLASSES;
  InitCommonControlsEx(&icex);

  LoadLibrary("RichEd32.dll");
  
  mainWin *mainwin=new mainWin(hInstance);
  MSG msg;
  bool done=false;
  while(!done){
    if(PeekMessage(&msg,0, 0, 0, PM_NOREMOVE)){
      if(msg.message==WM_QUIT){
        done=TRUE;
      }else if(GetMessage(&msg, NULL, 0, 0)>0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

 winend:
  delete mainwin;
  return 0;
}
