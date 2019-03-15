﻿#pragma once

#define DECELERATION_PATTERN 19.5	//P接近計算用減速定数
#define DECELERATION_BRAKE 19.5	//常用P計算用減速定数
#define DECELARATION_EMR 25	//非常P計算用減速定数
#define ATC_MAX 120.0	//ATC最高速度[km/h]
#define NOTICE_TIME 1.5	//P接近表示時間[s]
#define PATTERN_BRAKE 5	//ATC緩和パターン段数

#define STOPLIMIT_UPDATE 
/*
(StopLimit != false && rand() % STOPLIMIT_UPDATE == 0) { panel[]; }	//←とかやる？
*/

#define SPEED_ATS_OFF 15	//ATS非保安(構内)時照査速度

//信号インデックス
const int SpeedLimit[256] = {0,0,0,0,0,0,0,0,0,0,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,0,0,125,130,135,140,145,150,155,160,165,170,175,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,25,45,55,65,75,90,100,110,120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


enum ATC_status {
	OFF = 0x0000,	//SW=off
	ON = 0x8000,	//SW=on but mascon_key=off

	ATC_ON = 0x0800,
	ATC_brake = 0x0400,

	ATO_OFF = 0x0000,	//SW!=ATO
	ATO_ON = 0x0080,	//SW==ATO
	ATO_control = 0x0040,	//controling
	ATO_doing = 0x0020,
	ATO_stopping = 0x0010,	//stopping & brake

	TASC_OFF = 0x0000,	//SW!=TASC
	TASC_ON = 0x0008,	//SW==TASC
	TASC_control = 0x0004,	//controling
	TASC_doing = 0x0002,	//braking
	TASC_stopping = 0x0001,	//stoppping & brake

	/*
	0b0000
	  ||||
	  ABCD

		A:ON/OFF	0x8
		B:control	0x4
		C:doing		0x2
		D:parking	0x1
	*/
};


enum ATC_Beacon {
	notice_force = 7,	//前方予告（強制）
	notice_link = 31,	//前方予告（リアルタイム）
	ORP = 12,	//ORP添線
	TASC_P4 = 21,	//
	TASC_P3 = 22,	//
	TASC_P2 = 23,	//
	TASC_P1 = 24,	//
	TASC_P0 = 25,	//
	TASC_passage = 256,	//TASC停通判定
};


enum ATC_Panel {
	Brake_notches = 67,	//
	notice = 133,	//前方予告
	TASC_power = 136,	//TASC灯
	TASC_release,	//TASC解放灯
	TASC_braking,	//TASC制御灯
	TASC_controling,	//TASCブレーキ灯
	TASC_noches,	//TASC制動ノッチ
	TASC_failed,	//TASC故障
	TASC_power_M,	//TASC灯	マスコンキー西武のみ
	TASC_controling_M,	//TASC制御灯	マスコンキー西武のみ
	ATO_P,	//ATO動作P段数
	ATO_B,	//ATO動作B段数
	ATO_power,	//ATO電源灯
	ATO_controling,	//ATO動作
	TASC_debug,	//TASCパターン指針
	ATO_debug,	//ATOパターン指針
	Reservation,	//予約領域

	TIS_send = 194,
	TIS_recieve_VVVF_0,
	TIS_recieve_VVVF_1,
	TIS_recieve_VVVF_2,
	TIS_recieve_VVVF_3,
	TIS_recieve_VVVF_4,
	TIS_recieve_VVVF_5,
	
	TIS_recieve_brake_01 = 207,
	TIS_recieve_brake_02,
	TIS_recieve_brake_11,
	TIS_recieve_brake_12 = 215,
	TIS_recieve_brake_21,
	TIS_recieve_brake_22,
	TIS_recieve_brake_31,
	TIS_recieve_brake_32,
	TIS_recieve_brake_41,
	TIS_recieve_brake_42,
	TIS_recieve_brake_51,
	TIS_recieve_brake_52,
	TIS_recieve_brake_61,
	TIS_recieve_brake_62,
	TIS_recieve_brake_71,
	TIS_recieve_brake_72,
	TIS_recieve_brake_81,
	TIS_recieve_brake_82,
	TIS_recieve_brake_91,
	TIS_recieve_brake_92
};


enum ATC_Sound {
	ATC_bell = 2,	//ATC単打ベル
	ATC_notice = 4,	//前方予告
};


enum ATC_Signal {
	ATC_02 = 9,
	ATC_01,
	ATC_05,
	ATC_10,
	ATC_15,
	ATC_20,
	ATC_25,
	ATC_30,
	ATC_35,
	ATC_40,
	ATC_45,
	ATC_50,
	ATC_55,
	ATC_60,
	ATC_65,
	ATC_70,
	ATC_75,
	ATC_80,
	ATC_85,
	ATC_90,
	ATC_95,
	ATC_100,
	ATC_105,
	ATC_110,
	ATC_115,
	ATC_120,
	ATC_ORP = 35,

};


enum ATC_Switch {
};

enum Key{
KeyOff = 0,	//off
TRTA = 1,	//TRTA(S)
TOB = 2,	//東武(T)
TKK = 3,	//東急(K) / 横高(Y)
SEB = 4,	//西武(M)
SOT = 5,	//相鉄(？)
JNR = 6,	//JNR(K) / 伊豆急(？)
OER = 7,	//小田急(E)
TOY = 8,	//東葉(？)
};


// KeyCode
#define ATO出発 1&&2||1&&3
#define ATOモードSW右回し 0&&3
#define ATOモードSW左回し 0&&4
#define TASC解放 0&&9