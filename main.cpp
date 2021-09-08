#include <iostream>
#include <windows.h>
#include <tchar.h>

using namespace std;
#define INFO_BUFFER_SIZE 3276
#define MAX_KEY_LENGTH 255

double PCFreq = 0.0;
__int64 CounterStart = 0;
//����� ������� ������� f ��
void StartCounter()
{
 LARGE_INTEGER frequency;
 if (!QueryPerformanceFrequency(&frequency))
  cout << "\nQueryPerformanceFrequency ERROR!\n";
 PCFreq = double(frequency.QuadPart);
 printf("\n\nProcessor frequency in 2.1:  %u Hz\n", frequency);
 QueryPerformanceCounter(&frequency);
 CounterStart = frequency.QuadPart;
}

//������� ���������� ������ ��
double GetCounter()
{
 LARGE_INTEGER frequency;
 QueryPerformanceCounter(&frequency);
 return double(frequency.QuadPart - CounterStart) / PCFreq;
}


int main()
{
    cout << "Hello wocddncmrld!" << endl;

    // ����������� � ����� ������ ������������  �������
    OSVERSIONINFO osVers = { 0 };// ��������� ��������� �������� ������� (����������� ��������� C89).
    osVers.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); // WinAPI ������� ��������� ������ ���������, ����� ����� �������������������� ��������� � ����� ������� ��.
    GetVersionEx(&osVers);
    printf("Windows v%d.%d (build %d)\n", osVers.dwMajorVersion, osVers.dwMinorVersion, osVers.dwBuildNumber);

    //�����������  ����������  �������� � ��� ����� �� �����
    TCHAR system_dir[INFO_BUFFER_SIZE];
    DWORD CountBuf = INFO_BUFFER_SIZE;
    GetSystemDirectory(system_dir, INFO_BUFFER_SIZE);
    printf("\nSystem directory: %s \n", system_dir);

    //�������� ���������� � ��������� �������� ������������
    CountBuf = INFO_BUFFER_SIZE;
    GetComputerName(system_dir, &CountBuf);  //����������� �������� ����������
    printf("\nComputer name: %s\n", system_dir);

    GetUserName(system_dir, &CountBuf);//����������� ����� ������������
    printf("    User name: %s\n", system_dir);

    //����� ��� ������� ���� ��������� ���, ������ ����, ����� � ����������� ���������� �����
     DWORD CharCount = MAX_PATH;
     char buffer[MAX_PATH];
     CHAR firstN[MAX_PATH+1];
     __int64 total, available;


     HANDLE search = FindFirstVolume(buffer, sizeof(buffer));
     //printf("\n First name   %s", buffer);
     do {
        GetVolumePathNamesForVolumeName(buffer, firstN, CharCount, &CharCount);
        printf("\nNext name of tom: %s",buffer);
        printf("\nFirst path in tom: %s", firstN);

        if (GetDiskFreeSpaceEx((LPCSTR)firstN, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, NULL)==0) {
            printf("\nNo information.\n");
            }
           else {
            printf("\nVolume of tom: %u bytes, Free place: %u bytes\n", total, available);

            }
        }while (FindNextVolume(search, buffer, sizeof(buffer))==1);
FindVolumeClose(search);


       //������ ��������, ����������� ��� ������ �������
      DWORD dwSize;
      TCHAR szName[MAX_KEY_LENGTH];
      HKEY hKey;
      DWORD index = 0;
      DWORD retCode;
      DWORD BufferSize = 8192;
      DWORD cbData;

      PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
      cbData = BufferSize;

    if (RegOpenKeyEx(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
    {
        printf("\nRegOpenKeyEx() Error!\n");
    }
    else printf("\nRegOpenKeyEx() Good Job!\n");

    while (1) { // ����������� ����, ����� �����������, ���� �� ��������� break-��
        dwSize = sizeof(szName);
        retCode = RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("\n%d) %s %s", index + 1, szName, PerfData);
            index++;
        }
        else break;
    }
    RegCloseKey(hKey);

    StartCounter();
    printf("Tacts in 2.2: %d mks\n\n", GetCounter()*1000000);
    return 0;
}


