#include "test.h"

//test loop
constexpr size_t loop_time = 100000;

void preface()
{
	printf("each time test repeats %lld loops \n", loop_time);
	printf("Timer & dummy code average time");
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			double tmp = 0.;
			dont_opt_away(tmp);
		}
	}
}

int lagrange_inter_example() 
{
	std::vector<Point2D<double>> points_2;
	std::vector<Point2D<double>> points_4;
	for (auto i : Range(11, 13)) {
		points_2.emplace_back(i, log(i));
	}
	for (auto i : Range(11, 15)) {
		points_4.emplace_back(i, log(i));
	}

	/*int index = 0;
	for (auto& pt : points) {
		printf("point[%i]: x:%f y:%f \n", index, pt.x, pt.y);
		index++;
	}*/

	float target_x = 11.75;
	float real_y = log(target_x);

	auto la_inter = LagrangeInterp(points_2);
	double target_y = la_inter.calc(target_x);
	printf("2 point lagrange_inter: target_y:%f real_y:%f diff:%f \n", target_y, real_y, abs(target_y - real_y));

	la_inter.setPoints(points_4);
	target_y = la_inter.calc(target_x);
	printf("3 point lagrange_inter: target_y:%f real_y:%f diff:%f \n", target_y, real_y, abs(target_y - real_y));

	printf("%lld order Lagrange interp average time", points_4.size());
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			double tmp = la_inter.calc(target_x);
			dont_opt_away(tmp);
		}
	}

	return 0;
}

int newton_inter_example() 
{
	std::vector<Point2D<double>> eg_0;
	std::vector<Point2D<double>> eg_1;
	std::vector<Point2D<double>> eg_2{ Point2D<double>(0.4, 0.41075), 
																Point2D<double>(0.55, 0.57815), 
																Point2D<double>(0.65, 0.69675), 
																Point2D<double>(0.80, 0.88811) };

	for (auto i : Range(0, 4)) {
		eg_0.emplace_back(i, pow(3, i));
	}
	for (auto i : Range(10, 13)) {
		eg_1.emplace_back(pow(i, 2), i);
	}
	auto nt_inter = NewtonInterp(eg_0);
	double target_x = 0.5;
	double real_y = pow(3, target_x);
	double target_y = nt_inter.calc(target_x);
	printf("3^x newton_inter: target_y:%f real_y:%f diff:%f \n", target_y, real_y, abs(target_y - real_y));

	target_x = 115;
	real_y = pow(115, 0.5);
	nt_inter.setPoints(eg_1);
	target_y = nt_inter.calc(target_x);
	printf("x^1/2 newton_inter: target_y:%f real_y:%f diff:%f \n", target_y, real_y, abs(target_y - real_y));

	target_x = 0.596;
	real_y = 0.62836;
	nt_inter.setPoints(eg_2);
	target_y = nt_inter.calc(target_x);
	printf("example 2 newton_inter: target_y:%f real_y:%f diff:%f \n", target_y, real_y, abs(target_y - real_y));

	printf("%lld order newton average time", eg_0.size());
	{
		Timer t("");
		auto nt_inter_tmp = NewtonInterp(eg_0);
		for (int i = 0; i < loop_time; i++) {
			double tmp = nt_inter_tmp.calc(target_x);
			dont_opt_away(tmp);
		}
	}

	return 0;
}




int cotes_integ_example() 
{
	auto integ_factory = std::unique_ptr<IntegFactory>(new IntegFactory());
	auto cts_integ = static_cast<CotesInteg*>(integ_factory->create(IntegFactory::IntegType::COTES));

	int order = 3;
	cts_integ->setOrder(order);
	double res = cts_integ->calc(1, 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
	printf("%d-st order newton-cotes (0->1)f(x)=sinx/x results:%.7f \n", order, res);

	order = 2; // simpson
	cts_integ->setOrder(order);
	res = cts_integ->calc(2, 1, [](double x) -> double {return exp(1. / x); });
	printf("%d-st order newton-cotes (1->2)f(x)=e^(1/x) results:%.7f \n", order, res);

	printf("%d-st order newton-cotes average time", order);
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			double tmp = cts_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
			dont_opt_away(tmp);
		}
	}

	delete cts_integ;
	return 0;
}

int composite_integ_example() 
{
	auto integ_factory = std::unique_ptr<IntegFactory>(new IntegFactory());
	auto cp_integ =  static_cast<CompositeInteg*>(integ_factory->create(IntegFactory::IntegType::COMPOSITE));

	int subInterval = 8;
	cp_integ->setOrder(CompositeInteg::Order::MIDPOINT);
	cp_integ->setSubInterval(subInterval);
	double res = cp_integ->calc(1, 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
	printf("%d-section Composite midpoint (0->1)f(x)=sinx/x results:%.7f \n", subInterval, res);

	subInterval = 8;
	cp_integ->setOrder(CompositeInteg::Order::TRAPEZOIDAL);
	cp_integ->setSubInterval(subInterval);
	res = cp_integ->calc(1, 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
	printf("%d-section Composite Trapezoid (0->1)f(x)=sinx/x results:%.7f \n", subInterval, res);

	subInterval = 4;
	cp_integ->setOrder(CompositeInteg::Order::SIMPSON);
	cp_integ->setSubInterval(subInterval);
	res = cp_integ->calc(1, 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
	printf("%d-section Composite Simpson (0->1)f(x)=sinx/x results:%.7f \n", subInterval, res);

	printf("%d-section Composite Simpson average time", subInterval);
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			double tmp = cp_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
			dont_opt_away(tmp);
		}
	}

	delete cp_integ;
	return 0;
}

int guass_integ_example() 
{
	auto integ_factory = std::unique_ptr<IntegFactory>(new IntegFactory());
	auto gs_integ = static_cast<GuassInteg*>(integ_factory->create(IntegFactory::IntegType::GUASS));

	int point = 5;
	gs_integ->setType(GuassInteg::Type::Legendre);
	gs_integ->setPoint(point);
	double res = gs_integ->calc(ht::constants::pi / 2., 0, [](double x) -> double {return pow(x, 2) * cos(x); });
	printf("%d-point Guass-Legendre (0->pi/2)f(x)=x^2*cosx results:%.7f \n", point, res);
	res = gs_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
	printf("%d-point Guass-Legendre (0->1)f(x)=sinx/x results:%.7f \n", point, res);
	res = gs_integ->calc(3., 0., [](double x) -> double {return sin(4 * x) * exp((-2.) * x); });
	printf("%d-point Guass-Legendre (0->3)sin(4x) * e^(-2x) results:%.7f \n", point, res);
	res = gs_integ->calc(3., 0., [](double x) -> double {return sin(2 * x) / (1 + pow(x, 2)); });
	printf("%d-point Guass-Legendre (0->3)sin(2x) / (1+x^2) results:%.7f \n", point, res);
	res = gs_integ->calc(2., 1 / (2 * ht::constants::pi), [](double x) -> double {return sin(1. / x); });
	printf("%d-point Guass-Legendre (1/2pi->2)sin(1/x) results:%.7f \n", point, res);
	res = gs_integ->calc(4., 0., [](double x) -> double {return 13. * (x - pow(x, 2)) * exp(-1.5 * x); });
	printf("%d-point Guass-Legendre (0->4)13(x-x^2)e^(-1.5x) results:%.7f \n", point, res);
	res = gs_integ->calc(1., -1., [](double x) -> double {return exp(x); });
	printf("%d-point Guass-Legendre (-1->1)f(x)=e^x results:%.7f \n", point, res);
	res = gs_integ->calc(1., -1., [](double x) -> double {return exp(x) / sqrt(1 - pow(x, 2)); });
	printf("%d-point Guass-Legendre (-1->1)f(x)=e^x/(1-x^2)^(0.5) results:%.7f \n", point, res);

	gs_integ->setType(GuassInteg::Type::Chebyshev);
	gs_integ->setPoint(point);
	res = gs_integ->calc(1., -1., [](double x) -> double {return exp(x); });
	printf("%d-point Guass-Chebyshev (-1->1)f(x)=e^x/(1-x^2)^(0.5) results:%.7f \n", point, res);

	gs_integ->setType(GuassInteg::Type::Legendre);
	gs_integ->setPoint(point);
	printf("%d-point Guass-Legendre average time", point);
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			double tmp = gs_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
			dont_opt_away(tmp);
		}
	}

	delete gs_integ;
	return 0;
}

int romber_integ_example() 
{
	auto integ_factory = std::unique_ptr<IntegFactory>(new IntegFactory());
	auto rb_integ = static_cast<RomberInteg*>(integ_factory->create(IntegFactory::IntegType::ROMBER));

	int iter = 100;
	double tolerance = 1e-8;
	rb_integ->setTol(tolerance);
	rb_integ->setMaxIter(iter);
	double res = rb_integ->calc(ht::constants::pi / 2., 0, [](double x) -> double {return pow(x, 2) * cos(x); });
	printf("Tolerance:%.1e Romber (0->pi/2)f(x)=x^2*cosx results:%.7f \n", tolerance, res);
	res = rb_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
	printf("Tolerance:%.1e Romber (0->1)f(x)=sinx/x results:%.7f \n", tolerance, res);
	res = rb_integ->calc(3., 0., [](double x) -> double {return sin(4 * x) * exp((-2.) * x); });
	printf("Tolerance:%.1e Romber (0->3)sin(4x) * e^(-2x) results:%.7f \n", tolerance, res);
	res = rb_integ->calc(3., 0., [](double x) -> double {return sin(2 * x) / (1+pow(x, 2)); });
	printf("Tolerance:%.1e Romber (0->3)sin(2x) / (1+x^2) results:%.7f \n", tolerance, res);
	res = rb_integ->calc(2., 1 / (2 * ht::constants::pi), [](double x) -> double {return sin(1. / x); });
	printf("Tolerance:%.1e Romber (1/2pi->2)sin(1/x) results:%.7f \n", tolerance, res);
	res = rb_integ->calc(4., 0., [](double x) -> double {return 13. * (x - pow(x, 2)) * exp(-1.5 * x); });
	printf("Tolerance:%.1e Romber (0->4)13(x-x^2)e^(-1.5x) results:%.7f \n", tolerance, res);
	res = rb_integ->calc(1., -1., [](double x) -> double {return exp(x); });
	printf("Tolerance:%.1e Romber (-1->1)f(x)=e^x results:%.7f \n", tolerance, res);
	
	printf("Tolerance:%.1e Romber average time", tolerance);
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			double tmp = rb_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
			dont_opt_away(tmp);
		}
	}

	delete rb_integ;
	return 0;
}

int adapt_integ_example() 
{
	auto integ_factory = std::unique_ptr<IntegFactory>(new IntegFactory());
	auto ad_integ = static_cast<AdaptInteg*>(integ_factory->create(IntegFactory::IntegType::ADAPT));

	int iter = 100;
	double tolerance = 1e-8;
	ad_integ->setTol(tolerance);
	ad_integ->setMaxIter(iter);
	double res = ad_integ->calc(ht::constants::pi / 2., 0, [](double x) -> double {return pow(x, 2) * cos(x); });
	printf("Tolerance:%.1e Adaptive Simpson (0->pi/2)f(x)=x^2*cosx results:%.7f \n", tolerance, res);
	res = ad_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
	printf("Tolerance:%.1e Adaptive Simpson (0->1)f(x)=sinx/x results:%.7f \n", tolerance, res);
	res = ad_integ->calc(3., 0., [](double x) -> double {return sin(4 * x) * exp((-2.) * x); });
	printf("Tolerance:%.1e Adaptive Simpson (0->3)sin(4x) * e^(-2x) results:%.7f \n", tolerance, res);
	res = ad_integ->calc(3., 0., [](double x) -> double {return sin(2 * x) / (1 + pow(x, 2)); });
	printf("Tolerance:%.1e Adaptive Simpson (0->3)sin(2x) / (1+x^2) results:%.7f \n", tolerance, res);
	res = ad_integ->calc(2., 1 / (2 * ht::constants::pi), [](double x) -> double {return sin(1. / x); });
	printf("Tolerance:%.1e Adaptive Simpson (1/2pi->2)sin(1/x) results:%.7f \n", tolerance, res);
	res = ad_integ->calc(4., 0., [](double x) -> double {return 13. * (x - pow(x, 2)) * exp(-1.5 * x); });
	printf("Tolerance:%.1e Adaptive Simpson (0->4)13(x-x^2)e^(-1.5x) results:%.7f \n", tolerance, res);
	res = ad_integ->calc(1., -1., [](double x) -> double {return exp(x); });
	printf("Tolerance:%.1e Adaptive Simpson (-1->1)f(x)=e^x results:%.7f \n", tolerance, res);

	printf("Tolerance:%.1e Adaptive Simpson average time", tolerance);
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			double tmp = ad_integ->calc(1., 0, [](double x) -> double {if (x == 0.0) return 1.0; else return sin(x) / x; });
			dont_opt_away(tmp);
		}
	}

	delete ad_integ;
	return 0;
}

int euler_example()
{
	auto el = Euler();

	int interval = 5;
	double b = 0.5, a = 0.;
	double ya = 2.0;
	auto dy = [](double x, double y) -> double {return (-2.) * y - 4. * x; };

	el.setType(Euler::Type::EXPLICIT);
	std::vector<Point2D<double>> res_explicit = el.calc(dy, b, a, ya, interval);
	el.setType(Euler::Type::IMPLICIT);
	std::vector<Point2D<double>> res_implicit = el.calc(dy, b, a, ya, interval);
	el.setType(Euler::Type::IMPROVED);
	std::vector<Point2D<double>> res_impoved = el.calc(dy, b, a, ya, interval);

	printf("Euler method: (0->0.5)y'=-2y-4x;y0=2\n");
	printf("X\tEXPLICIT\tIMPLICIT\tIMPROVED\n");
	for(auto i : Range(0, res_explicit.size())){
		printf("%.2f\t%.6f\t%.6f\t%.6f\n", res_explicit[i].x, res_explicit[i].y, res_implicit[i].y, res_impoved[i].y);
	}

	interval = 10;
	b = 0.02, a = 0.;
	ya = 0.;
	auto dy_1 = [](double x, double y) -> double {return 0.5 * (-y + pow(x, 2) + 4 * x - 1); };

	el.setType(Euler::Type::EXPLICIT);
	res_explicit = el.calc(dy_1, b, a, ya, interval);
	el.setType(Euler::Type::IMPLICIT);
	res_implicit = el.calc(dy_1, b, a, ya, interval);
	el.setType(Euler::Type::IMPROVED);
	res_impoved = el.calc(dy_1, b, a, ya, interval);

	printf("Euler method: (0->0.02)y'=0.5(-y+x^2+4x-1);y0=0\n");
	printf("X\tEXPLICIT\tIMPLICIT\tIMPROVED\n");
	for (auto i : Range(0, res_explicit.size())) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", res_explicit[i].x, res_explicit[i].y, res_implicit[i].y, res_impoved[i].y);
	}

	interval = 12;
	b = 13., a = 1.;
	ya = 0.;
	auto dy_2 = [](double x, double y) -> double {return -y + x - exp(-1.); };

	el.setType(Euler::Type::EXPLICIT);
	res_explicit = el.calc(dy_2, b, a, ya, interval);
	el.setType(Euler::Type::IMPLICIT);
	res_implicit = el.calc(dy_2, b, a, ya, interval);
	el.setType(Euler::Type::IMPROVED);
	res_impoved = el.calc(dy_2, b, a, ya, interval);

	printf("Euler method: (0->2)y'=1 - 2xy/(1+x^2);y0=0\n");
	printf("X\tEXPLICIT\tIMPLICIT\tIMPROVED\n");
	for (auto i : Range(0, res_explicit.size())) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", res_explicit[i].x, res_explicit[i].y, res_implicit[i].y, res_impoved[i].y);
	}

	interval = 4;
	b = 2.0, a = 0.;
	ya = 0.;
	auto dy_3 = [](double x, double y) -> double {return 1. - (2. * x * y) / (1. + pow(x, 2.)); };

	el.setType(Euler::Type::EXPLICIT);
	res_explicit = el.calc(dy_3, b, a, ya, interval);
	el.setType(Euler::Type::IMPLICIT);
	res_implicit = el.calc(dy_3, b, a, ya, interval);
	el.setType(Euler::Type::IMPROVED);
	res_impoved = el.calc(dy_3, b, a, ya, interval);

	printf("Euler method: (0->2)y'=1 - 2xy/(1+x^2);y0=0\n");
	printf("X\tEXPLICIT\tIMPLICIT\tIMPROVED\n");
	for (auto i : Range(0, res_explicit.size())) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", res_explicit[i].x, res_explicit[i].y, res_implicit[i].y, res_impoved[i].y);
	}

	el.setType(Euler::Type::EXPLICIT);
	printf("Euler method: (0->2)y'=1 - 2xy/(1+x^2);y0=0;h=%.3f average time", (b-a)/interval);
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			auto tmp = el.calc(dy_3, b, a, ya, interval);
			dont_opt_away(tmp);
		}
	}

	return 0;
}

int RK_example()
{
	auto rk = RK();

	int interval = 5;
	double b = 0.5, a = 0.;
	double ya = 2.0;
	auto dy = [](double x, double y) -> double {return (-2.) * y - 4. * x; };

	rk.setType(RK::Type::MIDPOINT);
	std::vector<Point2D<double>> res_mid = rk.calc(dy, b, a, ya, interval);
	rk.setType(RK::Type::HEUN);
	std::vector<Point2D<double>> res_heun = rk.calc(dy, b, a, ya, interval);
	rk.setType(RK::Type::RK4);
	std::vector<Point2D<double>> res_rk4 = rk.calc(dy, b, a, ya, interval);

	printf("RK method: (0->0.5)y'=-2y-4x;y0=2\n");
	printf("X\tMIDPOINT\tHEUN\t\tRK4\n");
	for (auto i : Range(0, res_mid.size())) {
		printf("%.2f\t%.6f\t%.6f\t%.6f\n", res_mid[i].x, res_mid[i].y, res_heun[i].y, res_rk4[i].y);
	}

	interval = 10;
	b = 0.02, a = 0.;
	ya = 0.;
	auto dy_1 = [](double x, double y) -> double {return 0.5 * (-y + pow(x, 2) + 4 * x - 1); };

	rk.setType(RK::Type::MIDPOINT);
	res_mid = rk.calc(dy_1, b, a, ya, interval);
	rk.setType(RK::Type::HEUN);
	res_heun = rk.calc(dy_1, b, a, ya, interval);
	rk.setType(RK::Type::RK4);
	res_rk4 = rk.calc(dy_1, b, a, ya, interval);

	printf("RK method: (0->0.02)y'=0.5(-y+x^2+4x-1);y0=0\n");
	printf("X\tMIDPOINT\tHEUN\t\tRK4\n");
	for (auto i : Range(0, res_mid.size())) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", res_mid[i].x, res_mid[i].y, res_heun[i].y, res_rk4[i].y);
	}

	interval = 12;
	b = 13., a = 1.;
	ya = 0.;
	auto dy_2 = [](double x, double y) -> double {return -y + x - exp(-1.); };

	rk.setType(RK::Type::MIDPOINT);
	res_mid = rk.calc(dy_2, b, a, ya, interval);
	rk.setType(RK::Type::HEUN);
	res_heun = rk.calc(dy_2, b, a, ya, interval);
	rk.setType(RK::Type::RK4);
	res_rk4 = rk.calc(dy_2, b, a, ya, interval);

	printf("RK method: (1->13)y'=-y+x-e^-1;y0=0\n");
	printf("X\tMIDPOINT\tHEUN\t\tRK4\n");
	for (auto i : Range(0, res_mid.size())) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", res_mid[i].x, res_mid[i].y, res_heun[i].y, res_rk4[i].y);
	}

	interval = 6;
	b = 3.0, a = 0.;
	ya = 1.;
	auto dy_3 = [](double x, double y) -> double {return 0.5 * (x - y); };

	rk.setType(RK::Type::MIDPOINT);
	res_mid = rk.calc(dy_3, b, a, ya, interval);
	rk.setType(RK::Type::HEUN);
	res_heun = rk.calc(dy_3, b, a, ya, interval);
	rk.setType(RK::Type::RK4);
	res_rk4 = rk.calc(dy_3, b, a, ya, interval);

	printf("RK method: (1->3)y'=(x-y)/2;y0=1\n");
	printf("X\tMIDPOINT\tHEUN\t\tRK4\n");
	for (auto i : Range(0, res_mid.size())) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", res_mid[i].x, res_mid[i].y, res_heun[i].y, res_rk4[i].y);
	}

	interval = 4;
	b = 2.0, a = 0.;
	ya = 0.;
	auto dy_4= [](double x, double y) -> double {return 1. - (2. * x * y) / (1. + pow(x, 2.)); };

	rk.setType(RK::Type::MIDPOINT);
	res_mid = rk.calc(dy_4, b, a, ya, interval);
	rk.setType(RK::Type::HEUN);
	res_heun = rk.calc(dy_4, b, a, ya, interval);
	rk.setType(RK::Type::RK4);
	res_rk4 = rk.calc(dy_4, b, a, ya, interval);

	printf("RK method: (0->2)y'=1 - 2xy/(1+x^2);y0=0\n");
	printf("X\tMIDPOINT\tHEUN\t\tRK4\n");
	for (auto i : Range(0, res_mid.size())) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", res_mid[i].x, res_mid[i].y, res_heun[i].y, res_rk4[i].y);
	}

	printf("RK4 method: (0->2)y'=1 - 2xy/(1+x^2);y0=0;h=%.3f average time", (b - a) / interval);
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			auto tmp = rk.calc(dy_4, b, a, ya, interval);
			dont_opt_away(tmp);
		}
	}
	return 0;
}

int RKF45_example()
{
	auto rk = RK();

	//eg 1
	int interval = 14;
	double b = 1.4, a = 0.;
	double ya = 0.;
	auto dy_1 = [](double x, double y) -> double {return 1 + pow(y, 2); };
	auto y_1 = [](double x) -> double {return tan(x); };
	rk.setType(RK::Type::RK4);
	std::vector<Point2D<double>> res_rk4 = rk.calc(dy_1, b, a, ya, interval);
	printf("y'=1+y^2;y0=0, RK4 result:\n");
	printf("X\tRK4\t\tANALYTIC\tDIFF\n");
	for (auto& p : res_rk4) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", p.x, p.y, y_1(p.x), abs(p.y - y_1(p.x)));
	}

	interval = 7;
	rk.setType(RK::Type::RKF45);
	RK::RKF45Option opt{ 2e-5, 64., 1. / 64.,  200, };
	rk.setRKF45Opt(opt);
	std::vector<Point2D<double>> res_rkf45 = rk.calc(dy_1, b, a, ya, interval);
	printf("y'=1+y^2;y0=0, RKF45 result:\n");
	printf("X\tRKF45\t\tANALYTIC\tDIFF\n");
	for (auto& p : res_rkf45) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", p.x, p.y, y_1(p.x), abs(p.y - y_1(p.x)));
	}

	//eg 2
	interval = 10;
	b = 5., a = 0.;
	ya = 1.;
	auto dy_2 = [](double x, double y) -> double {return exp(-x) - y; };
	auto y_2 = [](double x) -> double {return x * exp(-x) + exp(-x); };
	rk.setType(RK::Type::RK4);
	res_rk4 = rk.calc(dy_2, b, a, ya, interval);
	printf("y'=e^(-x)-y;y0=1, RK4 result:\n");
	printf("X\tRK4\t\tANALYTIC\tDIFF\n");
	for (auto& p : res_rk4) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", p.x, p.y, y_2(p.x), abs(p.y - y_2(p.x)));
	}

	interval = 5;
	rk.setType(RK::Type::RKF45);
	opt = { 2e-5, 64., 1. / 64.,  200, };
	rk.setRKF45Opt(opt);
	res_rkf45 = rk.calc(dy_2, b, a, ya, interval);
	printf("y'=e^(-x)-y;y0=1, RKF45 result:\n");
	printf("X\tRKF45\t\tANALYTIC\tDIFF\n");
	for (auto& p : res_rkf45) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", p.x, p.y, y_2(p.x), abs(p.y - y_2(p.x)));
	}

	//eg 3
	interval = 20;
	b = 0.95, a = 0.;
	ya = 1.;
	auto dy_3 = [](double x, double y) -> double {return 2. * x * y * y; };
	auto y_3 = [](double x) -> double {return 1. / (1. - x * x); };
	rk.setType(RK::Type::RK4);
	res_rk4 = rk.calc(dy_3, b, a, ya, interval);
	printf("y'=2xy^2;y0=1, RK4 result:\n");
	printf("X\tRK4\t\tANALYTIC\tDIFF\n");
	for (auto& p : res_rk4) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", p.x, p.y, y_3(p.x), abs(p.y - y_3(p.x)));
	}

	printf("RK4 method: y'=2xy^2;y0=1;average time");
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			auto tmp = rk.calc(dy_3, b, a, ya, interval);
			dont_opt_away(tmp);
		}
	}

	interval = 10;
	rk.setType(RK::Type::RKF45);
	opt = { 1e-5, 4., 1. / 64.,  200};
	rk.setRKF45Opt(opt);
	res_rkf45 = rk.calc(dy_3, b, a, ya, interval);
	printf("y'=2xy^2;y0=1, RKF45 result:\n");
	printf("X\tRKF45\t\tANALYTIC\tDIFF\n");
	for (auto& p : res_rkf45) {
		printf("%.3f\t%.6f\t%.6f\t%.6f\n", p.x, p.y, y_3(p.x), abs(p.y - y_3(p.x)));
	}

	printf("RKF45 method: y'=2xy^2;y0=1;average time");
	{
		Timer t("");
		for (int i = 0; i < loop_time; i++) {
			auto tmp = rk.calc(dy_3, b, a, ya, interval);
			dont_opt_away(tmp);
		}
	}

	return 0;
}