#pragma once
#ifndef HEADER
#define HEADER

#include <string>
#include <math.h>
#include "ats.h"
#include "../ATO/ATO.h"
#include "../TASC/TASC.h"
#include "../ATC/ATC.h"
#include "../INI/Getini.h"

#define LAG	200	//判定ラグ[ms]


extern int signal;	//信号インデックス
extern double speed;	//現在速度
extern double distance;	//現在位置

extern bool door;	//ドア状態
extern int ATCstatus;	//ATC状態
extern int LimitSpeed;	//制限速度
extern double accelaration;	//加減速度[km/h/s]
extern bool key_S, key_A1, key_A2, key_B1, key_B2, key_C1, key_C2, key_D, key_E, key_F, key_G, key_H, key_I, key_J, key_K, key_L;	//ATSkey

extern int MasCon_key;	//マスコンキー
extern int ATC_SW;	//ATC切り替えSW
extern int lag_cnt;	//ラグ用カウンター
extern bool isLoad;	//ラグ制御伝達用

extern Spec specific;	//車両性能
extern State Stat;	//車両状態(前フレーム)
extern Hand manual;	//手動ハンドル位置
extern Hand handle;	//ハンドル制御値

extern c_ATO ATO;	//ATO
extern c_TASC TASC;	//TASC
extern c_ATC ATC;	//ATC
extern c_INI INI;	//INI

void reload(void);	//駅ジャンプ時再読み込み処理
void SetStatus();	//ATO/TASCスイッチ操作
void setKey(int);	//マスコンキー操作

#endif // !HEADER