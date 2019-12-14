// BitBlt_Test.cpp : アプリケーションのエントリ ポイントを定義します。

#include "framework.h"
#include "windows.h"
#include "BitBlt_Test.h"
#include "strconv.h"
#include <string>
using namespace std;

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;					// 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];		// タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];	// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ここにコードを挿入してください。
	// グローバル文字列を初期化する
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BITBLTTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーション初期化の実行:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BITBLTTEST));

	MSG msg;
	// メイン メッセージ ループ:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}

// ウィンドウ クラスを登録します。
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra	= 0;
	wcex.cbWndExtra	= 0;
	wcex.hInstance	= hInstance;
	wcex.hIcon		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BITBLTTEST));
	wcex.hCursor	= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName		= MAKEINTRESOURCEW(IDC_BITBLTTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassExW(&wcex);
}

// インスタンス ハンドルを保存して、メイン ウィンドウを作成・表示します
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

// WM_PAINT の部分だけを抜き出している。
void mypaint(HWND hWnd, LPARAM lParam) {

	RECT recDisp;
	GetClientRect(hWnd, &recDisp);	// クライアントを取得
	int cx = recDisp.right;			// クライアントのヨコ
	int cy = recDisp.bottom;		// クライアントのタテ

	string x1 = to_string(cx);
	string y1 = to_string(cy);
	string ss = "x:" + x1 + " y:" + y1;
	wstring wss = utf8_to_wide(ss) + L" です";

	// ここから実際の描写
	HDC hdc = GetDC(hWnd);
	HDC mem = CreateCompatibleDC(hdc);					//HDCを取得
	HBITMAP bmp = CreateCompatibleBitmap(hdc, cx, cy);	//HBITMAPを取得 ★クライアントと同じ大きさ
	HBITMAP obmp = (HBITMAP) SelectObject(mem, bmp);		//HBITMAPを関連付ける
	HBRUSH br = CreateSolidBrush(RGB(0, 0, 255));		//HBRUSHを取得
	HBRUSH obr = (HBRUSH) SelectObject(mem, br);		//HBRUSHをを関連付ける

	// 裏画面に描写する
	FillRect(mem, &recDisp, br);
	TextOut(mem, 10, 10, wss.c_str(), (int)wss.length());

	// 表画面にコピー
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	BitBlt(hdc, 0, 0, cx, cy, mem, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);

	// あれこれを削除する。
	SelectObject(mem, obmp);
	SelectObject(mem, obr);
	DeleteObject(bmp);
	DeleteObject(br);
	DeleteDC(mem);
}

// メイン ウィンドウのメッセージを処理します。
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId) {
			case IDC_BITBLTTEST:
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

	case WM_PAINT:
		mypaint(hWnd, lParam);
		break;

	case WM_MOUSEMOVE:
		InvalidateRect(hWnd, NULL, FALSE);
		// ３つ目がTRUEだと、BeginPaint() で背景を消去するのでチラチラする
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
