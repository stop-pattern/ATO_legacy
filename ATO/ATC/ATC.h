#ifndef _ATC_
#define _ATC_

#include "../header/ats.h"

#define BRAKE_HALF 1500
#define FORWARDNOTICE 1900

class c_ATC{
public:
	void Control(State, int *, int *);	//���t���[������
	void setSignal(void);	//�M���ݒ�
	Hand control;
	void notice(int, int);	//�O���\��
	int limit;	//�������x
private:
	long int brake_cnt;	//�n�[�t�u���[�L�p�J�E���^
	bool changeSignal;	//�����ω�
	bool isNotice;	//�O���\��
	int notice_panel;	//�O���\��index
	long int notice_time;	//�O���\���p�J�E���^
};


#endif // !_ATC_
