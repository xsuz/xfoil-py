#include <cmath>

class Vector2d
{
    public:
        double x;
        double y;

        //inline constructors
        Vector2d() : x{0}, y{0}
        {
        }

        Vector2d(double xi, double yi) : x{xi}, y{yi}
        {
        }
        //access
        float xf() const {return float(x);}
        float yf() const {return float(y);}

        //inline operators
        double & coord(int i)
        {
            if      (i==0) return x;
            else if (i==1) return y;
            return x;
        }

        double const& coord(int i) const
        {
            if      (i==0) return x;
            else if (i==1) return y;
            return x;
        }

        bool operator ==(Vector2d const &V)
        {
            //used only to compare point positions
            return (V.x-x)*(V.x-x) + (V.y-y)*(V.y-y)<0.000000001;
        }

        void operator+=(Vector2d const &T)
        {
            x += T.x;
            y += T.y;
        }

        void operator-=(Vector2d const &T)
        {
            x -= T.x;
            y -= T.y;
        }

        void operator*=(double const &d)
        {
            x *= d;
            y *= d;
        }

        Vector2d operator *(double const &d) const
        {
            Vector2d T(x*d, y*d);
            return T;
        }


        Vector2d operator /(double const &d) const
        {
            Vector2d T(x/d, y/d);
            return T;
        }

        Vector2d operator +(Vector2d const &V) const
        {
            Vector2d T(x+V.x, y+V.y);
            return T;
        }


        Vector2d operator -(Vector2d const &V) const
        {
            Vector2d T(x-V.x, y-V.y);
            return T;
        }


        //inline methods
        void copy(Vector2d const &V)
        {
            x = V.x;
            y = V.y;
        }

        void set(double const &x0, double const &y0)
        {
            x = x0;
            y = y0;
        }

        void set(Vector2d const &V)
        {
            x = V.x;
            y = V.y;
        }

        void normalize()
        {
            double abs = norm();
            if(abs< 1.e-10) return;
            x/=abs;
            y/=abs;
        }

        Vector2d normalized() const
        {
            double l = norm();
            if(fabs(l)<0.000000001) return Vector2d(0.0,0.0);
            else return Vector2d(x/l, y/l);
        }

        Vector2d toUnit()
        {
            Vector2d unitVector;
            double abs = norm();
            if(abs> 1.e-10)
            {
                unitVector.x/=abs;
                unitVector.y/=abs;
            }
            else
            {
                unitVector.set(x,y);
                unitVector.normalize();
            }
            return unitVector;
        }


        double norm() const
        {
            return sqrt(x*x+y*y);
        }

        double dot(Vector2d const &V) const
        {
            return x*V.x + y*V.y ;
        }

        bool isSame(Vector2d const &V, double precision=1.e-9) const
        {
            //used only to compare point positions
            return sqrt((V.x-x)*(V.x-x) + (V.y-y)*(V.y-y)) < precision;
        }

        void translate(Vector2d const &T)
        {
            x += T.x;
            y += T.y;
        }

        void translate(const double &tx, const double &ty)
        {
            x += tx;
            y += ty;
        }


        Vector2d translated(const double &tx, const double &ty) const
        {
            return Vector2d(x+tx, y+ty);
        }

        double angle(Vector2d const &V) const{
		double dot = V.x*x + V.y*y; // proportional to cosine
		double det = V.x*y - V.y*x; // proportional to sine
		return atan2(det, dot);
	}

        double distanceTo(Vector2d pt) const {return sqrt((pt.x-x)*(pt.x-x) + (pt.y-y)*(pt.y-y));}

        //other methods
	void rotateZ(Vector2d const &O, double beta){
		//Rotate the vector around the Z-axis, by an angle ZTilt
		Vector2d OP;
		OP.x = x-O.x;
		OP.y = y-O.y;

		x = O.x + OP.x * cos(beta) - OP.y * sin(beta);
		y = O.y + OP.x * sin(beta) + OP.y * cos(beta);
	}
	void rotateZ(double beta){
		double xo = x,yo=y;
		x =  xo * cos(beta) - yo * sin(beta);
		y =  xo * sin(beta) + yo * cos(beta);
	}

};
