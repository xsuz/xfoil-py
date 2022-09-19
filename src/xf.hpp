#ifndef LIB_XF_H
#define LIB_XF_H

#include <tuple>
#include "core/xfoil.hpp"

using namespace std;

class XF{
	public:
		XF();
		int LOAD(string filename);
		//void TYPE(int mode);
		void setRe(double val){Re=val;}
		double getRe(){return Re;}
		void setMach(double val){Mach=val;}
		double getMach(){return Mach;}
		void ITER(int n);
		tuple<double,double,double,double> ALFA(double alpha);
		//tuple<py::array_t<double>,py::array_t<double>,py::array_t<double>,py::array_t<double>> ASEQ(double start,double end,double step);
		tuple<double,double,double,double> CL(double cl);
		//tuple<py::array_t<double>,py::array_t<double>,py::array_t<double>,py::array_t<double>> CSEQ(double start,double end,double step);
	private:
		bool iterate();
		double Re=500000;
		double Mach=0.0;
		XFoil* xf;
		// m_InitBL:境界層の影響を評価するかどうか
		bool m_bInitBL=false;
		// m_InitBL:境界層の影響を評価するかどうか
		bool m_Viscous=true;
};

#endif
