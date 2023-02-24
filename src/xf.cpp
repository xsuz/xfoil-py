#include"xf.hpp"
#include <iostream>
#include <fstream>

namespace{
	stringstream ss;
	int s_IterLim=100,m_Iterations=0;
}

XF::XF(){
	xf=new XFoil();
}

int XF::Load(string filename,bool primary=true){
	std::ifstream fs(filename);
	if (!fs) {
		std::cout << "Failed to open dat file" << std::endl;
		return -1;
	}

	std::string line;
	std::getline(fs, line);
	std::cout << "Foil name : " << line << std::endl;
	int cnt = 0;
	double nx[604],ny[604];
	while (!fs.eof()) {
		std::getline(fs, line);
		line.erase(0, line.find_first_not_of(" \t"));
		int endOfX = line.find_first_of(" \t");
		if (endOfX == -1) continue;
		std::string sx = line.substr(0, endOfX);
		std::string sy = line.substr(endOfX);
		if(primary){
			x1[cnt] = atof(sx.c_str());
			y1[cnt] = atof(sy.c_str());
		}else{
			x2[cnt] = atof(sx.c_str());
			y2[cnt] = atof(sy.c_str());
		}
		cnt++;
	}
	if(primary){
		n1=cnt;
		if(!xf->initXFoilGeometry(n1,x1,y1,nx,ny)){
			std::cout << "Initialization error!" << std::endl;
			return 1;
		}
	}else{
		n2=cnt;
		interpolate(interpolate_rate);
	}
	return cnt;
}

void XF::tegap(double gapnew,double blend){
	xf->tgap(gapnew,blend);
	double x[IBX],y[IBX],nx[IBX],ny[IBX];
	for(int j=0;j<xf->nb;j++){
		x[j]=xf->xb[j+1];
		y[j]=xf->yb[j+1];
	}
	if(!xf->initXFoilGeometry(n1,x,y,nx,ny)){
		std::cout << "Initialization error!" << std::endl;
		return;
	}
}

void XF::iter(int n=100){
	s_IterLim=n;
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

void XF::interpolate(double rate=0.5){
	if(n2==0 || n1==0){
		std::cout << "ERROR:YOU MUST LOAD TWO FOILS BEFORE YOU INTERPOLATE FOILS";
		return;
	}
	interpolate_rate=1-rate;
	xf->interpolate(x1,y1,n1,x2,y2,n2,1-rate);
	double x[IBX],y[IBX],nx[IBX],ny[IBX];
	for(int j=0;j<xf->nb;j++){
		x[j]=xf->xb[j+1];
		y[j]=xf->yb[j+1];
	}
	if(!xf->initXFoilGeometry(xf->nb,x,y,nx,ny)){
		std::cout << "Initialization error!" << std::endl;
		return;
	}
}

map<string,double> XF::calc(double alpha,double Re){
	m_Iterations = 0;
	if (!xf->initXFoilAnalysis(Re, 0, 0.0, 9.0, 1.0, 1.0, 1, 1, true, ss)) {
		std::cout << "Initialization error!" << std::endl;
		return map<string,double>{{"cl",xf->cl},{"cd",xf->cd},{"cm",xf->cm},{"xcp",xf->xcp},{"status",1}};
	}

	xf->setBLInitialized(m_bInitBL);
	xf->lipan = false;

	xf->setAlpha(alpha * 3.14159 / 180);
	xf->lalfa = true;
	xf->setQInf(1.0);
	std::cout << "alpha : " << alpha << std::endl;

	if(!xf->specal()){
		std::cout << "Invalid Analysis Settings\nCpCalc: local speed too large\n Compressibility corrections invalid ";
		return map<string,double>{{"cl",xf->cl},{"cd",xf->cd},{"cm",xf->cm},{"xcp",xf->xcp},{"status",1}};
	}

	xf->lwake = false;
	xf->lvconv = false;

	while (!this->iterate())
		;

	//std::cout << ss.str() << std::endl;

	if (xf->lvconv) {
		std::cout << "  converged after " << m_Iterations << " iterations"
			<< std::endl;
		return map<string,double>{{"cl",xf->cl},{"cd",xf->cd},{"cm",xf->cm},{"xcp",xf->xcp},{"status",0}};
	} else {
		std::cout << "  unconverged" << std::endl;
		return map<string,double>{{"cl",xf->cl},{"cd",xf->cd},{"cm",xf->cm},{"xcp",xf->xcp},{"status",1}};
	}
}

vector<double> XF::getX()const{
	auto vec=vector<double>(xf->nb);
	for(int i = 0;i<xf->nb;i++){
		vec[i]=xf->xb[i+1];
	}
	return vec;
}
vector<double> XF::getY()const{
	auto vec=vector<double>(xf->nb);
	for(int i = 0;i<xf->nb;i++){
		vec[i]=xf->yb[i+1];
	}
	return vec;
}
tuple<vector<double>,vector<double>> XF::cpv(double alpha,double Re){
	m_Iterations = 0;
	if (!xf->initXFoilAnalysis(Re, 0, 0.0, 9.0, 1.0, 1.0, 1, 1, true, ss)) {
		std::cout << "Initialization error!" << std::endl;
		return tuple<vector<double>,vector<double>>();
	}

	xf->setBLInitialized(m_bInitBL);
	xf->lipan = false;

	xf->setAlpha(alpha * 3.14159 / 180);
	xf->lalfa = true;
	xf->setQInf(1.0);
	std::cout << "alpha : " << alpha << std::endl;

	if(!xf->specal()){
		std::cout << "Invalid Analysis Settings\nCpCalc: local speed too large\n Compressibility corrections invalid ";
		return tuple<vector<double>,vector<double>>();
	}

	xf->lwake = false;
	xf->lvconv = false;

	while (!this->iterate())
		;

	//std::cout << ss.str() << std::endl;

	if (xf->lvconv) {
		std::cout << "  converged after " << m_Iterations << " iterations"
			<< std::endl;
		auto cpv=vector<double>(xf->n);
		auto x=vector<double>(xf->n);
		for(int i = 0;i<xf->n;i++){
			cpv[i]=xf->cpv[i+1];
			x[i]=xf->xb[i+1];
		}
		return {x,cpv};
	} else {
		std::cout << "  unconverged" << std::endl;
		return tuple<vector<double>,vector<double>>();
	}
}
int XF::save(string foilname,string filename)const{
	ofstream ss=ofstream(filename);
	if(!ss){
		std::cout << "ERROR: CANNOT OPEN THE FILE";
		return -1;
	}
	ss << foilname;
	for(int i=0;i<xf->nb;i++){
		ss << "\n" << xf->xb[i+1]/xf->xb[0] << "\t" << xf->yb[i+1]/xf->xb[0];
	}
	return 0;
}
