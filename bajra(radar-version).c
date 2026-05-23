#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define WIDTH 1200
#define HEIGHT 800
#define RADAR_RADIUS 400
#define MAX_TARGETS 80
#define MAX_STATUS_LINES 12


typedef struct {
    float angle;         
    float distance;      
    int type;            
    int active;          
    float speed;         
    int detected;        
} SmoothTarget;

SmoothTarget targets[MAX_TARGETS];
float sweepAngle = 0;

int destroyed = 0;
int penetrated = 0;
int wave_index = 0;
int t = 9999; 

char statusLogs[MAX_STATUS_LINES][100] = {
    "SYSTEM INIT... SUCCESS",
    "CONTINUOUS AIR DEFENSE ACTIVATED"
};
int logCount = 2;

void AddLog(const char* format, ...) {
    va_list args;
    if (logCount >= MAX_STATUS_LINES) {
        memmove(statusLogs[0], statusLogs[1], sizeof(statusLogs[0]) * (MAX_STATUS_LINES - 1));
        logCount = MAX_STATUS_LINES - 1;
    }
    va_start(args, format);
    vsprintf(statusLogs[logCount], format, args);
    va_end(args);
    logCount++;
}


void SpawnContinuousTargets(int count) {
    int spawned = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < MAX_TARGETS; j++) {
            if (!targets[j].active) {
                targets[j].active = 1;
            
                targets[j].angle = (rand() % 360) * 3.14159f / 180.0f;
            
                targets[j].distance = (float)(RADAR_RADIUS - 10 - (rand() % 40));
                targets[j].detected = 0;
                int roll = rand() % 19 + 1;
                if (roll < 3) {
                    targets[j].type = 1;   
                    targets[j].speed = 1.4f;    
                } else if (roll >= 3 && roll <= 10) {
                    targets[j].type = 2;      
                    targets[j].speed = 0.8f;   
                } else if (roll > 10) {
                    targets[j].type = 3;       
                    targets[j].speed = 0.5f;  
                }
                
                spawned++;
                break;
            }
        }
    }
}

