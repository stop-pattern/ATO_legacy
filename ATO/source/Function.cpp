#include <Windows.h>
#include <string>
#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"

using namespace std;

extern string dllPath;
extern string iniPath;


void GetPath(HMODULE hModule) {
	using namespace std;
	LPSTR FilePath;//	 = _T("");
	char buf[100];
	FilePath = &buf[0];
	string drive, dir, fname, ext;
	GetModuleFileNameA(hModule, FilePath, MAX_PATH);
	dllPath = string(FilePath);
	//_splitpath(&fpath, &drive, &dir, &fname, &ext);//パス名を構成要素に分解します
	int drive_i = dllPath.find_first_of("\\");
	int path_i = dllPath.find_last_of("\\") + 1;
	int ext_i = dllPath.find_last_of(".");
	drive = dllPath.substr(0, drive_i);
	dir = dllPath.substr(drive_i, path_i - drive_i);//0文字目から７文字切り出す "C:\\aaa\\"
	fname = dllPath.substr(path_i, ext_i - path_i);// ７文字目から３文字切り出す　"bbb"
	ext = dllPath.substr(ext_i, dllPath.size() - ext_i); // 10文字目から４文字切り出す ".txt"
	iniPath = drive + dir + fname + ".ini";
}

void reload(void) {
	switch (ATCstatus) {
	case ATC_status::ATO_ON:
	case ATC_status::ATO_driving:
	case ATC_status::ATO_stopping:
	case ATC_status::ATO_waiting:
	case ATC_status::ATO_TASC_control:
	case ATC_status::ATO_TASC_brake:
		ATCstatus = ATC_status::ATO_waiting;
		ATOstatus = ATC_status::ATO_waiting;
		TASCstatus = ATC_status::TASC_waiting;
		break;
	case ATC_status::TASC_ON:
	case ATC_status::TASC_control:
	case ATC_status::TASC_brake:
	case ATC_status::TASC_waiting:
		ATCstatus = ATC_status::TASC_waiting;
		ATOstatus = ATC_status::OFF;
		TASCstatus = ATC_status::TASC_waiting;
		break;
	case ATC_status::OFF:
	default:
		break;
	}
}


void SetStatus(bool in) {
	if (in) {
		if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
			switch (MasCon_key) {
			case Key::TRTA:
				ATCstatus = ATC_status::ATO_ON;
				ATOstatus = ATC_status::ATO_ON;
				TASCstatus = ATC_status::ATO_ON;
				break;
			case Key::TOB:
			case Key::TKK:
			case Key::SEB:
				ATCstatus = ATC_status::TASC_ON;
				ATOstatus = ATC_status::OFF;
				TASCstatus = ATC_status::TASC_ON;
				break;
			case Key::KeyOff:
			case Key::SOT:
			case Key::JNR:
			case Key::OER:
			case Key::TOY:
				ATCstatus = ATC_status::ON;
				ATOstatus = ATC_status::ON;
				TASCstatus = ATC_status::ON;
				break;
			default:
				ATCstatus = ATC_status::OFF;
				ATOstatus = ATC_status::OFF;
				TASCstatus = ATC_status::OFF;
				break;
			}
		}
	}
	else {
		ATCstatus = ATC_status::OFF;
	}
}

void setKey(int in) {
	if (manual.P == 0 && manual.B == specific.E && manual.R == 0 && key_S == false) {
		MasCon_key += in;
		if (MasCon_key < 0) {
			MasCon_key = 0;
		}
		else if (MasCon_key > 8) {
			MasCon_key = 8;
		}
		ATCstatus == ATC_status::OFF ? SetStatus(false) : SetStatus(true);
	}
}