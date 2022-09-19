#include "foil.hpp"
#include<fstream>
#include <iostream>

int Foil::Load_from_file(string filename){
	x.clear();
	y.clear();
	std::ifstream fs(filename);
	if (!fs) {
		std::cout << "Failed to open dat file" << std::endl;
		return -1;
	}

	std::string line;
	std::getline(fs, line);
	std::cout << "Foil name : " << line << std::endl;
	int cnt = 0;

	while (!fs.eof()) {
		std::getline(fs, line);

		line.erase(0, line.find_first_not_of(" \t"));
		int endOfX = line.find_first_of(" \t");
		if (endOfX == -1) continue;

		std::string sx = line.substr(0, endOfX);
		std::string sy = line.substr(endOfX);

		x.push_back(atof(sx.c_str()));
		y.push_back(atof(sy.c_str()));

		cnt++;
	}
	Load(x,y);
	return 0;
}

int Foil::Load(vector<double> _x,vector<double> _y){
	x=vector<double>(_x);
	y=vector<double>(_y);
	int cnt=x.size();
	auto vec= new vector<Vector2d>(cnt);
	for(int i=0;i<cnt;i++){
		(*vec)[i].x=x[i];
		(*vec)[i].y=y[i];
	}
	return _Load(*vec);
}

int Foil::_Load(vector<Vector2d> coords){
	m_n=coords.size();
	while(coords[m_nUpper].x >= coords[m_nUpper+1].x)m_nUpper++;
	m_nLower=m_n-m_nUpper;
	for(int i=0;i<m_nUpper;i++){
		m_upper[i]=coords[m_nUpper-1-i];
	}
	for(int i=0;i<m_nLower;i++){
		m_lower[i]=coords[m_nUpper+i];
	}
	compMidLine();
	std::cout << "Thickness 	= " << thickness()*100 <<std::endl;
	std::cout << "Max. Thick.pos. 	= " << xThickness()*100 <<std::endl;
	std::cout << "Max. Camber       = " << camber()*100 <<std::endl;
	std::cout << "Max. Camber pos.  = " << xCamber()*100 <<std::endl;
	std::cout << "Number of Panels 	= " << m_n << std::endl;

	return 0;
}

void Foil::compMidLine()
{
	double xt=0, ytop=0, ybottom=0, step=0, nx=0, ny=0;

	m_fThickness  = 0.0;
	m_fCamber     = 0.0;
	m_fXCamber    = 0.0;
	m_fXThickness = 0.0;

	step = (m_upper[m_nUpper-1].x-m_upper[0].x)/double(MIDPOINTCOUNT-1);

	for (int l=0; l<MIDPOINTCOUNT; l++)
	{
		xt = m_upper[0].x + l*step;
		getUpperY(double(l)*step, ytop);
		getLowerY(double(l)*step, ybottom);

		m_cambLine_x[l] = xt;
		m_cambLine_y[l] = (ytop+ybottom)/2.0;

		if(fabs(ytop-ybottom)>m_fThickness)
		{
			m_fThickness  = fabs(ytop-ybottom);
			m_fXThickness = xt;
		}
		if(fabs(m_cambLine_y[l])>fabs(m_fCamber))
		{
			m_fCamber  = m_cambLine_y[l];
			m_fXCamber = xt;
		}
	}
}

void Foil::getUpperY(double _x, double &_y) const
{
	_x = m_upper[0].x + _x*(m_upper[m_nUpper-1].x-m_upper[0].x);

	if(_x<=m_upper[0].x)
	{
		_y = m_upper[0].y;
		return;
	}

	for (int i=0; i<m_nUpper; i++)
	{
		if (m_upper[i].x <m_upper[i+1].x  &&  m_upper[i].x <= _x && _x<=m_upper[i+1].x )
		{
			_y = (m_upper[i].y     + (m_upper[i+1].y-m_upper[i].y)
					/ (m_upper[i+1].x-m_upper[i].x) * (_x-m_upper[i].x));

			return;
		}
	}

	_y = m_upper[m_nUpper-1].y;

}

void Foil::getLowerY(double _x, double &_y) const
{
	_x = m_lower[0].x + _x*(m_lower[m_nLower-1].x-m_lower[0].x);

	if(_x<=m_lower[0].x)
	{
		_y = m_lower[0].y;
		return;
	}

	for (int i=0; i<m_nLower; i++)
	{
		if (m_lower[i].x <m_lower[i+1].x  &&  m_lower[i].x <= _x && _x<=m_lower[i+1].x )
		{
			_y = (m_lower[i].y + (m_lower[i+1].y-m_lower[i].y) / (m_lower[i+1].x-m_lower[i].x) * (_x-m_lower[i].x));
			return;
		}
	}

	_y = m_lower[m_nLower-1].y;
}
