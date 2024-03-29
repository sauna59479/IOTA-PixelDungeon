#include "stdafx.h"
#include "mainGame.h"

//전역변수를 선언한다면 요기요기요 
HINSTANCE _hInstance;		//어플 고유번호
HWND _hWnd;					//조작질
POINT _ptMouse;

LPTSTR _lpszClass = TEXT("Iota Team");


mainGame _mg;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void setWindowsSize(int x, int y, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG message;			//메세지
	WNDCLASS wndClass;		//윈도우 정보를 담을 구조체

	//Salt Bae 
	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//클래스 여분 메모리
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리 
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 백그라운드 
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//윈도우 아이콘
	wndClass.hInstance = hInstance;									//윈도우 인스턴스 번호
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//윈도우 프로시져
	wndClass.lpszClassName = _lpszClass;							//윈도우 클래스 이름
	wndClass.lpszMenuName = NULL;									//윈도우 메뉴 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 창 스타일

	//윈도우 정보 등록
	RegisterClass(&wndClass);

	//윈도우 창 생성정보
	_hWnd = CreateWindow(
		WINNAME,
		WINNAME,
		WS_OVERLAPPEDWINDOW,		//윈도우 창 스타일
		WINSTARTX,					//윈도우 시작 좌표 X
		WINSTARTY,					//윈도우 시작 좌표 Y
		WINSIZEX,					//윈도우 창 가로크기
		WINSIZEY,					//윈도우 창 세로크기
		NULL,						//부모 윈도우 사용여부
		(HMENU)NULL,				//메뉴 사용여부
		hInstance,					//인스턴스
		NULL);						//자식 윈도우 사용여부

	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//윈도우 창을 모니터에 띄워줌
	ShowWindow(_hWnd, cmdShow);

	if (FAILED(_mg.init()))
	{
		return 0;
	}

	//GetMessage : 메시지 정보가 들어오면 그때 반응
	//PeekMessage : 메시지 정보가 들어오든 말든 무조건 반응(상시 루프돌고있음)

	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);	//입력이 들어오는 순간 그 메시지 처리 담당
			DispatchMessage(&message);	//실제로 윈도우에 그 메시지 전달
		}
		else
		{
			TIMEMANAGER->update(60.0f);
			_mg.update();
			_mg.render();
		}
	}

	/* 그동안 고생했으니 무덤으로 보내주자
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);	//입력이 들어오는 순간 그 메시지 처리 담당
		DispatchMessage(&message);	//실제로 윈도우에 그 메시지 전달
	}
	*/
	_mg.release();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _mg.MainProc(hWnd, iMessage, wParam, lParam);
}

void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;
	
	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;
	winRect.bottom = height;

	AdjustWindowRect(&winRect, WINSTYLE, false);

	SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);

}
