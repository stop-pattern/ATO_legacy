#include "../header/define.h"
#include "../header/Header.h"
#include "../ATO/ATO.h"
#include"../TASC/TASC.h"

string dllPath;	//.dllのpath
string iniPath;	//.iniのpath

int signal;	//信号インデックス
double speed;	//現在速度
double distance;	//現在位置

bool door;	//ドア状態
int ATCstatus;	//ATC状態
int LimitSpeed;	//制限速度
double accelaration;	//加減速度
bool key_S, key_A1, key_A2, key_B1, key_B2, key_C1, key_C2, key_D, key_E, key_F, key_G, key_H, key_I, key_J, key_K, key_L;	//ATSkey

int MasCon_key;	//マスコンキー
int ATC_SW;	//ATC切り替えSW
int lag_cnt;	//ラグ用カウンター

Spec specific;	//車両性能
State Stat;	//車両状態(前フレーム)
Hand manual;	//手動ハンドル位置
Hand handle;	//ハンドル制御値

c_ATO ATO;	//ATO
c_TASC TASC;	//TASC
