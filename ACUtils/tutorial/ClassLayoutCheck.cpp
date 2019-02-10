#include "ClassLayoutCheck.h"

/*
	VS Compiler:
	/d1 reportSingleClassLayout#ClassName#
	/d1 reportAllClassLayout
*/

#include <iostream>

using namespace std;

/*
	class BaseClass	size(8):
		+---
	 0	| {vfptr}
	 4	| baseVal
		+---

	BaseClass::$vftable@:
		| &BaseClass_meta
		|  0
	 0	| &BaseClass::doSth

	BaseClass::doSth this adjustor: 0
*/
class BaseClass {
public:
	virtual void doSth()
	{
		cout << "Base::doSth" << endl;
	}

	void functionInBase()
	{
		cout << __FUNCTION__ << endl;
	}

public:
	int baseVal;
};

/*
	class DeriveA	size(12):
		+---
	 0	| +--- (base class BaseClass)
	 0	| | {vfptr}
	 4	| | baseVal
		| +---
	 8	| deriveVal_A
		+---

	DeriveA::$vftable@:
		| &DeriveA_meta
		|  0
	 0	| &DeriveA::doSth

	DeriveA::doSth this adjustor: 0
*/
class DeriveA : public BaseClass
{
public:
	virtual void doSth()override
	{
		cout << "DeriveA::doSth" << endl;
	}

	void functionInDeriveA()
	{
		cout << __FUNCTION__ << endl;
	}

protected:
	int deriveVal_A;
};

/*
	class DeriveB	size(12):
		+---
	 0	| +--- (base class BaseClass)
	 0	| | {vfptr}
	 4	| | baseVal
		| +---
	 8	| deriveVal_B
		+---

	DeriveB::$vftable@:
		| &DeriveB_meta
		|  0
	 0	| &DeriveB::doSth

	DeriveB::doSth this adjustor: 0
*/
class DeriveB : protected BaseClass
{
public:
	virtual void doSth()override
	{
		cout << "DeriveB::doSth" << endl;
	}

	void functionInDeriveB()
	{
		cout << __FUNCTION__ << endl;
	}
protected:
	int deriveVal_B;
};

/*
	class MixtureDerive	size(28):
		+---
	 0	| +--- (base class DeriveA)
	 0	| | +--- (base class BaseClass)
	 0	| | | {vfptr}
	 4	| | | baseVal
		| | +---
	 8	| | deriveVal_A
		| +---
	12	| +--- (base class DeriveB)
	12	| | +--- (base class BaseClass)
	12	| | | {vfptr}
	16	| | | baseVal
		| | +---
	20	| | deriveVal_B
		| +---
	24	| deriveVal_Mixture
		+---

	MixtureDerive::$vftable@DeriveA@:
		| &MixtureDerive_meta
		|  0
	 0	| &MixtureDerive::doSth

	MixtureDerive::$vftable@DeriveB@:
		| -12
	 0	| &thunk: this-=12; goto MixtureDerive::doSth

	MixtureDerive::doSth this adjustor: 0
*/
class MixtureDerive : public DeriveA, public DeriveB
{
public:
	virtual void doSth()override
	{
		cout << "MixtureDerive::doSth" << endl;
	}

	void functionInMixtureDerive()
	{
		cout << __FUNCTION__ << endl;
	}
protected:
	int deriveVal_Mixture;
};

/*
	class VirtualDeriveA	size(16):
		+---
	 0	| {vbptr}
	 4	| virtualDeriveVal_A
		+---
		+--- (virtual base BaseClass)
	 8	| {vfptr}
	12	| baseVal
		+---

	VirtualDeriveA::$vbtable@:
	 0	| 0
	 1	| 8 (VirtualDeriveAd(VirtualDeriveA+0)BaseClass)

	VirtualDeriveA::$vftable@:
		| -8
	 0	| &VirtualDeriveA::doSth

	VirtualDeriveA::doSth this adjustor: 8
	vbi:	   class  offset o.vbptr  o.vbte fVtorDisp
		   BaseClass       8       0       4 0
*/
class VirtualDeriveA : virtual public BaseClass
{
public:
	virtual void doSth()override
	{
		cout << "VirtualDeriveA::doSth" << endl;
	}

