#include "xf.hpp"
#include <iostream>
#include <fstream>

XF::XF(){
	xf = new XFoil();
}
namespace{
	stringstream ss;
	int s_IterLim=100,m_Iterations=0L;
}
int XF::LOAD(string filename){
	std::ifstream fs(filename);
	if (!fs) {
		std::cout << "Failed to open dat file" << std::endl;
		return -1;
	}

	std::string line;
	std::getline(fs, line);
	std::cout << "Foil name : " << line << std::endl;
	int cnt = 0;
	double x[IBX],y[IBX];
	double nx[604],ny[604];
	while (!fs.eof()) {
		std::getline(fs, line);
		line.erase(0, line.find_first_not_of(" \t"));
		int endOfX = line.find_first_of(" \t");
		if (endOfX == -1) continue;
		std::string sx = line.substr(0, endOfX);
		std::string sy = line.substr(endOfX);
		x[cnt] = atof(sx.c_str());
		y[cnt] = atof(sy.c_str());
		cnt++;
	}
	if(!xf->initXFoilGeometry(cnt,x,y,nx,ny)){
		std::cout << "Initialization error!" << std::endl;
		return 1;
	}
	return cnt;
}

bool XF::iterate() {
	if (!xf->viscal()) {
		xf->lvconv = false;
		std::cout
			<< "CpCalc: local speed too large\nCompressibility corrections invalid"
			<< std::endl;
		return false;
	}

	while (m_Iterations < s_IterLim && !xf->lvconv) {
		if (xf->ViscousIter()) {
			m_Iterations++;
		} else
			m_Iterations = s_IterLim;
	}

	if (!xf->ViscalEnd()) {
		xf->lvconv = false;  // point is unconverged

		xf->setBLInitialized(false);
		xf->lipan = false;
		return true;  // to exit loop
	}

	if (m_Iterations >= s_IterLim && !xf->lvconv) {
		if (m_bInitBL) {
			xf->setBLInitialized(false);
			xf->lipan = false;
		}
		return true;
	}
	return xf->lvconv;
}

tuple<double,double,double,double> XF::ALFA(double alpha){
	m_Iterations = 0;
	if (!xf->initXFoilAnalysis(Re, 0, Mach, 9.0, 1.0, 1.0, 1, 1, true, ss)) {
		std::cout << "Initialization error!" << std::endl;
		return tuple<double,double,double,double>(xf->cl,xf->cd,xf->cm,xf->xcp);
	}

	xf->setBLInitialized(false);
	xf->lipan = false;

	xf->setAlpha(alpha * 3.14159 / 180);
	xf->lalfa = true;
	xf->setQInf(1.0);
	std::cout << "alpha : " << alpha << std::endl;

	if(!xf->specal()){
		std::cout << "Invalid Analysis Settings\nCpCalc: local speed too large\n Compressibility corrections invalid ";
		return tuple<double,double,double,double>(xf->cl,xf->cd,xf->cm,xf->xcp);
	}

	xf->lwake = false;
	xf->lvconv = false;

	while (!this->iterate())
		;

	//std::cout << ss.str() << std::endl;

	if (xf->lvconv) {
		std::cout << "  converged after " << m_Iterations << " iterations"
			<< std::endl;
		std::cout << "  cl : " << xf->cl << ", cd : " << xf->cd
			<< ", cm : " << xf->cm << ", xcp : " << xf->xcp
			<< std::endl;
	} else {
		std::cout << "  unconverged" << std::endl;
	}
	return tuple<double,double,double,double>(xf->cl,xf->cd,xf->cm,xf->xcp);
}

tuple<double,double,double,double> XF::CL(double cl){
	m_Iterations = 0;
	if (!xf->initXFoilAnalysis(Re, 0, Mach, 9.0, 1.0, 1.0, 1, 1, true, ss)) {
		std::cout << "Initialization error!" << std::endl;
		return tuple<double,double,double,double>(xf->alfa,xf->cd,xf->cm,xf->xcp);
	}

	xf->setBLInitialized(m_bInitBL);
	xf->lipan = false;

	xf->setClSpec(cl * 3.14159 / 180);
	xf->lalfa = false;
	xf->setQInf(1.0);
	std::cout << "cl : " << cl << std::endl;

	if(!xf->specal()){
		std::cout << "Invalid Analysis Settings\nCpCalc: local speed too large\n Compressibility corrections invalid ";
		return tuple<double,double,double,double>(xf->alfa,xf->cd,xf->cm,xf->xcp);
	}

	xf->lwake = false;
	xf->lvconv = false;

	while (!this->iterate())
		;

	//std::cout << ss.str() << std::endl;

	if (xf->lvconv) {
		std::cout << "  converged after " << m_Iterations << " iterations"
			<< std::endl;
		std::cout << "  alfa : " << xf->alfa << ", cd : " << xf->cd
			<< ", cm : " << xf->cm << ", xcp : " << xf->xcp
			<< std::endl;
	} else {
		std::cout << "  unconverged" << std::endl;
	}
	return tuple<double,double,double,double>(xf->alfa,xf->cd,xf->cm,xf->xcp);
}
