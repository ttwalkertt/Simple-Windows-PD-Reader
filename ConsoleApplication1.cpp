// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <fileapi.h>
#include <errhandlingapi.h>

#include <chrono>
#include <thread>

#define _devicename L"\\\\.\\PhysicalDrive2"

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
    switch (fdwCtrlType)
    {
        // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
        printf("Ctrl-C event\n\n");
        Beep(750, 300);
        return FALSE;

        // CTRL-CLOSE: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        Beep(600, 200);
        printf("Ctrl-Close event\n\n");
        return TRUE;

        // Pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
        Beep(900, 200);
        printf("Ctrl-Break event\n\n");
        return FALSE;

    case CTRL_LOGOFF_EVENT:
        Beep(1000, 200);
        printf("Ctrl-Logoff event\n\n");
        return FALSE;

    case CTRL_SHUTDOWN_EVENT:
        Beep(750, 500);
        printf("Ctrl-Shutdown event\n\n");
        return FALSE;

    default:
        return FALSE;
    }
}



int main()
{
    std::cout << "Hello World!\n";
    HANDLE hDevice; 
    HANDLE invalid_handle = (HANDLE)-1;
    int numberofsectors = 8;
    DWORD bytesread;
    char* buffer = (char*)malloc(512 * numberofsectors);
    bool stat;

    if (SetConsoleCtrlHandler(CtrlHandler, TRUE))
    {
        std::cout << "handler installed";
    }
    else
    {
        std::cout << "failed to install handler";
        return -1;
    }

    hDevice = CreateFile(_devicename,
        GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);

    std::cout << hDevice;

    if (hDevice == invalid_handle)
    {
        std::cout << "unable to open" << _devicename;
        return -1;
    }

    while (1)
    {
        auto start = std::chrono::high_resolution_clock::now(); 
        stat = ReadFile(hDevice, buffer, 512 * numberofsectors, &bytesread, NULL);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> float_ms = end - start;
        std::cout <<  GetLastError() << " " << stat << " " << bytesread << " " << float_ms.count() << " ms\n";
        if (float_ms.count() > 100)std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}
