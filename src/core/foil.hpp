#pragma once

#include <string>
#include <pybind11/stl.h>
#include "vec2.hpp"

#include "xfoil_params.h"

namespace {
#define MIDPOINTCOUNT 1000
	using namespace std;
}
class Foil{
	public:
		int Load_from_file(string filename);
		int Load(vector<double> _x,vector<double> _y);
		const tuple<vector<double>,vector<double>> getCamberLine() const{return tuple<vector<double>,vector<double>>(m_cambLine_x,m_cambLine_x);}
		double camber() const { return m_fCamber;}
		double xCamber() const {return m_fXCamber;}
		double thickness() const {return m_fThickness;}
		double xThickness() const{return m_fXThickness;}
		vector<double> get_x() const{return x;}
		vector<double> get_y() const{return y;}
	private:
		void compMidLine();
		int _Load(vector<Vector2d> coords);
		void getUpperY(double _x, double &_y)const;
		void getLowerY(double _x, double &_y)const;
		string name;
		int m_nLower;                          /**< the number of points on the lower surface of the current foil */
		int m_nUpper;                          /**< the number of points on the upper surface of the current foil */
		vector<double> m_cambLine_x=vector<double>(MIDPOINTCOUNT);/**< the mid camber line points */
		vector<double> m_cambLine_y=vector<double>(MIDPOINTCOUNT);/**< the mid camber line points */
		Vector2d m_upper[IQX];           /**< the upper surface points */
		Vector2d m_lower[IQX];           /**< the lower surface points */
		vector<double> x;
		vector<double> y;

		double m_fCamber;                    /**< the Foil's max camber */
		double m_fThickness;                 /**< the Foil's max thickness */
		double m_fXCamber;                   /**< the x-position of the Foil's max camber point */
		double m_fXThickness;                /**< the x-position of the Foil's max thickness point */
		size_t m_n;
};
