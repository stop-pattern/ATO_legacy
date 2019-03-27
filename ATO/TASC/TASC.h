#ifndef _TASC_
#define _TASC_

#include "../header/ats.h"

class c_TASC {
public:
	void Control(State, int *, int *);	//���t���[������
	Hand control;	//�n���h������l
	void setBeacon(int, Beacon);	//beacon set
	void setStatus(bool);	//status set
	void inEmergency(void);	//������
	double Limit;	//�ڕW���x
	bool isControl;	//���䒆
private:
	double Location;	//�ڕW�ʒu
	double Distance;	//�c����
	Beacon P[5][3];	//TASC�n��q
	Beacon passage;	//��ʔ���
};

#endif // !_TASC_