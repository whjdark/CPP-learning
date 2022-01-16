#include "lagrange_inter.h"

auto LagrangeInterp::calc(const double& target_x) const -> double 
{
	size_t num_pts = m_pts.size();
	if (num_pts <= 1) { throw "Invalid number of points, should be > 1."; }
	/*std::string msg;
	switch (num_pts) {
	case 0:
		msg = "empty input point array: ";
		break;
	case 1:
		msg = "invalid parameter: ";
		break;
	case 2:
		msg = "linear interpolation: ";
		break;
	case 3:
		msg = "parabolic interpolation: ";
		break;
	default:
		msg = "polynominal order: ";
		break;
	}
	msg = msg + std::string("length of points == ") + std::to_string(num_pts) + "\n";
	printf(msg.c_str());*/

	double result = 0.0;
	for (size_t i = 0; i < num_pts; i++) {
		double l = 1.0L;
		for (size_t j = 0; j < num_pts; j++) {
			if (i != j) {
				l *= (target_x - m_pts[j].x) / (m_pts[i].x - m_pts[j].x);
			}
		}
		result += l * m_pts[i].y;
	}

	return result;
}