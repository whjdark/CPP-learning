#include "newton_inter.h"

NewtonInterp::NewtonInterp(const std::vector<Point2D<double>>& _pts)
	: inter_base(_pts) 
{
	constructDiffTable(_pts);
}

NewtonInterp::~NewtonInterp() 
{ 
	if (m_diffTable) { 
		delete[] m_diffTable; 
	} 
}

auto NewtonInterp::calc(const double& target_x) const ->double 
{
	if (m_pts.size() <= 1) { throw "Invalid number of points, should be > 1."; }
	double result = m_pts[0].y;
	for (size_t i = 0; i < m_tableSize; i++) {	//1st order difference quotient
		double tmp = 1.0;
		for (size_t j = 0; j <= i; j++) {
			tmp *= target_x - m_pts[j].x;
		}
		result += tmp * m_diffTable[i * m_tableSize + i];
	}
	return result;
}

void NewtonInterp::setPoints(const std::vector<Point2D<double>>& _pts)
{
	m_pts = _pts;
	constructDiffTable(_pts);
}

void NewtonInterp::constructDiffTable(const std::vector<Point2D<double>>& _pts)
{
	size_t num_pts = _pts.size();
	m_tableSize = num_pts - 1;
	if (m_diffTable) { 
		delete[] m_diffTable; 
	}
	m_diffTable = new double[m_tableSize * m_tableSize];

	// difference quotient table
	for (size_t i = 0; i < m_tableSize; i++) {	//1st order difference quotient
		m_diffTable[i] = (_pts[i + 1].y - _pts[i].y) / (_pts[i + 1].x - _pts[i].x);
	}
	for (size_t i = 1; i < m_tableSize; i++) { // i-th order difference quotient
		size_t cur_col = i * m_tableSize;
		size_t last_col= cur_col - m_tableSize;
		for (size_t j = i; j < m_tableSize; j++) {
			double numer = m_diffTable[last_col + j] - m_diffTable[last_col + j - 1];
			double denom = _pts[j + 1].x - _pts[j - i].x;
			m_diffTable[cur_col + j] = numer / denom;
		}
	}

	//for (int i = 0; i < table_size; i++) {
	//	for (int j = 0; j < table_size; j++) {
	//		int cur_order = i * table_size;
	//		printf("%f ", diff_table[cur_order + j]);
	//	}
	//	printf("\n ");
	//}
}
