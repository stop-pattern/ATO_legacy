#include "ATC.h"

void c_ATC::Control(State S, int * panel, int * sound) {
	if (limit + 2 < S.V) {
		control.B;
		this->brake_cnt = S.T;
	}
	else {
		if (S.T - brake_cnt < BRAKE_HALF) {
			this->control.B = int(specific.E / 2);
		}
		else control.B = 0;
	}
}

void c_ATC::setSignal(int a) {
	this->limit = SpeedLimit[a];
}
