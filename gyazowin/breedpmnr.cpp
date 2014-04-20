//

#include "stdafx.h"
#include "breedpmnr.h"

HINSTANCE hInst;							// МїНЁВћГCГУГ^Б[ГtГFГCГX
TCHAR *szTitle			= _T("Breed");		// Г^ГCГgГЛ ГoБ[ВћГeГLГXГg
TCHAR *szWindowClass	= _T("BREEDWINC");	// ГБГCГУ ГEГBГУГhГE ГNГЙГXЦЉ
TCHAR *szWindowClassL	= _T("BREEDWINL");	// ГМГCГДБ[ ГEГBГУГhГE ГNГЙГXЦЉ
HWND hLayerWnd;
bool Tutorial = TRUE;
NOTIFYICONDATA	niData;	// notify icon data
#define WM_MYMESSAGE (WM_USER + 1)
#define SWM_EXIT	WM_APP + 3//	close the window
int ofX, ofY;	// ЙжЦ ГIГtГZГbГg
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	LayerWndProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd;
int					GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

VOID				drawRubberband(HDC hdc, LPRECT newRect, BOOL erase);
VOID				execUrl(const char* str);
BOOL				SetClipboardText(LPCTSTR pszText);
BOOL				convertJPEG(LPCTSTR destFile, LPCTSTR srcFile);
BOOL				saveJPEG(LPCTSTR fileName, HBITMAP newBMP);
BOOL				uploadFile(HWND hwnd, LPCTSTR fileName);
std::string			getId();
BOOL				saveId(const WCHAR* str);
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HANDLE hMutex=::CreateMutex(NULL,TRUE,L"BreedPmnr");
	if(GetLastError()==ERROR_ALREADY_EXISTS){
		MessageBox(hWnd, _T("ѕрограмма уже запущена"), szTitle, 
		MB_OK | MB_ICONERROR);
		return 0;
	}
	TCHAR	szThisPath[MAX_PATH];
	DWORD   sLen;
	sLen = GetModuleFileName(NULL, szThisPath, MAX_PATH);
	for(unsigned int i = sLen; i >= 0; i--) {
		if(szThisPath[i] == _T('\\')) {
			szThisPath[i] = _T('\0');
			break;
		}
	}
	SetCurrentDirectory(szThisPath);
	MyRegisterClass(hInstance);
	InitInstance (hInstance, nCmdShow);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style         = 0;							// WM_PAINT ВрСЧВзВ»ВҐ
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GYAZOWIN));
	wc.hCursor       = LoadCursor(NULL, IDC_CROSS);	// + ВћГJБ[Г\ГЛ
	wc.hbrBackground = 0;							// ФwМiВаРЁТиВµВ»ВҐ
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	RegisterClass(&wc);
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = LayerWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GYAZOWIN));
	wc.hCursor       = LoadCursor(NULL, IDC_CROSS);	// + ВћГJБ[Г\ГЛ
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClassL;

	return RegisterClass(&wc);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	
	hInst = hInstance; // ГOГНБ[ГoГЛХѕРФВ…ГCГУГXГ^ГУГXПИЧЭВрКiФ[ВµВ№ВЈБB
		hWnd = CreateWindowEx(
		WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_TOPMOST
#if(_WIN32_WINNT >= 0x0500)
		| WS_EX_NOACTIVATE
#endif
		,
		szWindowClass, NULL, WS_POPUP,
		0, 0, 0, 0,
		NULL, NULL, hInstance, NULL);

	hLayerWnd = CreateWindowEx(
	 WS_EX_TOOLWINDOW
#if(_WIN32_WINNT >= 0x0500)
		| WS_EX_LAYERED | WS_EX_NOACTIVATE
#endif
		,
		szWindowClassL, NULL, WS_POPUP,
		100, 100, 300, 300,
		hWnd, NULL, hInstance, NULL);
	RegisterHotKey(hWnd,2,NULL,VK_SNAPSHOT);
	ZeroMemory(&niData,sizeof(NOTIFYICONDATA));
	niData.cbSize=NOTIFYICONDATA_V2_SIZE;
	niData.uID=1;
	niData.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE|NIF_INFO;
	niData.dwInfoFlags = NIIF_INFO;
	niData.hIcon = (HICON)LoadImage(hInstance,MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);
	niData.hWnd = hWnd;
	niData.uCallbackMessage=WM_APP;
	lstrcpyn(niData.szTip, szTitle, sizeof(niData.szTip)/sizeof(TCHAR));
	Shell_NotifyIcon(NIM_ADD,&niData);
	niData.uTimeout=1;
	lstrcpyn(niData.szInfo,L"»спользуйте PrintScreen дл€ выделени€ части экрана",sizeof(niData.szInfo)/sizeof(TCHAR));
	lstrcpyn(niData.szInfoTitle,L"Breed запущен",sizeof(niData.szInfoTitle)/sizeof(TCHAR));
	Shell_NotifyIcon( NIM_MODIFY, &niData );
    SetLayeredWindowAttributes(hLayerWnd, RGB(255, 0, 0), 100, LWA_COLORKEY|LWA_ALPHA);

	


	
	return TRUE;
}
void NotifyTray(LPCWSTR title,LPCWSTR info)
{
	lstrcpyn(niData.szInfo,info,sizeof(niData.szInfo)/sizeof(TCHAR));
	lstrcpyn(niData.szInfoTitle,title,sizeof(niData.szInfoTitle)/sizeof(TCHAR));
	Shell_NotifyIcon( NIM_MODIFY, &niData );
}
BOOL InitMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	
	int x, y, w, h;
	x = GetSystemMetrics(SM_XVIRTUALSCREEN);
	y = GetSystemMetrics(SM_YVIRTUALSCREEN);
	w = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	ofX = x; ofY = y;
	if (!hWnd) return FALSE;
	MoveWindow(hWnd, x, y, w, h, FALSE);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetTimer(hWnd, 1, 100, NULL);
	return TRUE;
}
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}
VOID drawRubberband(HDC hdc, LPRECT newRect, BOOL erase)
{
	
	static BOOL firstDraw = TRUE;	// 1 ЙсЦЏВЌСOВћГoГУГhВћПЅЛОВрНsВнВ»ВҐ
	static RECT lastRect  = {0};	// Н≈МгВ…Х`ЙжВµВљГoГУГh
	static RECT clipRect  = {0};	// Н≈МгВ…Х`ЙжВµВљГoГУГh
	
	if(firstDraw) {
		ShowWindow(hLayerWnd, SW_SHOW);
		UpdateWindow(hLayerWnd);

		//firstDraw = FALSE;
	}

	if (erase) {
		ShowWindow(hLayerWnd, SW_HIDE);
		
	}
	clipRect = *newRect;
	if ( clipRect.right  < clipRect.left ) {
		int tmp = clipRect.left;
		clipRect.left   = clipRect.right;
		clipRect.right  = tmp;
	}
	if ( clipRect.bottom < clipRect.top  ) {
		int tmp = clipRect.top;
		clipRect.top    = clipRect.bottom;
		clipRect.bottom = tmp;
	}
	MoveWindow(hLayerWnd,  clipRect.left, clipRect.top, 
			clipRect.right-  clipRect.left + 1, clipRect.bottom - clipRect.top + 1,true);

	
	return;
}
BOOL convertJPEG(LPCTSTR destFile, LPCTSTR srcFile)
{
	BOOL				res = FALSE;

	GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR			gdiplusToken;
	CLSID				clsidEncoder;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	int Quality = 90;
	Image *b = new Image(srcFile, 0);
	EncoderParameters encoderParams;
    encoderParams.Count = 1;
    encoderParams.Parameter[0].NumberOfValues = 1;
    encoderParams.Parameter[0].Guid  = EncoderQuality;
    encoderParams.Parameter[0].Type  = EncoderParameterValueTypeLong;
    encoderParams.Parameter[0].Value = &Quality;
	if (0 == b->GetLastStatus()) {
		if (GetEncoderClsid(L"image/jpeg", &clsidEncoder)) {
			if (0 == b->Save(destFile, &clsidEncoder, &encoderParams) ) {
					res = TRUE;//суд€ по описанию в документации - работать не должно
			}
		}
	}
	delete b;
	GdiplusShutdown(gdiplusToken);

	return res;
}
BOOL saveJPEG(LPCTSTR fileName, HBITMAP newBMP)
{
	BOOL				res = FALSE;
	int Quality = 90;
	GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR			gdiplusToken;
	CLSID				clsidEncoder;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Bitmap *b = new Bitmap(newBMP, NULL);
	EncoderParameters encoderParams;
    encoderParams.Count = 1;
    encoderParams.Parameter[0].NumberOfValues = 1;
    encoderParams.Parameter[0].Guid  = EncoderQuality;
    encoderParams.Parameter[0].Type  = EncoderParameterValueTypeLong;
    encoderParams.Parameter[0].Value = &Quality;	
	if (GetEncoderClsid(L"image/jpeg", &clsidEncoder)) {
		if (0 ==
			b->Save(fileName, &clsidEncoder, &encoderParams) ) {
				res = TRUE;
		}
	}
	delete b;
	GdiplusShutdown(gdiplusToken);

	return res;
}
LRESULT CALLBACK LayerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	RECT clipRect	= {0, 0, 500, 500};
	HBRUSH hBrush;
	HPEN hPen;
	HFONT hFont;


	switch (message)
	{

	case WM_ERASEBKGND:
		 GetClientRect(hWnd, &clipRect);
		
		hdc = GetDC(hWnd);
        hBrush = CreateSolidBrush(RGB(100,100,100));
        SelectObject(hdc, hBrush);
		hPen = CreatePen(PS_DASH,1,RGB(255,255,255));
		SelectObject(hdc, hPen);
		Rectangle(hdc,0,0,clipRect.right,clipRect.bottom);
		int fHeight;
		fHeight = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		hFont = CreateFont(fHeight,0,0,0,FW_REGULAR,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,//ГNГКГbГsГУГOРЄУx
			PROOF_QUALITY,			FIXED_PITCH | FF_MODERN,//ГsГbГ`В∆ГtГ@Г~ГКБ[
			L"Tahoma");
		SelectObject(hdc, hFont);
		int iWidth, iHeight;
		iWidth  = clipRect.right  - clipRect.left;
		iHeight = clipRect.bottom - clipRect.top;

		wchar_t sWidth[200], sHeight[200];
		swprintf_s(sWidth, L"%d", iWidth);
		swprintf_s(sHeight, L"%d", iHeight);

		int w,h,h2;
		w = -fHeight * 2.5 + 8;
		h = -fHeight * 2 + 8;
		h2 = h + fHeight;

		SetBkMode(hdc,TRANSPARENT);
		SetTextColor(hdc,RGB(0,0,0));
		TextOut(hdc, clipRect.right-w+1,clipRect.bottom-h+1,(LPCWSTR)sWidth,wcslen(sWidth));
		TextOut(hdc, clipRect.right-w+1,clipRect.bottom-h2+1,(LPCWSTR)sHeight,wcslen(sHeight));
		SetTextColor(hdc,RGB(255,255,255));
		TextOut(hdc, clipRect.right-w,clipRect.bottom-h,(LPCWSTR)sWidth,wcslen(sWidth));
		TextOut(hdc, clipRect.right-w,clipRect.bottom-h2,(LPCWSTR)sHeight,wcslen(sHeight));

		DeleteObject(hPen);
		DeleteObject(hBrush);
		DeleteObject(hFont);
		ReleaseDC(hWnd, hdc);

		return TRUE;

        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}
