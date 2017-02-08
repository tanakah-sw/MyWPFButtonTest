// mainwin.h

#define MAINWIN_CLASS_NAME "mainwin"

class mainWin
{
public:
  mainWin(HINSTANCE hAppInstance);
  ~mainWin();
  static LRESULT CALLBACK dlgProcMain(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

  void writeInformation(bool caution);

protected:
  HINSTANCE m_hAppInstance;
  HWND m_hdlg;

  char MText[256];
  char homeFolderName[MAX_PATH], iniFileName[MAX_PATH];
};
