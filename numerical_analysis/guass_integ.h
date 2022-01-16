#pragma once

#include "cotes_integ.h"

class GuassInteg : public integ_base
{
public:
	GuassInteg() {}
	~GuassInteg() {}

	enum class Type {
		Legendre,
		Chebyshev,
	};

	void setType(const Type t);
	void setPoint(const size_t point);
	double calc(const double& b, const double& a, const func& f) const override;

private:
	Type m_type = Type::Legendre;
	size_t m_point = 2;
	//legendre Xk Ak
	static constexpr double l_Xk[4][5] = { {-0.577350, 0.577350},
																{-0.774597, 0., 0.774597},
																{-0.861136, -0.339981, 0.339981, 0.861136},
																{-0.906180, -0.538469, 0., 0.538469, 0.906180},
															 };
	static constexpr double l_Ak[4][5] = { {1.0, 1.0},
																{5./9., 8./9., 5./9.},
																{0.347855, 0.652145, 0.652145, 0.347855},
																{0.236927, 0.478629, 0.568889, 0.478629, 0.236927},
															};
	//chebyshev Xk
	static constexpr double c_Xk[4][5] = { {-0.7071068, 0.7071068},
																{-0.8660254, 0., 0.8660254},
																{-0.9238795, -0.3826834, 0.3826834, 0.9238795},
																{-0.9510565, -0.5877853, 0., 0.5877853, 0.9510565},
															};

};