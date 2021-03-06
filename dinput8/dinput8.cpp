#include "dinput8.h"
#define ClassName 0x00A034E8

//Global Variables
double bDesiredFramerate = 60;
float bDesiredTimescale = 0.8f;

HMODULE baseModule;

//Dll Main
bool WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		//Get module location and its ini file
		char path[MAX_PATH];
		HMODULE hm = NULL;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)"dinput8.dll", &hm);
		GetModuleFileNameA(hm, path, sizeof(path));
		*strrchr(path, '\\') = '\0';
		strcat_s(path, "\\dinput8.ini");

	
		//DesiredFramerate read as string and parse
		{
			char strValue[255];
			DWORD LenghtWhatever = GetPrivateProfileString("MAIN", "FOV", "60", strValue, 254, path);
			bDesiredFramerate = std::atof(strValue);
			if (bDesiredFramerate < 25 || bDesiredFramerate > 120)
				bDesiredFramerate = 62;
		}

		//DesiredTimescale read as string and parse
		{
			char strValue[255];
			DWORD LenghtWhatever = GetPrivateProfileString("MAIN", "Timescale", "0.8", strValue, 254, path);
			bDesiredTimescale = (float)std::atof(strValue);
			if (bDesiredTimescale < 0.5 || bDesiredTimescale > 1.5)
				bDesiredTimescale = 0.9f;
		}

		//Get dll from Windows directory
		GetSystemDirectory(path, MAX_PATH);
		strcat_s(path, "\\dinput8.dll");

		//Set pointers
		dinput8.dll = LoadLibraryA(path);
		dinput8.DirectInput8Create = (LPWDirectInput8Create)GetProcAddress(dinput8.dll, "DirectInput8Create");
		dinput8.DllCanUnloadNow = (LPWDllCanUnloadNow)GetProcAddress(dinput8.dll, "DllCanUnloadNow");
		dinput8.DllGetClassObject = (LPWDllGetClassObject)GetProcAddress(dinput8.dll, "DllGetClassObject");
		dinput8.DllRegisterServer = (LPWDllRegisterServer)GetProcAddress(dinput8.dll, "DllRegisterServer");
		dinput8.DllUnregisterServer = (LPWDllUnregisterServer)GetProcAddress(dinput8.dll, "DllUnregisterServer");

		//Get base module
		baseModule = GetModuleHandle(NULL);
		UnprotectModule(baseModule);

		*(double*)((DWORD)baseModule + 0x228AD0) = bDesiredFramerate / bDesiredTimescale;
		*(float*)((DWORD)baseModule + 0x228FC4) = (float)(1.0f / bDesiredFramerate);
		*(float*)((DWORD)baseModule + 0x25142C) = (float)(1.0f / bDesiredFramerate);


		break;
	}
	case DLL_PROCESS_DETACH:
	{
		FreeLibrary(hModule);
		break;
	}
	return true;
	}

	return TRUE;
}


HRESULT WINAPI DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter)
{
	HRESULT hr = dinput8.DirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);

	return hr;
}

HRESULT WINAPI DllCanUnloadNow()
{
	return dinput8.DllCanUnloadNow();
}

HRESULT WINAPI DllGetClassObject(REFCLSID riidlsid, REFIID riidltf, LPVOID whatever)
{
	return dinput8.DllGetClassObject(riidlsid, riidltf, whatever);
}

HRESULT WINAPI DllRegisterServer()
{
	return dinput8.DllRegisterServer();
}

HRESULT WINAPI DllUnregisterServer()
{
	return dinput8.DllUnregisterServer();
}
