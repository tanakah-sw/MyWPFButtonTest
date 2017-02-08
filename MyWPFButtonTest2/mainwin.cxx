// mainwin.cxx

#include <tchar.h>
#include <stdio.h>
#define _USE_MATH_DEFINES // M_PIを有効に
#include <math.h>

#include <map>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <commctrl.h>
//#pragma comment(lib, "comctl32.lib")

#include <richedit.h>

#include "mainwin.h"
#include "resource.h"

#pragma warning(disable: 4996) // warning safe_stdio

mainWin::mainWin(HINSTANCE hAppInstance)
{
  // メンバ変数初期化
  m_hAppInstance=hAppInstance;
  m_hdlg=NULL;

  //  GetCurrentDirectory(MAX_PATH, homeFolderName);
  GetModuleFileName(NULL, homeFolderName, MAX_PATH);
  PathRemoveFileSpec(homeFolderName);

  m_hdlg=CreateDialogParam(hAppInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL,
                           (DLGPROC)dlgProcMain, (LONG_PTR)this);
  InvalidateRect(m_hdlg, NULL, FALSE);
  UpdateWindow(m_hdlg);
}

mainWin::~mainWin()
{
}

namespace ManagedCode
{
  using namespace System;
  using namespace System::Windows;
  using namespace System::Windows::Interop;
  using namespace System::Windows::Media;
  
  IntPtr ChildHwndSourceHook(IntPtr, int msg, IntPtr, IntPtr, bool% handled)
  {
    if(msg==WM_GETDLGCODE){
      handled = true;
      return IntPtr(DLGC_WANTCHARS);
    }

    if(msg==WM_COMMAND){
      ::MessageBox(NULL, "in", NULL, MB_OK);
    }

    return IntPtr(0);
  }

  HWND GetHwnd(HWND parent, int x, int y, int width, int height){
    HwndSource^ source;
    source=gcnew HwndSource(
                            0, // class style
                            WS_VISIBLE | WS_CHILD, // style
                            0, // exstyle
                            x, y, width, height,
                            "xaml", // NAME
                            IntPtr(parent)        // parent window 
                            );
    source->AddHook(gcnew HwndSourceHook(ChildHwndSourceHook));

    UIElement^ page;
    page=gcnew WPFPage::ButtonTest();

    source->RootVisual=page;

    return (HWND)source->Handle.ToPointer();
  }
}

LRESULT CALLBACK mainWin::dlgProcMain(HWND hdlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
  mainWin *_this=(mainWin *)GetWindowLongPtr(hdlg, GWLP_USERDATA);
  switch(msg){
  case WM_COMMAND:
    {
      switch(LOWORD(wparam)){
      case IDC_BUTTON_TEST:
        strcpy(_this->MText, "test");
        _this->writeInformation(false);
        return TRUE;
        break;
      case IDC_BUTTON_QUIT:
      case IDM_QUIT:
        DestroyWindow(hdlg);
        return TRUE;
        break;
        
      }
    }
    break;

  case WM_INITDIALOG:
    {
      _this=(mainWin *)lparam;
      SetWindowLongPtr(hdlg, GWLP_USERDATA, (LONG_PTR)_this);

      HWND placeholder=GetDlgItem(hdlg, IDC_STATIC_WPF);
      RECT rectangle; 
      GetWindowRect(placeholder, &rectangle);

      POINT point;
      int width, height;
      
      point.x=rectangle.left;
      point.y=rectangle.top;
      width=rectangle.right-rectangle.left;
      height=rectangle.bottom-rectangle.top;
      MapWindowPoints(NULL, hdlg, &point, 1);
      ShowWindow(placeholder, SW_HIDE);
      
      ManagedCode::GetHwnd(hdlg, point.x, point.y, width, height);
      
      return TRUE;
    }
    break;

  case WM_CLOSE:
    DestroyWindow(hdlg);
    return TRUE;
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    return TRUE;
    break;

  }
  return FALSE;
}

void mainWin::writeInformation(bool caution)
{
  HWND hwndRE=GetDlgItem(m_hdlg, IDC_RICHEDIT1);
  
  int oldstart, oldend;
  SendMessage(hwndRE, EM_GETSEL, (WPARAM)&oldstart, (LPARAM)&oldend);

  const int nlinebuf=1024;
  unsigned char line[nlinebuf];
  int lines=(int)SendMessage(hwndRE, EM_GETLINECOUNT, 0, 0);
  int len=0;
  if(lines>nlinebuf){
    line[0]=0xfe;
    line[1]=0x04;
    len=(int)SendMessage(hwndRE, EM_GETLINE, 0, (LPARAM)line);
    if(len!=0){
      SendMessage(hwndRE, EM_SETSEL, 0, (LPARAM)len);
      SendMessage(hwndRE, EM_REPLACESEL, 0, (LPARAM)"");
    }
  }
  DWORD index=(DWORD)SendMessage(hwndRE, WM_GETTEXTLENGTH, 0, 0);
  SendMessage(hwndRE, EM_SETSEL, index, (LPARAM)index);

  //現在の文字フォーマットを取得
  CHARFORMAT2 cf;
  cf.cbSize = sizeof(cf);
  cf.dwMask = CFM_COLOR;
  SendMessage(hwndRE, EM_GETCHARFORMAT, (WPARAM)SCF_SELECTION, (LPARAM)&cf);
  //文字色を変更
  COLORREF cr;
  if(caution==true){
    cr = RGB(255, 0, 0);
  }else{
    cr = RGB(0, 0, 0);
  }
  cf.dwEffects &= ~CFE_AUTOCOLOR;
  cf.crTextColor = cr;
  //変更した文字フォーマットを適用
  SendMessage(hwndRE, EM_SETCHARFORMAT, (WPARAM)SCF_SELECTION, (LPARAM)&cf);
  
  strcpy((char *)line, MText);

  strcat((char *)line, "\r\n");
  SendMessage(hwndRE, EM_REPLACESEL, 0, (LPARAM)line);

  oldstart=oldstart-len;
  oldend=oldend-len;
  if(oldend<0){
    oldstart=-1;
    oldend=0;
  }else{
    if(oldstart<0) oldstart=0;
  }
  SendMessage(hwndRE, EM_SETSEL, (WPARAM)oldstart, (LPARAM)oldend);
  SendMessage(hwndRE, WM_VSCROLL, SB_BOTTOM, 0);
}
