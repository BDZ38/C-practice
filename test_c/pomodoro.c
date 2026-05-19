#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

/* ── Pomodoro durations (in minutes) ── */
#define WORK_MINUTES       5  /* TODO: change to 25 */
#define BREAK_MINUTES      1  /* TODO: change to  5 */
#define LONG_BREAK_MINUTES 15
#define SESSIONS_BEFORE_LONG 4

/* ── Timer IDs ── */
#define ID_TICK   1
#define ID_OVERLAY_AUTO  2

/* ── Colors ── */
#define CLR_BG_WORK   RGB(30, 30, 30)
#define CLR_BG_BREAK  RGB(20, 50, 20)
#define CLR_TEXT      RGB(240, 240, 240)
#define CLR_GOLD      RGB(255, 210, 50)
#define CLR_RED       RGB(255, 70, 70)

/* ── Button IDs ── */
#define BTN_START  201
#define BTN_PAUSE  202
#define BTN_RESET  203
#define BTN_SKIP   204

/* ── State machine ── */
typedef enum {
    ST_IDLE,
    ST_WORKING,
    ST_PAUSED,
    ST_BREAKING,
    ST_BREAK_PAUSED
} PomState;

/* ── Globals ── */
static PomState  g_st         = ST_IDLE;
static int       g_total_sec  = WORK_MINUTES * 60;
static int       g_remaining  = WORK_MINUTES * 60;
static int       g_sessions   = 0;   /* completed work sessions */
static HWND      g_hwnd       = NULL;
static HWND      g_hwndOver   = NULL;
static HFONT     g_fontTimer  = NULL;
static HFONT     g_fontBig    = NULL;
static HFONT     g_fontSub    = NULL;
static BOOL      g_overShown  = FALSE;

/* ── Forward decls ── */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK OverProc(HWND, UINT, WPARAM, LPARAM);
void  PaintMain(HDC hdc, RECT* rc);
void  PaintOver(HDC hdc, RECT* rc);
void  SetTo(int minutes);
void  StartTimer(void);
void  PauseTimer(void);
void  OnTick(void);
void  FlashTaskbar(void);
void  ShowOverlay(const wchar_t* subtitle);
void  HideOverlay(void);

/* ══════════════════════════════════════════════════════════════════════ */

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdLine, int nShow)
{
    (void)hPrev; (void)cmdLine; (void)nShow;

    /* Register main window class */
    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(CLR_BG_WORK);
    wc.lpszClassName = L"PomoMain";
    RegisterClass(&wc);

    /* Register overlay class */
    WNDCLASS ov = {0};
    ov.lpfnWndProc   = OverProc;
    ov.hInstance     = hInst;
    ov.hCursor       = LoadCursor(NULL, IDC_ARROW);
    ov.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    ov.lpszClassName = L"PomoOver";
    RegisterClass(&ov);

    /* Screen metrics */
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int ww = 320, wh = 220;
    int x  = sw - ww - 40, y = 60;

    /* Main window — always-on-top tool window */
    g_hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"PomoMain", L"Pomodoro",
        WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        x, y, ww, wh, NULL, NULL, hInst, NULL);

    /* Overlay — fullscreen layered, initially hidden */
    g_hwndOver = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"PomoOver", L"",
        WS_POPUP, 0, 0, sw, sh,
        NULL, NULL, hInst, NULL);
    SetLayeredWindowAttributes(g_hwndOver, 0, 210, LWA_ALPHA);

    /* Fonts */
    g_fontTimer = CreateFont(72, 0, 0, 0, FW_BOLD, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Consolas");

    g_fontBig = CreateFont(96, 0, 0, 0, FW_BOLD, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Microsoft YaHei");

    g_fontSub = CreateFont(36, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Microsoft YaHei");

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(g_fontTimer);
    DeleteObject(g_fontBig);
    DeleteObject(g_fontSub);
    return 0;
}

/* ══════════════════════════════════════════════════════════════════════ */
/*  Main window                                                          */
/* ══════════════════════════════════════════════════════════════════════ */

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        /* Create 4 buttons across the bottom */
        int by = 155;
        CreateWindow(L"BUTTON", L"Start", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
            18, by, 64, 28, hwnd, (HMENU)BTN_START, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"Pause", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
            90, by, 64, 28, hwnd, (HMENU)BTN_PAUSE, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"Reset", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
            162, by, 64, 28, hwnd, (HMENU)BTN_RESET, GetModuleHandle(NULL), NULL);
        CreateWindow(L"BUTTON", L"Skip", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
            234, by, 64, 28, hwnd, (HMENU)BTN_SKIP, GetModuleHandle(NULL), NULL);
        break;
    }

    case WM_CTLCOLORSTATIC:
        SetBkMode((HDC)wp, TRANSPARENT);
        SetTextColor((HDC)wp, CLR_TEXT);
        return (LRESULT)GetStockObject(NULL_BRUSH);

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc; GetClientRect(hwnd, &rc);
        rc.bottom = 148;
        PaintMain(hdc, &rc);
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wp))
        {
        case BTN_START:
            switch (g_st) {
            case ST_IDLE:
                g_sessions = 0;
                SetTo(WORK_MINUTES);
                g_st = ST_WORKING;
                StartTimer();
                break;
            case ST_PAUSED:
                g_st = ST_WORKING;
                StartTimer();
                break;
            case ST_BREAK_PAUSED:
                g_st = ST_BREAKING;
                StartTimer();
                break;
            case ST_BREAKING:
                /* Break was auto-started; no-op if already running */
                break;
            case ST_WORKING:
                break;
            }
            break;

        case BTN_PAUSE:
            PauseTimer();
            break;

        case BTN_RESET:
            KillTimer(hwnd, ID_TICK);
            g_st = ST_IDLE;
            g_sessions = 0;
            SetTo(WORK_MINUTES);
            HideOverlay();
            InvalidateRect(hwnd, NULL, TRUE);
            SetWindowText(hwnd, L"Pomodoro");
            break;

        case BTN_SKIP:
            KillTimer(hwnd, ID_TICK);
            if (g_st == ST_WORKING || g_st == ST_PAUSED) {
                /* Skip to break */
                ++g_sessions;
                int bm = (g_sessions % SESSIONS_BEFORE_LONG == 0)
                    ? LONG_BREAK_MINUTES : BREAK_MINUTES;
                SetTo(bm);
                g_st = ST_BREAKING;
                StartTimer();
            } else {
                /* Skip break → back to work */
                SetTo(WORK_MINUTES);
                g_st = ST_IDLE;
            }
            HideOverlay();
            InvalidateRect(hwnd, NULL, TRUE);
            SetWindowText(hwnd, L"Pomodoro");
            break;
        }
        break;

    case WM_TIMER:
        if (wp == ID_TICK)
            OnTick();
        else if (wp == ID_OVERLAY_AUTO) {
            KillTimer(hwnd, ID_OVERLAY_AUTO);
            HideOverlay();
        }
        break;

    case WM_DESTROY:
        KillTimer(hwnd, ID_TICK);
        KillTimer(hwnd, ID_OVERLAY_AUTO);
        if (g_hwndOver) DestroyWindow(g_hwndOver);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