	void functionInVirtualDeriveA()
	{
		cout << __FUNCTION__ << endl;
	}
protected:
	int virtualDeriveVal_A;
};


/*
	class VirtualDeriveB	size(16):
		+---
	 0	| {vbptr}
	 4	| virtualDeriveVal_B
		+---
		+--- (virtual base BaseClass)
	 8	| {vfptr}
	12	| baseVal
		+---
	
	VirtualDeriveB::$vbtable@:
	 0	| 0
	 1	| 8 (VirtualDeriveBd(VirtualDeriveB+0)BaseClass)
	
	VirtualDeriveB::$vftable@:
		| -8
	 0	| &VirtualDeriveB::doSth
	
	VirtualDeriveB::doSth this adjustor: 8
	vbi:	   class  offset o.vbptr  o.vbte fVtorDisp
	       BaseClass       8       0       4 0
*/
class VirtualDeriveB : virtual public BaseClass
{
public:
	virtual void doSth()override
	{
		cout << "VirtualDeriveB::doSth" << endl;
	}
	void functionInVirtualDeriveB()
	{
		cout << __FUNCTION__ << endl;
	}
protected:
	int virtualDeriveVal_B;
};

/*
	class MixtureDerive2	size(28):
		+---
	 0	| +--- (base class VirtualDeriveA)
	 0	| | {vbptr}
	 4	| | virtualDeriveVal_A
		| +---
	 8	| +--- (base class VirtualDeriveB)
	 8	| | {vbptr}
	12	| | virtualDeriveVal_B
		| +---
	16	| virtualDeriveVal_Mixture
		+---
		+--- (virtual base BaseClass)
	20	| {vfptr}
	24	| baseVal
		+---

	MixtureDerive2::$vbtable@VirtualDeriveA@:
	 0	| 0
	 1	| 20 (MixtureDerive2d(VirtualDeriveA+0)BaseClass)

	MixtureDerive2::$vbtable@VirtualDeriveB@:
	 0	| 0
	 1	| 12 (MixtureDerive2d(VirtualDeriveB+0)BaseClass)

	MixtureDerive2::$vftable@:
		| -20
	 0	| &MixtureDerive2::doSth

	MixtureDerive2::doSth this adjustor: 20
	vbi:	   class  offset o.vbptr  o.vbte fVtorDisp
		   BaseClass      20       0       4 0
*/
class MixtureDerive2 : public VirtualDeriveA, public VirtualDeriveB
{
public:
	virtual void doSth()override
	{
		cout << "MixtureDerive2::doSth" << endl;
	}

	void functionInVirtualMixtureDerive2()
	{
		cout << __FUNCTION__ << endl;
	}

protected:
	int virtualDeriveVal_Mixture;
};

void ClassLayoutCheck::printClassLayout()
{
	intptr_t BaseClass::* p1 = nullptr;
	intptr_t BaseClass::* p2 = &BaseClass::baseVal;

	//	typedef void (*FuncPtr)();
	typedef void (BaseClass::*BaseClassFunPtr)();
	BaseClassFunPtr p3 = &BaseClass::doSth;
//	BaseClassFunPtr p3_null = nullptr;

	printf("Base Class Pointer: %p\n", p1);
	printf("Base Class Value Pointer: %p\n\n", p2);
	void (BaseClass::*p4)() = &BaseClass::functionInBase;
//	printf("Base Class Owner Function: %p\n", p4);

	/*
		注意：
			函数指针地址和虚函数表中的函数地址，两者均不是实际函数的入口地址
	*/
	BaseClass b;
	printf("Base Class vftpr :%p\n", b);
//	printf("Base Class Virtual function address(nullptr): %p\n", p3_null);	
	printf("Base Class Virtual function address: %p\n", p3);	

	b.doSth();


	DeriveA da;
	printf("DeriveA Class vftpr :%x\n", da);
	void (DeriveA::*p5)() = &DeriveA::doSth;
	printf("DeriveA override function address:%p\n", p5);

	DeriveB ba;
	printf("DeriveA Class vftpr :%x\n", ba);
	void (DeriveB::*p6)() = &DeriveB::doSth;
	printf("DeriveB override function address:%p\n", p6);

	printf("Finished.\n");

//	BaseClass* px = &ba;

}
