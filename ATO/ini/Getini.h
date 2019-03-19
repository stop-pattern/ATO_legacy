#pragma once
#include "../header/define.h"
#include "Getini.h"

using namespace std;

class c_INI {
public:
	//variant
	isATC;
	isATO;
	isTASC;

	//function
	void getIni(string);
	string GetPath(HMODULE);

private:
	string dllPath;	//.dllのpath
	string iniPath;	//.iniのpath
};
