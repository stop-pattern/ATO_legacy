#include <Windows.h>
#include <string>
#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"


//駅ジャンプ時再読み込み処理
void reload(void) {/*
	if (signal > 9 || signal < 36) {
		int sig = signal;	//一時記憶
		SetSignal(0);
		if (ATCstatus & ATC_status::ATC_ON) {
			ATCstatus &= ~ATC_status::ATC_brake;
		}
		if (ATCstatus & ATC_status::ATO_ON) {
			ATCstatus |= ATC_status::ATO_stopping;
			ATCstatus &= ~ATC_status::ATO_control;
			ATCstatus &= ~ATC_status::ATO_doing;
		}
		if (ATCstatus & ATC_status::TASC_ON) {
			ATCstatus |= ATC_status::TASC_stopping;
			ATCstatus &= ~ATC_status::TASC_control;
			ATCstatus &= ~ATC_status::TASC_doing;
		}
		SetSignal(sig);
	}
	else {
		if (ATCstatus & ATC_status::ATC_ON) {
			ATCstatus &= ~ATC_status::ATC_brake;
		}
		if (ATCstatus & ATC_status::ATO_ON) {
			ATCstatus |= ATC_status::ATO_stopping;
			ATCstatus &= ~ATC_status::ATO_control;
			ATCstatus &= ~ATC_status::ATO_doing;
		}
		if (ATCstatus & ATC_status::TASC_ON) {
			ATCstatus |= ATC_status::TASC_stopping;
			ATCstatus &= ~ATC_status::TASC_control;
			ATCstatus &= ~ATC_status::TASC_doing;
		}
	}*/
}

//ATO/TASCスイッチ操作
void SetStatus() {
	if (ATCstatus & ATC_status::ON) {
		if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
			ATCstatus &= ATC_status::ATC_ON;	//ATC情報以外を消去
			switch (MasCon_key) {
			case Key::TRTA:
				ATCstatus |= ATC_status::ATO_ON;
			case Key::TOB:
			case Key::TKK:
			case Key::SEB:
				ATCstatus |= ATC_status::TASC_ON;
			case Key::TOY:
			case Key::KeyOff:
			case Key::SOT:
			case Key::JNR:
			case Key::OER:
				break;
			default:
				break;
			}
		}
	}
	else {
		ATCstatus &= ATC_status::ATC_ON;
		ATCstatus &= ~ATC_status::ON;
	}
}

//マスコンキー操作
void setKey(int in) {
	if (Stat.V == 0 && manual.P == 0 && manual.B == specific.E && manual.R == 0 && key_S == false) {
		ATCstatus &= ATC_status::ON;
		MasCon_key += in;
		if (MasCon_key < 0) {
			MasCon_key = 0;
		}
		else if (MasCon_key > 8) {
			MasCon_key = 8;
		}
		if ((MasCon_key >= 1 && MasCon_key <= 4) || MasCon_key == 8) {
			ATCstatus |= ATC_status::ATC_ON;
		}
		else ATCstatus &= ~ATC_status::ATC_ON;	//ATC情報以外を消去
		SetStatus();
	}
}