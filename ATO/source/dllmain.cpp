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
	MasCon_key;	// panel[92];
	ATC_SW = panel[72];

	handle.P = manual.P;
	handle.B = manual.B;
	handle.R = manual.R;

	if (S.T - lag_cnt >= LAG) {
		ATO.Control(S, panel, sound);
		lag_cnt = S.T;
	}

	if (accelaration > 7.5) {
		//	handle.B = specific.E;
	}

	switch (ATCstatus) {
	case ATC_status::ATO_ON:
	case ATC_status::ATO_driving:
	case ATC_status::ATO_stopping:
	case ATC_status::ATO_waiting:
	case ATC_status::ATO_TASC_control:
	case ATC_status::ATO_TASC_brake:
		if (manual.B == 0 || manual.P == 0) {
			handle.P = ATO.control.P;
		}
		if (manual.P == 0 || manual.B < ATO.control.B) {
			handle.B = ATO.control.B;
		}
	case ATC_status::TASC_ON:
	case ATC_status::TASC_control:
	case ATC_status::TASC_brake:
		if (TASC.control.P > handle.P) {
			handle.P = TASC.control.P;
		}
		if (TASC.control.B > handle.B) {
			handle.B = TASC.control.B;
		}
		break;
	default:
		break;
	}
	if (signal > 9 && signal < 36) {
		for (size_t i = 101; i < 131; i++) {
			panel[i] = false;
		}
		panel[LimitSpeed / 5 + 102] = true;
	}
	panel[51] = handle.B;
	panel[66] = handle.P;
	panel[67] = manual.B;
	panel[92] = MasCon_key;
	panel[135] = ATO.Limit * 10;

	Stat = S;
	return handle;
}
DE void SC SetPower(int p) {
	manual.P = p;
}
DE void SC SetBrake(int b) {
	manual.B = b;
}
DE void SC SetReverser(int r) {
	manual.R = r;
}
DE void SC DoorOpen() {
	switch (ATCstatus / 10) {
	case 1:	//ATO
		ATCstatus = ATC_status::ATO_stopping;
		break;
	case 2:	//TASC
		ATCstatus = ATC_status::TASC_waiting;
		break;
	default:
		break;
	}
	door = true;
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
}
DE void SC SetBeaconData(Beacon b) {
	switch (b.Num) {
	case 0:
	default:
		break;
	}
}


