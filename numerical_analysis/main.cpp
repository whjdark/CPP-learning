#include"test.h"

#define LAGRANGE_INTER
#define NEWTON_INTER

#define COTES_INTEG
#define COMPOSITE_INTEG
#define GUASS_INTEG
#define ROMBER_INTEG
#define ADAPT_INTEG

#define EULER_ODE
#define RK_ODE
#define RKF45_ODE

int main(int argc, const char* argv[])
{
	printf("===========================================================\n");
	printf("===================NUMERICAL METHOD========================\n");
	printf("==========================WHJ==============================\n");
	printf("=======================2022.1.1============================\n");
	printf("===========================================================\n");

	preface();
	printf("\n");

	printf("===========================================================\n");
	printf("=====================INTERPOLATION=========================\n");
	printf("===========================================================\n");

#ifdef LAGRANGE_INTER
	lagrange_inter_example();
	printf("\n");
#endif

#ifdef NEWTON_INTER
	newton_inter_example();
	printf("\n");
#endif

	printf("===========================================================\n");
	printf("======================INTEGRATION==========================\n");
	printf("===========================================================\n");

#ifdef COTES_INTEG
	cotes_integ_example();
	printf("\n");
#endif

#ifdef COMPOSITE_INTEG
	composite_integ_example();
	printf("\n");
#endif

#ifdef GUASS_INTEG
	guass_integ_example();
	printf("\n");
#endif

#ifdef ROMBER_INTEG
	romber_integ_example();
	printf("\n");
#endif

#ifdef ADAPT_INTEG
	adapt_integ_example();
	printf("\n");
#endif

	printf("===========================================================\n");
	printf("==========================ODE==============================\n");
	printf("===========================================================\n");

#ifdef EULER_ODE
	euler_example();
	printf("\n");
#endif

#ifdef RK_ODE
	RK_example();
	printf("\n");
#endif

#ifdef RKF45_ODE
	RKF45_example();
	printf("\n");
#endif



	return 0;
}