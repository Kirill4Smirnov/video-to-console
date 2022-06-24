#include "opencv2/opencv.hpp"
#include <iostream>
#include <Windows.h>

using namespace cv;

const int Xconst = 8; //size of a symbol in console in pixels(enter your numbers)
const int Yconst =  17;

const int nScreenWidth = 120;// size of the console window
const int nScreenHeight = 30;


const double PixelRatio = (double)Xconst / Yconst;
const double ScreenRatio = (double)nScreenWidth / nScreenHeight;

const char gradient[] = " .:!/r(l1Z4H9W8$@";


void SetWindow(int Width = 120, int Height = 30, int Color = 0x0F)
{
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;
    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(Handle, coord);
    SetConsoleWindowInfo(Handle, TRUE, &Rect);
    SetConsoleTextAttribute(Handle, Color); //цвет символов в консоли
}

void MakeFrame(Mat& frame, wchar_t* screen, const int &Xpixels, const int& Ypixels) {
    const int brigth_scale = sizeof(gradient);
    for (int i = 0; i < nScreenWidth; i++) {
        for (int j = 0; j < nScreenHeight; j++) {
            int count = 0;
            long int bright = 0;

            for (int x = i * Xpixels; x < (i + 1) * Xpixels; x++) {
                for (int y = j * Ypixels; y < (j + 1) * Ypixels; y++) {
                    count++;
                    bright += (frame.at<Vec3b>(y, x)[0] + frame.at<Vec3b>(y, x)[1] + frame.at<Vec3b>(y, x)[2]);
                }
            }
            bright = bright * brigth_scale / 765 / count;

            screen[i + j * nScreenWidth] = gradient[bright];
        }
    }

    screen[nScreenWidth * nScreenHeight] = '\0';  // Последний символ - окончание строки
}

int main()
{
    wchar_t screen[nScreenWidth * nScreenHeight + 1];

    SetWindow(nScreenWidth, nScreenHeight); //initialization of console screen
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;


    VideoCapture video(0); //select this to get video from your camera 
    //VideoCapture video("C:\\Users\\kenlo\\OneDrive\\Рабочий стол\\Фотографии\\2022\\20220112_173920.mp4"); //select this to get video from your computer

    if (!video.isOpened()) {
        std::cout << "Cannot open the image\n";
        return -1;
    }

    Mat img;
    video >> img;

    const int height = img.size[0];
    const int width = img.size[1];

    const int Xpixels = width / nScreenWidth; //number of pixels(of the image) in one console symbol
    const int Ypixels = height / nScreenHeight;

   
    while (true) {
        //pres esc to exit the program
        if (GetAsyncKeyState((unsigned short)VK_ESCAPE) & 0x8000) { 
            break;
        }  

        video >> img;

        if (img.empty()) {
            break;
        }

        MakeFrame(img, screen, Xpixels, Ypixels);

        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten); // Вывод картинки на экран
        Sleep(50);
    }

    return 0;
}
