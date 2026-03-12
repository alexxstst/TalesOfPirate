// main.cpp : Defines the entry point for the console application.
//

#include "gateserver.h"
_DBC_USING;

BYTE g_wpe = 0;
BYTE g_ddos = 0;
BYTE g_rsaaes = 0;
uShort g_wpeversion = NULL;
HANDLE hConsole = nullptr;

//#pragma init_seg( lib )
pi_LeakReporter pi_leakReporter("gatememleak.log");
CResourceBundleManage g_ResourceBundleManage("GateServer.loc"); //Add by lark.li 20080130

//#include <ExceptionUtil.h>

int main(int argc, char* argv[])
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	setvbuf(stdout, nullptr, _IONBF, 0);
	setvbuf(stderr, nullptr, _IONBF, 0);

	C_TITLE("GateServer.exe")
	C_PRINT("Loading GateServer.cfg...\n");

	SEHTranslator translator;

	T_B

	const char* file_cfg = "GateServer.cfg";
	IniFile inf(file_cfg);
	try
	{
		g_wpe = std::stoi(inf["ToClient"]["WpeProtection"]);
		std::string v = inf["ToClient"]["WpeVersion"];
		g_wpeversion = (uShort)strtoul(v.c_str(), nullptr, 16);
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
	// Add by lark.li 20080731 begin
	pi_Memory m;
	m.startMonitor(1);
	// End

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

	// Add by lark.li 20080731 begin
	m.stopMonitor();
	m.wait();
	// End

	T_FINAL
	return 0;
}