/* ══════════════════════════════════════════════════════════════════════ */
/*  Overlay window                                                       */
/* ══════════════════════════════════════════════════════════════════════ */

LRESULT CALLBACK OverProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc; GetClientRect(hwnd, &rc);
        PaintOver(hdc, &rc);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        HideOverlay();
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

/* ══════════════════════════════════════════════════════════════════════ */
/*  Drawing                                                              */
/* ══════════════════════════════════════════════════════════════════════ */

void PaintMain(HDC hdc, RECT* rc)
{
    BOOL onBreak = (g_st == ST_BREAKING || g_st == ST_BREAK_PAUSED);
    BOOL paused  = (g_st == ST_PAUSED || g_st == ST_BREAK_PAUSED);

    /* Background fill */
    COLORREF bg = onBreak ? CLR_BG_BREAK : CLR_BG_WORK;
    SetDCBrushColor(hdc, bg);
    FillRect(hdc, rc, (HBRUSH)GetStockObject(DC_BRUSH));

    SetBkMode(hdc, TRANSPARENT);

    /* Mode label */
    HFONT hfSmall = CreateFont(22, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Microsoft YaHei");
    SelectObject(hdc, hfSmall);
    SetTextColor(hdc, CLR_GOLD);

    RECT lr = {0, 6, rc->right, 40};
    const wchar_t* label;
    if (paused)       label = L"||  PAUSED";
    else if (onBreak) label = L"[~]  BREAK";
    else              label = L"[>]  FOCUS";
    DrawText(hdc, label, -1, &lr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    /* Session dots */
    RECT dr = {0, 42, rc->right, 56};
    wchar_t dots[16] = {0};
    for (int i = 0; i < SESSIONS_BEFORE_LONG; i++)
        dots[i] = (i < (g_sessions % SESSIONS_BEFORE_LONG)) ? L'#' : L'_';
    SetTextColor(hdc, RGB(160, 160, 160));
    DrawText(hdc, dots, -1, &dr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DeleteObject(hfSmall);

    /* Big timer digits */
    SelectObject(hdc, g_fontTimer);
    SetTextColor(hdc, (g_remaining <= 10 && !paused) ? CLR_RED : CLR_TEXT);

    RECT tr = {0, 50, rc->right, 145};
    int m = g_remaining / 60, s = g_remaining % 60;
    wchar_t buf[16];
    swprintf(buf, 16, L"%02d:%02d", m, s);
    DrawText(hdc, buf, -1, &tr, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void PaintOver(HDC hdc, RECT* rc)
{
    HBRUSH bg = CreateSolidBrush(RGB(8, 8, 8));
    FillRect(hdc, rc, bg);
    DeleteObject(bg);

    SetBkMode(hdc, TRANSPARENT);

    /* Large headline — stored as window text */
    wchar_t headline[64];
    GetWindowText(g_hwndOver, headline, 64);

    SelectObject(hdc, g_fontBig);
    SetTextColor(hdc, CLR_RED);
    RECT r1 = *rc;
    r1.top += rc->bottom / 2 - 140;
    r1.bottom = r1.top + 120;
    DrawText(hdc, headline, -1, &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    /* Smaller hint */
    SelectObject(hdc, g_fontSub);
    SetTextColor(hdc, RGB(200, 200, 200));
    RECT r2 = *rc;
    r2.top = r1.bottom + 30;
    r2.bottom = r2.top + 50;
    DrawText(hdc, L"Click anywhere to dismiss", -1, &r2,
             DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

/* ══════════════════════════════════════════════════════════════════════ */
/*  Logic                                                                */
/* ══════════════════════════════════════════════════════════════════════ */

void SetTo(int minutes)
{
    g_total_sec = minutes * 60;
    g_remaining = g_total_sec;
}

void StartTimer(void)
{
    KillTimer(g_hwnd, ID_TICK);
    SetTimer(g_hwnd, ID_TICK, 1000, NULL);
    InvalidateRect(g_hwnd, NULL, TRUE);

    int m = g_remaining / 60, s = g_remaining % 60;
    wchar_t title[64];
    const wchar_t* mode = (g_st == ST_BREAKING) ? L"Break" : L"Work";
    swprintf(title, 64, L"%s - %02d:%02d", mode, m, s);
    SetWindowText(g_hwnd, title);
}

void PauseTimer(void)
{
    if (g_st == ST_WORKING) {
        g_st = ST_PAUSED;
        KillTimer(g_hwnd, ID_TICK);
        InvalidateRect(g_hwnd, NULL, TRUE);
        SetWindowText(g_hwnd, L"Paused");
    } else if (g_st == ST_BREAKING) {
        g_st = ST_BREAK_PAUSED;
        KillTimer(g_hwnd, ID_TICK);
        InvalidateRect(g_hwnd, NULL, TRUE);
        SetWindowText(g_hwnd, L"Paused");
    }
}

void OnTick(void)
{
    BOOL isWorking = (g_st == ST_WORKING);
    BOOL isBreaking = (g_st == ST_BREAKING);
    if (!isWorking && !isBreaking) return;

    --g_remaining;
    InvalidateRect(g_hwnd, NULL, TRUE);

    /* Update title bar countdown */
    int m = g_remaining / 60, s = g_remaining % 60;
    wchar_t title[64];
    const wchar_t* mode = isBreaking ? L"Break" : L"Work";
    swprintf(title, 64, L"%s - %02d:%02d", mode, m, s);
    SetWindowText(g_hwnd, title);

    if (g_remaining > 0) return;

    /* Timer expired */
    KillTimer(g_hwnd, ID_TICK);
    FlashTaskbar();

    if (isWorking) {
        /* Work → Break */
        ++g_sessions;
        int bm = (g_sessions % SESSIONS_BEFORE_LONG == 0)
            ? LONG_BREAK_MINUTES : BREAK_MINUTES;

        wchar_t sub[64];
        swprintf(sub, 64, L"Take a %d-minute break!", bm);
        ShowOverlay(sub);

        SetTo(bm);
        g_st = ST_BREAKING;
    } else {
        /* Break → Idle */
        wchar_t sub[64];
        int wm = WORK_MINUTES;
        swprintf(sub, 64, L"Break is over! Time to focus for %d min.", wm);
        ShowOverlay(sub);

        SetTo(WORK_MINUTES);
        g_st = ST_IDLE;
    }

    InvalidateRect(g_hwnd, NULL, TRUE);
}

void FlashTaskbar(void)
{
    FLASHWINFO fi = {0};
    fi.cbSize  = sizeof(fi);
    fi.hwnd    = g_hwnd;
    fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
    fi.uCount  = 0;
    FlashWindowEx(&fi);
}

void ShowOverlay(const wchar_t* subtitle)
{
    SetWindowText(g_hwndOver, subtitle);

    /* Reposition to cover current screen in case of multi-monitor */
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(g_hwndOver, HWND_TOPMOST, 0, 0, sw, sh, SWP_SHOWWINDOW);

    ShowWindow(g_hwndOver, SW_SHOW);
    InvalidateRect(g_hwndOver, NULL, TRUE);
    UpdateWindow(g_hwndOver);
    g_overShown = TRUE;

    /* Auto-dismiss after 10 seconds */
    SetTimer(g_hwnd, ID_OVERLAY_AUTO, 10000, NULL);
}

void HideOverlay(void)
{
    if (g_overShown) {
        ShowWindow(g_hwndOver, SW_HIDE);
        g_overShown = FALSE;
        KillTimer(g_hwnd, ID_OVERLAY_AUTO);
    }

    /* After dismissing the "break over" overlay, auto-start work if user wants */
    if (g_st == ST_IDLE) {
        /* Stay idle — user clicks Start when ready */
    }
}
