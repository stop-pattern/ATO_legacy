#include <Windows.h>
#include <string>
#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"



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


void SetStatus(bool in) {
	if (in) {
		if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
			ATCstatus = ATC_status::OFF;
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
				ATCstatus |= ATC_status::ON;
				break;
			default:
				break;
			}
		}
	}
	else {
		ATCstatus = ATC_status::OFF;
	}
}

void setKey(int in) {
	if (Stat.V == 0 && manual.P == 0 && manual.B == specific.E && manual.R == 0 && key_S == false) {
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
		else ATCstatus = ATC_status::OFF;
		ATCstatus == ATC_status::OFF ? SetStatus(false) : SetStatus(true);
	}
}