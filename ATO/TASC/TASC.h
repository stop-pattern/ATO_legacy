#ifndef _TASC_
#define _TASC_

#include "../header/ats.h"

class c_TASC {
public:
	void Control(State, int *, int *);	//���t���[������
	Hand control;	//�n���h������l
	int Limit;	//�ڕW���x
	void setBeacon(int, Beacon);	//beacon set
	void setStatus(bool);	//status set
private:
	Beacon P[5][3];	//TASC�n��q
	Beacon passage;	//��ʔ���
};

#endif // !_TASC_