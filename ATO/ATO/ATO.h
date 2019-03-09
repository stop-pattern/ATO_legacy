#ifndef _ATO_
#define _ATO_

#include "../header/ats.h"


namespace ATO {
	void Control(State, int *, int *);	//���t���[������
	void CSC();	//ATO�葬
	bool Departure(void);	//�o���{�^������
	void SignalChange();	//�����ύX
	void ChangeMode(int);	//�^�]���[�h�ύX

	extern int Limit;	//�ڕW���x
	extern int Mode;	//ATO�^�]���[�h
	extern Hand control;	//�n���h������l

	extern bool isCSC;	//ATO�葬
	extern int button_buf;	//�{�^�����������O����p

	extern const int Reference_Speed[4][256];	//ATO�ڕW���x
	enum ATOmode {
		ATC�Ǐ],
		ATO����,
		ATO��,
		ATO�x��,
	};
};

#endif // _ATO_