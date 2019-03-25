#include "../header/define.h"
#include "../header/Header.h"
#include "ATO.h"

#define ATO_LAG 500	//���O[ms]


void c_ATO::Control(State S, int * panel, int * sound) {
	//�]���h�~
	if (ATCstatus & static_cast<int>(ATC_Status::ATO_stopping)) {
		if (manual.B > control.B) {
			ATCstatus &= ~static_cast<int>(ATC_Status::ATO_stopping);
		}
		else {
			this->control.P = 0;
			this->control.B = specific.E / 2;
		}
	}

	//���Ԕ���
	if (this->Departure()) {
		ATCstatus |= static_cast<int>(ATC_Status::ATO_control);
	}

	//ATO����
	if (ATCstatus & static_cast<int>(ATC_Status::ATO_control)) {
		//�葬����
		if (this->Limit > S.V && LimitSpeed * 0.9 < S.V) {
			this->CSCcnt = S.T;
		}
		else {
			if (S.T - this->CSCcnt > ATO_CSC_TIME) {
				this->isCSC = true;
				this->CSCspeed = int(S.V);
			}
		}

		//��������
		if (LimitSpeed + 1 < S.V) {
			this->isCSC = false;
			if (this->control.B < specific.B) {
				if (rand() % 2) {
					this->control.B++;
				}
			}
		}
		//��������
		else if (this->Limit > S.V) {
			this->isCSC = false;
			if (this->control.P < specific.P) {
				if (rand() % 2) {
					this->control.P++;
				}
			}
		}
		//�čs����
		else {
			if (this->isCSC) {	//ATO�葬
				this->CSC();
			}
			this->control.P = 0;
			this->control.B = 0;
			//isCSC = true;
		}

		//���O����
		if (this->isFD) {
			if (this->Location < S.Z || this->speed < S.V) {
				isFD = false;
			}
			else {
				this->Distance = Stat.Z - this->Location;
				if (this->control.B < specific.B) {
					if (rand() % 2) this->control.B++;
				}
				else this->control.B = specific.B;
			}
		}
	}

	/*
	if (abs(accelaration) > 10) {
		handle.B = specific.E;	//
	}
	*/
}


void c_ATO::CSC() {
	if (accelaration > 0.5) {
		this->control.B += 1;
		this->control.P = 0;
	}
	else if (accelaration < -0.5) {
		this->control.B = 0;
		this->control.P += 1;
	}
	else {
		//�葬�ێ�
	}

	//�͈͊O����
	if (this->control.B < 0) {
		this->control.B = 0;
	}
	if (this->control.P < 0) {
		this->control.P = 0;
	}
}


bool c_ATO::Departure() {
	if (Key_A1 == true && (Key_A2 == true || Key_B1 == true)) {
		if (door == false && Stat.V == 0 && manual.B == 0 && manual.P == 0 && manual.R == 1 && LimitSpeed >= 25) {
			this->control.P = 0;
			this->control.B = 0;
			return true;
		}
	}
	return false;
}

void c_ATO::SignalChange() {
	this->Limit = this->Reference_Speed[this->Mode][signal];
	this->isFD = false;
}


void c_ATO::ChangeMode(int in) {
	if (S == true) {
		if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
			if (this->Mode > 0 && this->Mode < 3) {
				this->Mode += in;
				this->SignalChange();
			}
			else if (this->Mode >= 3) {
				this->Mode = 3;
			}
			else {
				this->Mode = 1;
			}
		}
	}
}

void c_ATO::setPattern(Beacon b) {
	brake = b;
}

void c_ATO::inEmergency(void) {
	ATCstatus &= ~static_cast<int>(ATC_Status::ATO_control);
	ATCstatus &= ~static_cast<int>(ATC_Status::ATO_doing);
	ATCstatus &= ~static_cast<int>(ATC_Status::ATO_stopping);
	this->control.P = 0;
	this->control.B = 0;
	this->isCSC = false;
}

void c_ATO::Forward_Deceleration(Beacon b) {
	if (b.Sig != signal && Reference_Speed[this->Mode][b.Sig] < LimitSpeed) {
		this->brake = b;
		this->speed = this->Reference_Speed[this->Mode][b.Sig];
		this->Distance = b.X;
		this->Location = Stat.Z + b.X;
		this->isFD = true;
	}
}