void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if(hMenu)
	{
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, _T("Exit"));
		SetForegroundWindow(hWnd);

		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
			pt.x, pt.y, 0, hWnd, NULL );
		DestroyMenu(hMenu);
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int wmId,wmEvent;
	static BOOL onClip		= FALSE;
	static BOOL firstDraw	= TRUE;
	static RECT clipRect	= {0, 0, 0, 0};
	
	switch (message)
	{
	case WM_APP:
		switch(lParam)
		{
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(hWnd);
		}
		break;
	case WM_HOTKEY:
		 if(Tutorial)NotifyTray(L"¬ыделение",L"«ажмите левую кнопку мыши и выделите нужную часть экрана\nƒл€ отмены выделени€ нажмите Escape или правую кнопку мыши.");
		 InitMainWindow(hInst,0);
		break;
	case WM_RBUTTONDOWN:
		ShowWindow(hWnd,SW_HIDE);
		ShowWindow(hLayerWnd,SW_HIDE);
		onClip = FALSE;
		ReleaseCapture();
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_TIMER:
		if (GetKeyState(VK_ESCAPE) & 0x8000){
			ShowWindow(hWnd,SW_HIDE);onClip = FALSE;
			ReleaseCapture();ShowWindow(hLayerWnd,SW_HIDE);
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_MOUSEMOVE:
		if (onClip) {
			clipRect.right  = LOWORD(lParam) + ofX;
			clipRect.bottom = HIWORD(lParam) + ofY;
			
			hdc = GetDC(NULL);
			drawRubberband(hdc, &clipRect, FALSE);

			ReleaseDC(NULL, hdc);
		}
		break;
	

	case WM_LBUTTONDOWN:
		{
			onClip = TRUE;
			clipRect.left = LOWORD(lParam) + ofX;
			clipRect.top  = HIWORD(lParam) + ofY;
			SetCapture(hWnd);
		}
		break;

	case WM_LBUTTONUP:
		{
			onClip = FALSE;
			ReleaseCapture();
			clipRect.right  = LOWORD(lParam) + ofX;
			clipRect.bottom = HIWORD(lParam) + ofY;
			HDC hdc = GetDC(NULL);
			drawRubberband(hdc, &clipRect, TRUE);
			if ( clipRect.right  < clipRect.left ) {
				int tmp = clipRect.left;
				clipRect.left   = clipRect.right;
				clipRect.right  = tmp;
			}
			if ( clipRect.bottom < clipRect.top  ) {
				int tmp = clipRect.top;
				clipRect.top    = clipRect.bottom;
				clipRect.bottom = tmp;
			}
			int iWidth, iHeight;
			iWidth  = clipRect.right  - clipRect.left + 1;
			iHeight = clipRect.bottom - clipRect.top  + 1;

			if(iWidth == 0 || iHeight == 0) {
				ReleaseDC(NULL, hdc);
				DestroyWindow(hWnd);
				break;
			}
			HBITMAP newBMP = CreateCompatibleBitmap(hdc, iWidth, iHeight);
			HDC	    newDC  = CreateCompatibleDC(hdc);
			SelectObject(newDC, newBMP);
			BitBlt(newDC, 0, 0, iWidth, iHeight, 
				hdc, clipRect.left, clipRect.top, SRCCOPY);
			ShowWindow(hWnd, SW_HIDE);
			TCHAR tmpDir[MAX_PATH], tmpFile[MAX_PATH];
			GetTempPath(MAX_PATH, tmpDir);
			GetTempFileName(tmpDir, _T("gya"), 0, tmpFile);
			
			if (saveJPEG(tmpFile, newBMP)) {
				if (!uploadFile(hWnd, tmpFile)) {
				}
			} else {
				MessageBox(hWnd, _T("Ќеудаетс€ сохранить временное изображение"), szTitle, 
					MB_OK | MB_ICONERROR);
			}
			DeleteFile(tmpFile);
			
			DeleteDC(newDC);
			DeleteObject(newBMP);
			
			ReleaseDC(NULL, hdc);
			ShowWindow(hWnd,SW_HIDE);
			
			//DestroyWindow(hWnd);
			/*PostQuitMessage(0);*/
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam); 

		switch (wmId)
		{
		case SWM_EXIT:
			Shell_NotifyIcon(NIM_DELETE,&niData);
			DestroyWindow(hWnd);
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
BOOL SetClipboardText(LPCTSTR pszText)
{
   BOOL ok = FALSE;
   if(OpenClipboard(NULL)) {
      // the text should be placed in "global" memory
      HGLOBAL hMem = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, 
         (lstrlen(pszText)+1)*sizeof(pszText[0]) );
      LPTSTR ptxt = (LPTSTR)GlobalLock(hMem);
      lstrcpy(ptxt, pszText);
      GlobalUnlock(hMem);
      // set data in clipboard; we are no longer responsible for hMem
      ok = (BOOL)SetClipboardData(CF_TEXT, hMem);

      CloseClipboard(); // relinquish it for other windows
   }
   return ok;
}
VOID execUrl(const char* str)
{
	size_t  slen;
	size_t  dcount;
	slen  = strlen(str) + 1; // NULL

	TCHAR *wcUrl = (TCHAR *)malloc(slen * sizeof(TCHAR));
	mbstowcs_s(&dcount, wcUrl, slen, str, slen);
	SHELLEXECUTEINFO lsw = {0};
	lsw.cbSize = sizeof(SHELLEXECUTEINFO);
	lsw.lpVerb = _T("open");
	lsw.lpFile = wcUrl;

	ShellExecuteEx(&lsw);

	free(wcUrl);
}
std::string getId()
{
	std::string idStr;
	DWORD VSNumber;
	DWORD MCLength;
	DWORD FileSF;

	if (GetVolumeInformation(L"C:\\",NULL, NULL,
		&VSNumber,&MCLength,&FileSF,NULL,NULL))
	{
		idStr=std::to_string(VSNumber);
	}
	
	return idStr;
}
BOOL uploadFile(HWND hwnd, LPCTSTR fileName)
{
	const TCHAR* UPLOAD_SERVER	= _T("breedpmnr.ru");
	const TCHAR* UPLOAD_PATH	= _T("/parser");

	const char*  sBoundary = "----BOUNDARYBOUNDARY----";		// boundary
	const char   sCrLf[]   = { 0xd, 0xa, 0x0 };					// ЙьНs(CR+LF)
	const TCHAR* szHeader  = 
		_T("Content-type: multipart/form-data; boundary=----BOUNDARYBOUNDARY----");

	std::ostringstream	buf;	// СЧРMГБГbГZБ[ГW
	std::string			idStr;	// ID
	idStr = getId();
	buf << "--";
	buf << sBoundary;
	buf << sCrLf;
	buf << "content-disposition: form-data; name=\"id\"";
	buf << sCrLf;
	buf << sCrLf;
	buf << idStr;
	buf << sCrLf;
	buf << "--";
	buf << sBoundary;
	buf << sCrLf;
	buf << "content-disposition: form-data; name=\"imagedata\"; filename=\"temp.jpeg\"";
	buf << sCrLf;
	buf << sCrLf;
	std::ifstream jpeg;
	jpeg.open(fileName, std::ios::binary);
	if (jpeg.fail()) {
		MessageBox(hwnd, _T("‘айл пропал!"), szTitle, MB_ICONERROR | MB_OK);
		jpeg.close();
		return FALSE;
	}
	buf << jpeg.rdbuf();		// read all & append to buffer
	jpeg.close();
	buf << sCrLf;
	buf << "--";
	buf << sBoundary;
	buf << "--";
	buf << sCrLf;
	std::string oMsg(buf.str());
	HINTERNET hSession    = InternetOpen(szTitle, 
		INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(NULL == hSession) {
		MessageBox(hwnd, _T("—истема не дает доступа к сокетам"),
			szTitle, MB_ICONERROR | MB_OK);
		return FALSE;
	}
	HINTERNET hConnection = InternetConnect(hSession, 
		UPLOAD_SERVER, INTERNET_DEFAULT_HTTP_PORT,
		NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if(NULL == hSession) {
		MessageBox(hwnd, _T("Ќевозможно установить соединение"),
			szTitle, MB_ICONERROR | MB_OK);
		return FALSE;
	}
	HINTERNET hRequest    = HttpOpenRequest(hConnection,
		_T("POST"), UPLOAD_PATH, NULL,
		NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD, NULL);
	if(NULL == hSession) {
		MessageBox(hwnd, _T("POST запрос некорректен"),
			szTitle, MB_ICONERROR | MB_OK);
		return FALSE;
	}
	const TCHAR* ua = _T("User-Agent: Breed/1.0\r\n");
	BOOL bResult = HttpAddRequestHeaders(
		hRequest, ua, _tcslen(ua), 
		HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	if (FALSE == bResult) {
		MessageBox(hwnd, _T("User-Agent не установлен"),
			szTitle, MB_ICONERROR | MB_OK);
		return FALSE;
	}
	if (HttpSendRequest(hRequest,
                    szHeader,
					lstrlen(szHeader),
                    (LPVOID)oMsg.c_str(),
					(DWORD) oMsg.length()))
	{
		
		DWORD resLen = 8;
		TCHAR resCode[8];
		HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE, resCode, &resLen, 0);
		if( _ttoi(resCode) != 200 ) {
			MessageBox(hwnd, _T("«агрузка не удалась.(—айт на реконструкции?)"),
				szTitle, MB_ICONERROR | MB_OK);
		} else {
			DWORD idLen = 100;
			TCHAR newid[100];
			
			memset(newid, 0, idLen*sizeof(TCHAR));	
			_tcscpy_s(newid, _T("Random"));

			HttpQueryInfo(hRequest, HTTP_QUERY_CUSTOM, newid, &idLen, 0);
			DWORD len;
			char  resbuf[1024];
			std::string result;
			while(InternetReadFile(hRequest, (LPVOID) resbuf, 1024, &len) 
				&& len != 0)
			{
				result.append(resbuf, len);
			}
			if(result.find("Failed")!=std::string::npos)
			{
				MessageBox(hwnd, _T("«агрузка не удалась\nѕерехожу на сайт."), szTitle, MB_ICONERROR | MB_OK);
				execUrl("http://breedpmnr.ru");
				return FALSE;
			}
			SetClipboardText((LPCTSTR)result.c_str());
			execUrl(result.c_str());
			if(Tutorial)NotifyTray(L"«агружено",L"—криншот успешно загружен на сервер и ссылка на него сохранена в буфере обмена(»спользуйте Ctrl+V)\nBreed продолжает работать в фоне. »спользуйте PrintScreen по необходимости");
			Tutorial=false;
			return TRUE;
		}
	} else {
		MessageBox(hwnd, _T("«агрузка не удалась"), szTitle, MB_ICONERROR | MB_OK);
	}

	return FALSE;

}