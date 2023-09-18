// Spinning Donut.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <cmath>
#include <windows.h>
#include <stdlib.h>
using namespace std;

void usleep(__int64 usec)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

int main()
{
    /*
    origin: https://www.a1k0n.net/2011/07/20/donut-math.html
    futher optimization: https://www.a1k0n.net/2021/01/13/optimizing-donut.html (havn't implement)
    */
    int width = 40, height = 40;
    int R1 = 20, R2 = 40;
    int area = width * height;
    int K1 = 25, K2 = 100;
    int radiusP = (R1 + R2) * K1 / K2;
    int offsetx = width / 2 - radiusP, offsety = height / 2 - radiusP;
    char background = ' ';
    string spectrum = ".,-~:;=!*#$@";

    char* canvas = new char[area];
    for (int i = 0; i < area; i++) { canvas[i] = background; }
    float* Z = new float[area] {}; // z-buffer

    //float theta, phi;
    float A = 0, B = 0;
    for (;;) {
        float   cosA = cos(A), sinA = sin(A),
                cosB = cos(B), sinB = sin(B);

        for (float phi = 0; phi < 6.28; phi += 0.02) {
            float cosPhi = cos(phi), sinPhi = sin(phi);

            for (float theta = 0; theta < 6.28; theta += 0.02) {
                float sinTheta = sin(theta), cosTheta = cos(theta);
                
                float   U = R2 + R1 * cosTheta,
                        V = R1 * sinTheta;

                float   x = U * (cosB * cosPhi + sinA * sinB * sinPhi) - V * cosA * sinB,
                        y = U * (cosPhi * sinB - sinA * cosB * sinPhi) + V * cosA * cosB,
                        z = U * cosA * sinPhi + V * sinA + K2;
                
                double depth = 1 / z;
                
                int xp = radiusP + depth * K1 * x;
                int yp = radiusP - depth * K1 * y;

                // reflect = N dot (0, 0, -1), N = (cosθ, sinθ, 0) * rotation matrices
                double reflect = cosTheta * (cosPhi * sinB - sinPhi * cosA) - sinA * sinTheta
                                + cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);
                
                if (reflect >= 0){
                    int index = (xp + offsetx) +(yp + offsety) * width;
                    if (depth > Z[index]) {
                        Z[index] = depth;
                        int intensity_idx = 8 * reflect; // |reflect| <= sqrt(2), sqrt(2)*8 = 11.31
                        canvas[index] = spectrum[intensity_idx];
                    }
                }
            }
        }

        std::cout << "\x1b[H";
        int index = 0;
        for (int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++){
                index = j  + i * width;
                cout << canvas[index];
                //index += 1;
            }
            cout << endl;
        }

        usleep(30000);
        for (int i = 0; i < area; i++) {
            canvas[i] = background;
            Z[i] = 0;
        }
        A += 0.04;
        B += 0.02;
    }

    delete[] canvas;
    delete[] Z;

    return 0;
  /*  
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    char b[1760];
    printf("\x1b[2J");
    for(;;) {
        memset(b,32,1760);
        memset(z,0,7040);
        for(j=0; j < 6.28; j += 0.07) {
            for(i=0; i < 6.28; i += 0.02) {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y= 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if(22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for(k = 0; k < 1761; k++) {
            putchar(k % 80 ? b[k] : 10);
            A += 0.00004;
            B += 0.00002;
        }
        usleep(30000);
    }   
    */
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
