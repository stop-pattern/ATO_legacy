/*BVE ATSプラグインテンプレート
 *Author : Tetsu Otter
 *SDK : ver10.0.17763.0
 *DLLは、プロジェクト名で出力されます。任意のプロジェクト名を付与してください。
 *プロジェクト名変更時は、dllmain.defの "LIBRARY" に設定された "PITemp" という文字列を新たなプロジェクト名に変更してください。
*/

#include <string.h>
#include <math.h>
#include "../header/Header.h"
#include "../header/define.h"

#include "../ATO/ATO.h"
#include "../TASC/TASC.h"
#include "../ATC/ATC.h"

#include <fstream>


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	GetPath(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

DE void SC Load() {
}
DE void SC Dispose() {
}
DE int SC GetPluginVersion() {
	return PI_VERSION;
}
DE void SC SetVehicleSpec(Spec s) {
	specific.A = s.A;
	specific.J = s.J;
	specific.B = s.B;
	specific.E = s.B + 1;
	specific.C = s.C;
	specific.P = s.P;
}
DE void SC Initialize(int b) {
	b = InitialPosition::Service;
	reload();
}
DE Hand SC Elapse(State S, int * panel, int * sound) {
	accelaration = (S.V - Stat.V) / (S.T - Stat.T) * 1000;
	//MasCon_key = panel[92];
	//ATC_SW = panel[72];

	handle.P = manual.P;
	handle.B = manual.B;
	handle.R = manual.R;


	if (MasCon_key != Key::KeyOff) {
		if (S.T - lag_cnt >= LAG) {
			lag_cnt = S.T;
		}
	}

	//ATO
	if (ATCstatus & ATC_status::ATO_ON) {
		ATO.Control(S, panel, sound);	//制御関数

		//ATO動作
		if (ATCstatus & ATC_status::ATO_control) {
			if (manual.B == 0 && manual.P == 0) {
				handle.P = ATO.control.P;
				panel[ATC_Panel::ATO_P] = ATO.control.P + 1;
				panel[ATC_Panel::ATO_controling] = true;
			}
			else {
				ATO.control.P = 0;
				panel[ATC_Panel::ATO_P] = 0;
			}
			if (manual.P == 0 && manual.B < ATO.control.B) {
				handle.B = ATO.control.B;
				panel[ATC_Panel::ATO_B] = ATO.control.B + 1;
			}
			else {
				panel[ATC_Panel::ATO_B] = 0;
			}
		}
	}
	else {
		panel[ATC_Panel::Brake_notches] = manual.B;
		panel[ATC_Panel::ATO_P] = 0;
		panel[ATC_Panel::ATO_B] = 0;
		panel[ATC_Panel::ATO_power] = false;
		panel[ATC_Panel::ATO_controling] = false;
	}
	panel[ATC_Panel::ATO_debug] = int(ATO.Limit);


	//TASC
	if (ATCstatus & ATC_status::TASC_ON) {
		TASC.Control(S, panel, sound);	//制御関数

		panel[TASC_release] = false;
		panel[TASC_braking] = false;
		panel[TASC_noches] = 0;
		panel[TASC_failed] = false;
		MasCon_key == Key::SEB ? panel[TASC_power_M] = true : panel[TASC_power] = true;

		if (ATCstatus & ATC_status::TASC_control) {
			MasCon_key == Key::SEB ? panel[TASC_controling_M] = true : panel[TASC_controling] = false;

			//TASC動作
			if (ATCstatus & ATC_status::TASC_doing) {
				if (TASC.control.B > handle.B) {
					handle.P = 0;
					handle.B = TASC.control.B;
					panel[ATC_Panel::TASC_noches] = TASC.control.B + 1;
				}
			}
		}
	}
	else {
		panel[TASC_power] = false;
		panel[TASC_release] = false;
		panel[TASC_braking] = false;
		panel[TASC_controling] = false;
		panel[TASC_noches] = 0;
		panel[TASC_failed] = false;
		panel[TASC_power_M] = false;
		panel[TASC_controling_M] = false;
	}
	panel[TASC_debug] = int(TASC.Limit);


	//ATCブレーキ
	if (ATCstatus & ATC_status::ATC_ON) {
		ATC.Control(S, panel, sound);	//制御関数

		panel[ATC_Panel::ATC_braking] = false;
		if (ATCstatus & ATC_status::ATC_brake) {
			if (ATC.control.B > manual.B) {
				handle.P = 0;
				handle.B = ATC.control.B;
				panel[ATC_Panel::ATC_braking] = true;
			}
		}
	}

	panel[51] = handle.B == specific.E ? handle.B + 1 : handle.B;
	panel[66] = handle.P;
	panel[ATC_Panel::Brake_notches] = handle.B;
	panel[92] = MasCon_key;
	panel[135] = int(TASC.Limit * 10);

	//ATC_B
	ATC.control.B == specific.E ? panel[51] = ATC.control.B + 1 : panel[51] = ATC.control.B;

	Stat = S;
	return handle;
}
DE void SC SetPower(int p) {
	manual.P = p;
}
DE void SC SetBrake(int b) {
	manual.B = b;
	if (b == specific.E) {
		ATO.inEmergency();
		TASC.inEmergency();
	}
}
DE void SC SetReverser(int r) {
	manual.R = r;
}
DE void SC DoorOpen() {
	door = true;
	if (ATCstatus & ATC_status::ATC_ON) {
		//ATC.control.B = specific.E;
		ATO.SignalChange();
		ATC.setSignal();
	}
	if (ATCstatus & ATC_status::ATO_ON) {
		ATCstatus &= ~ATC_status::ATO_control;
		ATCstatus &= ~ATC_status::ATO_doing;
		ATCstatus |= ATC_status::ATO_stopping;
	}
	if (ATCstatus & ATC_status::TASC_ON) {
		ATCstatus &= ~ATC_status::TASC_control;
		ATCstatus &= ~ATC_status::TASC_doing;
		ATCstatus |= ATC_status::TASC_stopping;
		TASC.setStatus(false);	//TASC制御解放
	}
}
DE void SC DoorClose() {
	door = false;
}


DE void SC KeyDown(int k) {
	switch (k) {
	case ATSKeys::S:
		key_S = true;
		break;
	case ATSKeys::A1:
		key_A1 = true;
		break;
	case ATSKeys::A2:
		key_A2 = true;
		break;
	case ATSKeys::B1:
		key_B1 = true;
		break;
	case ATSKeys::B2:
		key_B2 = true;
		break;
	case ATSKeys::C1:
		key_C1 = true;
		break;
	case ATSKeys::C2:
		key_C2 = true;
		break;
	case ATSKeys::D:
		key_D = true;
		break;
	case ATSKeys::E:
		ATO.ChangeMode(-1);
		key_E = true;
		break;
	case ATSKeys::F:
		ATO.ChangeMode(+1);
		key_F = true;
		break;
	case ATSKeys::G:
		SetStatus(false);
		key_G = true;
		break;
	case ATSKeys::H:
		SetStatus(true);
		key_H = true;
		break;
	case ATSKeys::I:
		setKey(-1);
		key_I = true;
		break;
	case ATSKeys::J:
		setKey(+1);
		key_J = true;
		break;
	case ATSKeys::K:
		key_K = true;
		break;
	case ATSKeys::L:
		key_L = true;
		break;
	default:
		break;
	}
}
DE void SC KeyUp(int k) {
	switch (k) {
	case ATSKeys::S:
		key_S = false;
		break;
	case ATSKeys::A1:
		key_A1 = false;
		break;
	case ATSKeys::A2:
		key_A2 = false;
		break;
	case ATSKeys::B1:
		key_B1 = false;
		break;
	case ATSKeys::B2:
		key_B2 = false;
		break;
	case ATSKeys::C1:
		key_C1 = false;
		break;
	case ATSKeys::C2:
		key_C2 = false;
		break;
	case ATSKeys::D:
		key_D = false;
		break;
	case ATSKeys::E:
		key_E = false;
		break;
	case ATSKeys::F:
		key_F = false;
		break;
	case ATSKeys::G:
		key_G = false;
		break;
	case ATSKeys::H:
		key_H = false;
		break;
	case ATSKeys::I:
		key_I = false;
		break;
	case ATSKeys::J:
		key_J = false;
		break;
	case ATSKeys::K:
		key_K = false;
		break;
	case ATSKeys::L:
		key_L = false;
		break;
	default:
		break;
	}
}
DE void SC HornBlow(int k) {

}
DE void SC SetSignal(int a) {
	signal = a;
	LimitSpeed = SpeedLimit[a];

	ATO.SignalChange();
	ATC.setSignal();
}
DE void SC SetBeaconData(Beacon b) {
	switch (b.Num) {
	case ATC_Beacon::notice_force:
	case ATC_Beacon::notice_link:
		ATC.notice(b.Sig, b.Data);
		break;
	case ATC_Beacon::ORP:
		break;
	case ATC_Beacon::TASC_P0:
		TASC.setBeacon(0, b);
		break;
	case ATC_Beacon::TASC_P1:
		TASC.setBeacon(1, b);
		break;
	case ATC_Beacon::TASC_P2:
		TASC.setBeacon(2, b);
		break;
	case ATC_Beacon::TASC_P3:
		TASC.setBeacon(3, b);
		break;
	case ATC_Beacon::TASC_P4:
		TASC.setBeacon(4, b);
		break;
	case ATC_Beacon::TASC_passage:
		TASC.setBeacon(-1, b);
	default:
		break;
	}
}


