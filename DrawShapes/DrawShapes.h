#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// 전역 변수
LPDIRECT3D9             g_pD3D = NULL; // D3D 디바이스를 생성할 D3D객체변수
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // 렌더링에 사용될 D3D디바이스
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // 정점을 보관할 정점버퍼

// 정점 구조체
struct CUSTOMVERTEX
{
    D3DXVECTOR3 position; // 정점의 위치
    DWORD color; // 정점의 색상
};

// 윈도우 프로시저 함수
LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 초기화 함수
HRESULT InitD3D(HWND hWnd);

// 정리 함수
VOID Cleanup();

// 렌더링 함수
VOID Render();

// 애플리케이션 진입점
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT);