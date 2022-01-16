#include "RK.h"

void RK::setType(const Type t)
{
	m_type = t;
}

void RK::setRKF45Opt(const RKF45Option opt)
{
	m_opt = opt;
}

std::vector<Point2D<double>> RK::calc(const ODEqu& f,
																const double& b, const double& a, 
																const double& ya, const size_t& M) const
{
	if (b - a <= 0) {
		throw "Invalid interval b-a, b should be greater than a.";
	}
	switch (m_type)
	{
	case Type::MIDPOINT: //modified Euler-cauchy method
		return midPoint(f, b, a, ya, M);
		break;
	case Type::HEUN:
		return Heun(f, b, a, ya, M);
		break;
	case Type::RK4:
		return rk4(f, b, a, ya, M);
		break;
	case Type::RKF45: //Runge-Kunta-Fehlbrg
		return rkf45(f, b, a, ya, M);
		break;
	default:
		return rk4(f, b, a, ya, M);
		break;
	}
}

std::vector<Point2D<double>> RK::midPoint(const ODEqu& f,
																	const double& b, const double& a, 
																	const double& ya, const size_t& M) const
{
	double h = (b - a) / M;
	std::vector<Point2D<double>> pts;
	pts.reserve(M);
	pts.emplace_back(a, ya);
	for (size_t n = 0; n < M; n++) {
		double x_next = a + h * (n + 1);
		double x = pts[n].x + 0.5 * h;
		double y = pts[n].y + 0.5 * h * f(pts[n].x, pts[n].y);
		double y_next = pts[n].y + h * f(x, y);
		pts.emplace_back(x_next, y_next);
	}
	return pts;
}

std::vector<Point2D<double>> RK::Heun(const ODEqu& f,
																const double& b, const double& a, 
																const double& ya, const size_t& M) const
{
	double h = (b - a) / M;
	std::vector<Point2D<double>> pts;
	pts.reserve(M);
	pts.emplace_back(a, ya);
	for (size_t n = 0; n < M; n++) {
		double x_next = a + h * (n + 1);
		double k1 = f(pts[n].x, pts[n].y);
		double x = pts[n].x + (2. / 3.) * h;
		double y = pts[n].y + (2. / 3.) * h * k1;
		double y_next = pts[n].y + 0.25 * h * (k1 + 3 * f(x, y));
		pts.emplace_back(x_next, y_next);
	}
	return pts;
}

std::vector<Point2D<double>> RK::rk4(const ODEqu& f,
															const double& b, const double& a, 
															const double& ya, const size_t& M) const
{
	double h = (b - a) / M;
	std::vector<Point2D<double>> pts;
	pts.reserve(M);
	pts.emplace_back(a, ya);
	for (size_t n = 0; n < M; n++) {
		double x_next = a + h * (n + 1);
		double k1 = f(pts[n].x, pts[n].y);
		double k2 = f(pts[n].x + 0.5 * h, pts[n].y + 0.5 * h * k1);
		double k3 = f(pts[n].x + 0.5 * h, pts[n].y + 0.5 * h * k2);
		double k4 = f(pts[n].x + h, pts[n].y + h * k3);
		double y_next = pts[n].y + (1. / 6.) * h * (k1 + 2 * k2 + 2 * k3 + k4);
		if (isnan(y_next) || isinf(y_next))
			break;
		pts.emplace_back(x_next, y_next);
	}
	return pts;
}

std::vector<Point2D<double>> RK::rkf45(const ODEqu& f,
																const double& b, const double& a, 
																const double& ya, const size_t& M) const
{
	double h = (b - a) / M;
	std::vector<Point2D<double>> pts;
	pts.reserve(m_opt.maxIter);
	pts.emplace_back(a, ya);
	// coefficient to calc the y(n+1)
	constexpr double a2 = 0.25, b2 = 0.25, a3 = 3. / 8.0L, b3 = 3. / 32., c3 = 9.0L / 32., a4 = 12. / 13.,
								b4 = 1932. / 2197., c4 = -7200. / 2197., d4 = 7296. / 2197., a5 = 1.,
								b5 = 439. / 216., c5 = -8., d5 = 3680. / 513., e5 = -845. / 4104., a6 = 0.5,
								b6 = -8. / 27., c6 = 2., d6 = -3544. / 2565., e6 = 1859. / 4104.,
								f6 = -11. / 40., r1 = 1. / 360., r3 = -128. / 4275., r4 = -2197. / 75240., r5 = 0.02,
								r6 = 2. / 55., n1 = 25. / 216., n3 = 1408. / 2565., n4 = 2197. / 4104., n5 = -0.2;
	const double hmin = h * m_opt.minStepMultiple; // the min-max value of step
	const double hmax = h * m_opt.maxStepMultiple;
	size_t n = 0;
	size_t iter = 0;
	const double br = b - 0.00001 * abs(b); //???
	while (pts[n].x < b) {
		if ((pts[n].x + h) > br) // x out of range(a,b)
			h = b - pts[n].x;
		//calc k1 k2 k3 k4 k5 k6
		double k1 = h * f(pts[n].x, pts[n].y);
		double y2 = pts[n].y + b2 * k1;
		if (isinf(y2) || isnan(y2)) // iter terminatetd by nan or infinite
			break;
		double k2 = h * f(pts[n].x + a2 * h, y2);
		double y3 = pts[n].y + b3 * k1 + c3 * k2;
		if (isinf(y3) || isnan(y3)) 
			break;
		double k3 = h * f(pts[n].x + a3 * h, y3);
		double y4 = pts[n].y + b4 * k1 + c4 * k2 + d4 * k3;
		if (isinf(y4) || isnan(y4)) 
			break;
		double k4 = h * f(pts[n].x + a4 * h, y4);
		double y5 = pts[n].y + b5 * k1 + c5 * k2 + d5 * k3 + e5 * k4;
		if (isinf(y5) || isnan(y5)) 
			break;
		double k5 = h * f(pts[n].x + a5 * h, y5);
		double y6 = pts[n].y + b6 * k1 + c6 * k2 + d6 * k3 + e6 * k4 + f6 * k5;
		if (isinf(y6) || isnan(y6)) 
			break;
		double k6 = h * f(pts[n].x + a6 * h, y6);
		// calc error and y(n+1)
		double err = abs(r1 * k1 + r3 * k3 + r4 * k4 + r5 * k5 + r6 * k6);
		double y_next = pts[n].y + n1 * k1 + n3 * k3 + n4 * k4 + n5 * k5;
		if ((err < m_opt.tol) || (h < 2. * hmin)) { // update y(n+1) x(n+1)
			double x_next = (pts[n].x + h) > br ? b : (pts[n].x + h);
			pts.emplace_back(x_next, y_next);
			n++;
		}
		//calc optim step s
		double s = 0.;
		if (abs(err) != 0.0) //error != 0
			s = 0.84 * pow(m_opt.tol * h / err, 0.25);
		//control the step size
		if ((s < 0.75) && (0.5 * h > hmin))
			h *= 0.5;
		else if ((s > 1.5) && (2. * h < hmax))
			h *= 2.;
		// iter terminatetd by maxIter condition or divergence
		if (isinf(pts[n].y) || isnan(pts[n].y) || iter >= m_opt.maxIter)
			break;
		iter++;
	}
	return pts;
}
