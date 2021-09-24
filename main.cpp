#include <iostream>
#include <windows.h>
#include <tchar.h>

using namespace std;
#define INFO_BUFFER_SIZE 3276
#define MAX_KEY_LENGTH 255

int main(){
  LARGE_INTEGER lpPerformanceCount_s;
  QueryPerformanceCounter(&lpPerformanceCount_s); //выдает текущее значение счетчика, который непрерывно и равномерно растет с момента включения компьютера
  // 1.1 // Определение и вывод версии операционной  системы
  OSVERSIONINFO osVers = { 0 };// Заполняем структуру нулевыми байтами (специальный синтаксис C89).
  osVers.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); // WinAPI требует указывать размер структуры, чтобы иметь возможностьрасширять структуру в новых версиях ОС.
  GetVersionEx(&osVers); // получить номер версии ОС
  printf("Windows v%d.%d (build %d)\n", osVers.dwMajorVersion, osVers.dwMinorVersion, osVers.dwBuildNumber);

  // 1.2 // Определение  системного  каталога и его вывод на экран
  TCHAR system_dir[INFO_BUFFER_SIZE];
  DWORD CountBuf = INFO_BUFFER_SIZE;
  GetSystemDirectory(system_dir, INFO_BUFFER_SIZE);
  printf("\nSystem directory: %s \n", system_dir);

  // 1.3 // Название компьютера и псевдоним текущего пользователя
  CountBuf = INFO_BUFFER_SIZE;
  GetComputerName(system_dir, &CountBuf);  //Определение названия компьютера
  printf("\nComputer name: %s\n", system_dir);
  GetUserName(system_dir, &CountBuf);//Определение имени пользователя
  printf("    User name: %s\n", system_dir);

  // 1.4 // Вывод для каждого тома служебное имя, первый путь, объём и колическтво свободного места
  DWORD CharCount = MAX_PATH;
  char buffer[MAX_PATH];
  CHAR firstN[MAX_PATH+1];
  ULARGE_INTEGER total, available;

  HANDLE search = FindFirstVolume(buffer, sizeof(buffer));
  do {
    GetVolumePathNamesForVolumeName(buffer, firstN, CharCount, &CharCount);
    printf("\nNext name of tom: %s",buffer);
    printf("\nFirst path in tom: %s", firstN);

    if (GetDiskFreeSpaceEx((LPCSTR)firstN, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, NULL)==0) {
        cout << "\nNo information.\n";
    }
    else {
        cout << "\nVolume of tom:  " << total.QuadPart <<"  bytes, " ;
        cout << "\nFree place:  " << available.QuadPart <<"  bytes. " ;
        //printf("\n Free place:  = %lld\n", available.QuadPart); аналогичный вывод используя
    }
  }while (FindNextVolume(search, buffer, sizeof(buffer))==1);\
  FindVolumeClose(search);

  // 1.5 // Список программ, запускаемых при старте системы
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
        0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS){
    printf("\nRegOpenKeyEx() Error!\n");
  }
  else printf("\nRegOpenKeyEx() Good Job!\n");

  while (1) { // бесконечный цикл, будет выполняться, пока не оборвется break-ом
    dwSize = sizeof(szName);
    retCode = RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL);
    if (retCode == ERROR_SUCCESS){
        RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)PerfData, &cbData);
        printf("\n%d) %s %s", index + 1, szName, PerfData);
        index++;
    }
    else break;
  }
  RegCloseKey(hKey);

  // 2 // Определение функциональности измерения производительности ЦП:
  LARGE_INTEGER lpPerformanceCount_f;
  LARGE_INTEGER lpFrequency;
  QueryPerformanceFrequency(&lpFrequency); //замер рабочей частоты 𝑓 ЦП
  QueryPerformanceCounter(&lpPerformanceCount_f); //подсчет количества тактов Δ𝑡 ЦП, которое занимает выполнение программой пункта 1)

  printf("\n\nOperating frequency of CP = %lld Hz\n", lpFrequency.QuadPart);
  double Ncyc = 1e6 * ((double)lpPerformanceCount_f.QuadPart - (double)lpPerformanceCount_s.QuadPart) / (double)lpFrequency.QuadPart;
  std::cout<<"Number of program execution cycles = "<<Ncyc<<" ns\n";
  return 0;
}


