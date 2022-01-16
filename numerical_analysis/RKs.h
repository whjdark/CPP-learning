#pragma once

#include "ODEs.h"

class RKs : public ODEs_base
{
public:
	RKs() {}
	~RKs() {}

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
	// void setRKF45Opt(const RKF45Option opt);
	std::vector<std::vector<Point2D<double>>> calc(const std::vector<std::function<double(double, double)>>& f,
																const double& b, const double& a,
																const std::vector<double>& ya,
																const size_t& M) const;

private:
	/*std::vector<Point2D> midPoint(const std::function<double(double, double)>& f,
		const double& b, const double& a,
		const double& ya, const size_t& M) const;
	std::vector<Point2D> Heun(const std::function<double(double, double)>& f,
		const double& b, const double& a,
		const double& ya, const size_t& M) const;*/
	std::vector<std::vector<Point2D<double>>> rks4(const std::vector<std::function<double(double, double)>>& f,
																const double& b, const double& a,
																const std::vector<double>& ya,
																const size_t& M) const;
	/*std::vector<Point2D> rkf45(const std::function<double(double, double)>& f,
		const double& b, const double& a,
		const double& ya, const size_t& M) const;*/

private:
	Type m_type = Type::RK4;
	RKF45Option m_opt{ 2e-5, 64., 1. / 64., 200 };
};