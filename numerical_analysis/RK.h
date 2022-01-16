#pragma once

#include "ODE.h"

class RK : public ODE_base
{
public:
	RK() {}
	~RK() {}

	enum class Type {
		MIDPOINT,
		HEUN,
		RK4,
		RKF45,
	};

	struct RKF45Option {
		double tol;
		double maxStepMultiple;
		double minStepMultiple;
		size_t maxIter;
	};

	void setType(const Type t);
	void setRKF45Opt(const RKF45Option opt);
	std::vector<Point2D<double>> calc(const ODEqu& f,
															const double& b, const double& a,
															const double& ya, const size_t& M) const override;

private:
	std::vector<Point2D<double>> midPoint(const ODEqu& f,
																	const double& b, const double& a,
																	const double& ya, const size_t& M) const;
	std::vector<Point2D<double>> Heun(const ODEqu& f,
																const double& b, const double& a,
																const double& ya, const size_t& M) const;
	std::vector<Point2D<double>> rk4(const ODEqu& f,
															const double& b, const double& a,
															const double& ya, const size_t& M) const;
	std::vector<Point2D<double>> rkf45(const ODEqu& f,
																const double& b, const double& a,
																const double& ya, const size_t& M) const;

private:
	Type m_type = Type::RK4;
	RKF45Option m_opt{ 2e-5, 64., 1. / 64., 200};
};