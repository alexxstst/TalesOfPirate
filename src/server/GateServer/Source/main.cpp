// main.cpp : Defines the entry point for the console application.
//

#include "gateserver.h"
_DBC_USING;

BYTE g_wpe = 0;
BYTE g_ddos = 0;
BYTE g_rsaaes = 0;
uShort g_wpeversion = NULL;
HANDLE hConsole = NULL;

CLanguageRecord g_ResourceBundleManage{}; //Add by lark.li 20080130
CLanguageRecord& CLanguageRecordInstance() {
	if (g_ResourceBundleManage.GetRecordStringCount() == 0) {
		g_ResourceBundleManage.LoadFromTxtStringFile("server-localization.txt");
	}
	return g_ResourceBundleManage;
}

//#include <ExceptionUtil.h>

int main(int argc, char* argv[])
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	C_TITLE("GateServer.exe")
	C_PRINT("Loading GateServer.cfg...\n");
	std::cout << "Loaded string: " << CLanguageRecordInstance().GetRecordCount() << std::endl;

	T_B

	const char* file_cfg = "GateServer.cfg";
	IniFile inf(file_cfg);
	try
	{
		g_wpe = std::stoi(inf["ToClient"]["WpeProtection"]);
		std::string v = inf["ToClient"]["WpeVersion"];
		g_wpeversion = (uShort)strtoul(v.c_str(), NULL, 16);
		printf("Current WPE version is %s\n", v.c_str());
		g_ddos = std::stoi(inf["ToClient"]["DDoSProtection"]);
		g_rsaaes = std::stoi(inf["ToClient"]["RSAAES"]);

	}
	catch (...)
	{
		g_wpe = 0;
		g_ddos = 0;
		g_rsaaes = 0;
	}

	::SetLGDir("logfile/log");

	try {
		GateServerApp app;
		app.ServiceStart();
		g_gtsvr->RunLoop();
		app.ServiceStop();
	}
	catch (std::exception& e)
	{
		std::string filename;
		GetLGDir(filename);
		filename += "\\exception.txt";

		std::ofstream ofs(filename, std::ios::app);
		if (ofs.is_open())
		{
			SYSTEMTIME st;
			char tim[100] = { 0 };
			GetLocalTime(&st);
			sprintf(tim, "%02d-%02d %02d:%02d:%02d\r\n", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			ofs << tim;
			ofs << e.what() << std::endl;
		}
	}

	T_FINAL
	return 0;
}
