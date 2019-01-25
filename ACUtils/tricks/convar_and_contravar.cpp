#include "convar_and_contravar.h"
#include <functional>

void ConvarAndContraVarSolver::showExample()
{
	using AnimalDoctor = std::function<void(Animal*)>;
	using HorseDoctor = std::function<void(Horse*)>;

	//Covariant
	auto hackenbush = [](Horse*) {
		puts("Treat a horse.");
	};
	auto dolittle = [](Animal*)
	{
		puts("Treat all type animals.");
	};

	HorseDoctor a = hackenbush;
	AnimalDoctor d = dolittle;
	//*Key* - Param Covariant: (Horse*) -> (Animal*)
	HorseDoctor b = dolittle;

	//Test Code
	Cat catLisa;
	Horse horseMark;
	a(&horseMark);
	d(&catLisa);



	//Contracovariant
	using AnimalBreeder = std::function<Animal*(void)>;
	using HorseBreeder = std::function<Horse*(void)>;

	auto horseKeeper_Ben = []() -> Horse*{ return new Horse(); };
	auto farmerOwner_Mike = []() -> Animal* { return new Horse();/*replace with some animal*/};

	HorseBreeder e = horseKeeper_Ben;
	//*Key* - return value (Horse*) -> (Animal*)
	AnimalBreeder g = horseKeeper_Ben;
	AnimalBreeder h = farmerOwner_Mike;


#if _MSVC_LANG >= 201703L

//	template struct you<dolittle>;

#endif
}
