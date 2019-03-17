#include "../header/define.h"
#include "../header/Header.h"
#include "ATC.h"

void c_ATC::Control(State S, int * panel, int * sound) {
	//ATC現示・ベル
	if (changeSignal == true && signal > 9 && signal < 36) {
		for (size_t i = 101; i < 131; i++) {
			panel[i] = false;
		}
		panel[LimitSpeed / 5 + 102] = true;
		sound[ATC_Sound::ATC_bell] = SoundInfo::PlayOnce;
	}
	else {
		sound[ATC_Sound::ATC_bell] = SoundInfo::PlayContinue;
	}


	//前方予告
	if (this->isNotice && (S.T - this->notice_time) / FORWARDNOTICE % 2) {
		panel[ATC_Panel::notice] = true;
		panel[this->notice_panel] = true;
	}
	else {
		panel[ATC_Panel::notice] = false;
		panel[this->notice_panel] = false;
	}


	//速度照査
	if (!(ATCstatus & ATC_status::ATC_brake)) {
		this->brake_cnt = S.T;
	}

	if (Limit + 2 < S.V) {
		ATCstatus |= ATC_status::ATC_brake;
		this->control.B = int(specific.E / 2);
		if (Limit + 5 < S.V) {
			if (S.T - brake_cnt > BRAKE_HALF) {
				control.B = specific.B;
			}
		}
	}
	else {
		control.B = 0;
		ATCstatus &= ~ATC_status::ATC_brake;
	}


	changeSignal = false;
}

void c_ATC::setSignal() {
	this->isNotice = false;
	this->Limit = SpeedLimit[signal];
	this->changeSignal = true;
}

void c_ATC::notice(int sig, int param) {
	if (sig != signal || param != 0) {
		this->isNotice = true;
		this->notice_panel = sig - 10 + 102;
		this->notice_time = Stat.T;
	}
}