void PlayAirstrikeAlert() {
    PlaySound(TEXT(".\\siren.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
}


void UpdatePhysicsAndLogic() {
    if (rand() % 180 == 1) { 
        SpawnContinuousTargets(rand() % 3 + 1);
    }


    for (int i = 0; i < MAX_TARGETS; i++) {
        if (!targets[i].active) continue;
        targets[i].distance -= targets[i].speed;

        if (targets[i].distance < 30.0f) {
            penetrated++;
            AddLog("TERMINAL FAILURE: Perimeter Breach");
            targets[i].active = 0;
            continue;
        }

        float deg = targets[i].angle * 180.0f / 3.14159f;
        if (deg < 0) deg += 360.0f;
        float sweepDeg = sweepAngle * 180.0f / 3.14159f;
        float diff = fabs(deg - sweepDeg);

        if (diff < 6.0f) {
            if (!targets[i].detected) { 
                targets[i].detected = 1; 
            } 
        }

    
        if (targets[i].detected && (rand() % 400 < 3)) {
            int s = rand() % 2;
            int u = rand() % 2;

            if (s == 1 || u == 1) {
                destroyed++;
                if (targets[i].type == 1) AddLog("SUCCESS: Hypersonic Destroyed!");
                else if (targets[i].type == 2) AddLog("SUCCESS: Cruise Missile Destroyed!");
                else AddLog("SUCCESS: Ballistic Destroyed!");
                targets[i].active = 0;
            } else {
                
                if (targets[i].type == 1 || targets[i].type == 2) {
                    AddLog("FAIL: Intercept Missed!");
                    targets[i].type = 4;        
                    targets[i].speed *= 1.3f;   
                    targets[i].angle += 0.15f;  
                } else if (targets[i].type == 3) {
                    AddLog("FAIL:Intercept Missed!");
                    int retry_s = rand() % 2;
                    if (retry_s == 1) {
                        destroyed++;
                        AddLog("RETRY: Ballistic Target HIT!");
                    } else {
                        penetrated++;
                        AddLog("TERMINAL FAILURE: Target Penetrated");
                    }
                    targets[i].active = 0;
                }
            }
        }
    } 
}


void drawRadar(HDC hdc) {
    int cx = 500;
    int cy = HEIGHT / 2;

    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = { 0, 0, WIDTH, HEIGHT };
    FillRect(hdc, &rect, blackBrush);
    DeleteObject(blackBrush);

    HPEN greenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
    HPEN darkGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 100, 0));
    SetBkMode(hdc, TRANSPARENT);

    for (int r = 100; r <= RADAR_RADIUS; r += 100) {
        SelectObject(hdc, (r == RADAR_RADIUS) ? greenPen : darkGreenPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);
        SelectObject(hdc, oldBrush);
    }
    SelectObject(hdc, darkGreenPen);
    MoveToEx(hdc, cx - RADAR_RADIUS, cy, NULL); LineTo(hdc, cx + RADAR_RADIUS, cy);
    MoveToEx(hdc, cx, cy - RADAR_RADIUS, NULL); LineTo(hdc, cx, cy + RADAR_RADIUS);

    
    for (int i = 1; i < 18; i++) {
        float glow = sweepAngle - i * 0.02f;
        int gx = cx + (int)(RADAR_RADIUS * cos(glow));
        int gy = cy - (int)(RADAR_RADIUS * sin(glow));
        HPEN glowPen = CreatePen(PS_SOLID, 1, RGB(0, 160 - i * 8, 0));
        SelectObject(hdc, glowPen);
        MoveToEx(hdc, cx, cy, NULL); LineTo(hdc, gx, gy);
        DeleteObject(glowPen);
    }
    SelectObject(hdc, greenPen);
    int sx = cx + (int)(RADAR_RADIUS * cos(sweepAngle));
    int sy = cy - (int)(RADAR_RADIUS * sin(sweepAngle));
    MoveToEx(hdc, cx, cy, NULL); LineTo(hdc, sx, sy);

   
    HBRUSH redBlip = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH orangeBlip = CreateSolidBrush(RGB(255, 130, 0));
    HBRUSH yellowBlip = CreateSolidBrush(RGB(255, 235, 0));
    HBRUSH magentaBlip = CreateSolidBrush(RGB(255, 0, 255));
    HPEN alertRingPen = CreatePen(PS_SOLID, 1, RGB(220, 0, 0));

    for (int i = 0; i < MAX_TARGETS; i++) {
        if (targets[i].active && targets[i].detected) {
        
            int tx = cx + (int)(targets[i].distance * cos(targets[i].angle));
            int ty = cy - (int)(targets[i].distance * sin(targets[i].angle));

            SelectObject(hdc, redBlip);

            SelectObject(hdc, GetStockObject(BLACK_PEN));
            Ellipse(hdc, tx - 7, ty - 7, tx + 7, ty + 7);

            SelectObject(hdc, alertRingPen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Ellipse(hdc, tx - 13, ty - 13, tx + 13, ty + 13);

            char label[30];
            sprintf(label, "[R:%.0f, D:%s]", targets[i].distance, 
                targets[i].type == 1 ? "HYP" : (targets[i].type == 2 ? "CRU" : (targets[i].type == 3 ? "BAL" : "EVA")));
            SetTextColor(hdc, RGB(255, 90, 90));
            TextOut(hdc, tx + 14, ty - 6, label, lstrlen(label));
        }
    }
    DeleteObject(redBlip); DeleteObject(orangeBlip); 
    DeleteObject(yellowBlip); DeleteObject(magentaBlip);
    DeleteObject(alertRingPen);

    HBRUSH centerBrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, centerBrush);
    Ellipse(hdc, cx - 6, cy - 6, cx + 6, cy + 6);
    DeleteObject(centerBrush);

    
    SelectObject(hdc, greenPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, 900, 50, 1190, 720); 

    SetTextColor(hdc, RGB(0, 255, 0));
    TextOut(hdc, 930, 75, "REAL-TIME TACTICAL RADAR", 25);

    char buffer[100];
    sprintf(buffer, "ACTIVE RADAR MONITOR  : LIVE MODE");
    TextOut(hdc, 920, 110, buffer, lstrlen(buffer));

    sprintf(buffer, "THREATS NEUTRALIZED   : %d", destroyed);
    TextOut(hdc, 920, 140, buffer, lstrlen(buffer));

    sprintf(buffer, "PERIMETER BREACHES    : %d", penetrated);
    SetTextColor(hdc, (penetrated > 0) ? RGB(255, 50, 50) : RGB(0, 255, 0));
    TextOut(hdc, 920, 170, buffer, lstrlen(buffer));

    SetTextColor(hdc, RGB(0, 255, 255));
    TextOut(hdc, 920, 220, "--- LIVE WEAPON SYSTEMS FEED ---", 32);

    for (int idx = 0; idx < logCount; idx++) {
        if (strstr(statusLogs[idx], "TERMINAL") || strstr(statusLogs[idx], "ACTIVATED")||strstr(statusLogs[idx], "FAIL")) {
            SetTextColor(hdc, RGB(255, 70, 70));
        } else {
            SetTextColor(hdc, RGB(0, 230, 0));
        }
        TextOut(hdc, 905, 260 + (idx * 35), statusLogs[idx], lstrlen(statusLogs[idx])); 
    }

    DeleteObject(greenPen);
    DeleteObject(darkGreenPen);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY: PostQuitMessage(0); return 0;
        case WM_ERASEBKGND: return 1;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
            HANDLE hOld = SelectObject(hdcMem, hbmMem);

            drawRadar(hdcMem);

            BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hOld);
            DeleteObject(hbmMem);
            DeleteObject(hdcMem);
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand((unsigned int)time(NULL));

    
    SpawnContinuousTargets(6);

    PlayAirstrikeAlert();
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SmoothRadarWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, "SmoothRadarWindow", "NATIVE AIR DEFENSE SYSTEM - REALTIME FLUID SIMULATOR",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        50, 50, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (1) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return 0;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        sweepAngle += 0.025f;
        if (sweepAngle > 2 * 3.14159f) sweepAngle = 0;

        UpdatePhysicsAndLogic();

        InvalidateRect(hwnd, NULL, FALSE);
        Sleep(16); 
    }
    return 0;
}