#include "DrawShapes.h"

// ������ ���ν��� �Լ�
LRESULT WINAPI WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

// �ʱ�ȭ �Լ�
HRESULT InitD3D(HWND hWnd)
{
    // Direct3D ��ü ����
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!g_pD3D)
        return E_FAIL;

    // ����̽� �ʱ�ȭ ����
    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = 800;
    d3dpp.BackBufferHeight = 600;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    // Direct3D ����̽� ����
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    // ���� ���� ����
    int numVertices = 6;

    if (FAILED(g_pd3dDevice->CreateVertexBuffer(numVertices * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL))) 
    {
        return E_FAIL;
    }

    // ���� ������ ����
    CUSTOMVERTEX* pVertices;

    if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
        return E_FAIL;

    float radius = 1.0f; // �������� ������
    D3DXVECTOR3 center = { 0.0f, 0.0f, 0.0f }; // �������� �߽�
    DWORD color = D3DCOLOR_XRGB(255, 0, 0); // ������

    pVertices[0].position = center; // �߽���
    pVertices[0].color = color;

    for (int i = 1; i <= 6; ++i)
    {
        float theta = (2 * D3DX_PI * i) / 5 + D3DX_PI / 2; // ���� ��� (���� ����) �� ȸ�� ����
        pVertices[i].position = D3DXVECTOR3(center.x + radius * cosf(theta), center.y + radius * sinf(theta), center.z);
        pVertices[i].color = color;
    }

    g_pVB->Unlock();

    return S_OK;
}

// ���� �Լ�
VOID Cleanup()
{
    if (g_pVB != NULL)
        g_pVB->Release();

    if (g_pd3dDevice != NULL)
        g_pd3dDevice->Release();

    if (g_pD3D != NULL)
        g_pD3D->Release();
}

// ������ �Լ�
VOID Render()
{
    // ��� ����
    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        // ȭ�� �����
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

        // ���� ���� ����
        g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
        g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

        // �ﰢ�� �׸���
        g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 5); // ���� ������ ����

        // ��� ����
        g_pd3dDevice->EndScene();

        // ȭ�鿡 ǥ��
        g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
    }
}

// ���ø����̼� ������
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT nCmdShow)
{
    // ������ Ŭ���� ���
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WindowProc, 0, 0, hInstance, NULL, LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, "Draw Shapes", NULL };
    RegisterClassEx(&wc);

    // ������ ����
    HWND hWnd = CreateWindow("Draw Shapes", "Draw Pentagon", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Direct3D �ʱ�ȭ
    if (FAILED(InitD3D(hWnd)))
        return 0;

    // ���� ����
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg); // Ű���� �Է� �޽����� ���� �޽����� ��ȯ�Ѵ�.
            DispatchMessage(&msg); // �޽����� �ش� ������ ���ν����� �����Ͽ� ó���Ѵ�.
        }
        else
        {
            Render();
        }
    }

    Cleanup();
    return 0;
}