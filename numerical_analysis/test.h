#pragma once

//interp
#include "lagrange_inter.h"
#include "newton_inter.h"
//integration
#include "integ_factory.h"
//ODE
#include "euler.h"
#include "RK.h"

//preface
void preface();

//tests

//interpolation
int lagrange_inter_example();
int newton_inter_example();

//integration
int cotes_integ_example();
int composite_integ_example();
int guass_integ_example();
int romber_integ_example();
int adapt_integ_example();

//ODE
int euler_example();
int RK_example();
int RKF45_example();