#undef UNICODE
#include <windows.h>
#include "resource.h"
#include <iostream>
#include <stdio.h> 

using namespace std;
HANDLE hFile;
HANDLE handle = 0;
DWORD lpNumberOfBytesRead;//���-�� ���������� ������, ���� 0 �� ��� ����� �����


// hwnd - � ������� ���� ���� ���������� ������������� HWND. ���� ������������� ��������� �� ����. 

BOOL CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
// PID - ���������� ������������� �������� 
//BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
// ������ ���� ��������� ������ 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, MainWndProc);
	return 0;
}

BOOL RunNotepad(HWND hWnd) { // ������ �������
	char processName[50]; // ��� ������ "Notepad.exe"
	int processNameLength = sizeof(processName);
	if (!GetDlgItemText(hWnd, IDC_COMANDLINE, processName, processNameLength)) // ������ ��� �� �������� � ������ � ��������� �����
		return 0;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si)); // �������� ����� ������������� ��������
	si.cb = sizeof(si);

	if (!CreateProcess(
		NULL,
		processName, // ��������� ������
		NULL,
		NULL,
		FALSE,
		0,   // ������ ��������
		NULL,
		NULL,
		&si, // ���������� ������������� 
		&pi) // ���������� ��������
		)return 0;

	if (handle != 0) CloseHandle(handle);
	CloseHandle(pi.hThread);
	handle = pi.hProcess;
	return 1; // return true
}







BOOL CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {
	case WM_INITDIALOG:
		SetTimer(hWnd, NULL, 250, NULL);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DestroyWindow(hWnd);
			return TRUE;
		case IDC_START:
			if (handle != 0) return TRUE;
			if (!RunNotepad(hWnd))return TRUE;
			return TRUE;
		case IDC_TERMINATE:
			if (handle != 0)
				TerminateProcess(handle, 1);
			return TRUE;
		case IDC_WRITETEXT:
		{
			
			char cBufferText[100];
			WriteFile(handle, cBufferText, sizeof(cBufferText), 0, NULL);
			ReadFile(handle, cBufferText, sizeof(cBufferText), 0, NULL); SendMessage(hWnd, CB_ADDSTRING, 0, (LONG)cBufferText);
			return TRUE;
		}

		}


		return FALSE;

		handle = 0;
		return TRUE;
	case WM_DESTROY:
		if (handle != 0)CloseHandle(handle);
		PostQuitMessage(0);
		return TRUE;
	}
	return FALSE;

}
