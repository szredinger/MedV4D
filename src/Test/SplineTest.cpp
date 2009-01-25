#include "iostream"
#include "Imaging/BSpline.h"

using namespace std;

using namespace M4D;
using namespace M4D::Imaging::Geometry;

typedef float CoordType;
typedef BSpline< CoordType, 2 > Curve;
typedef PointSet< CoordType, 2 > 	Points;
typedef Coordinates< CoordType, 2 > Coords;


void
SetLine( const Coords &a, const Coords &b )
{
	//cout <<	"set arrow from " << a[0] << "," << a[1] 
	//	<< " to " << b[0] << "," << b[1] << "\n";
	
	cout << a[0] << " " << a[1]  << "\n";
		
}

void
PrintPoint( const Coords &a )
{
	cout << a[0] << " " << a[1]  << "\n";
		
}

void
PrepareOutput()
{

}

void
CloseOutput()
{
	//cout << "plot x " << "\n";
}

int
main( int argc, char **argv )
{
	Curve curve;
	//Add points
	Coords xstep( 10, 0 );
	Coords ystep( 0, 10 );

	Coords actualPoint( 0, 0 );
	/*CoordType pom = 1;
	for( unsigned i = 0; i < 4; ++i ) {
		curve.AddPoint( actualPoint );	
		actualPoint += xstep;
		curve.AddPoint( actualPoint );	
		actualPoint += pom *ystep;
		pom *= -1;
	}*/
	curve.AddPoint( Coords(10,10) );
	curve.AddPoint( Coords(120,120) );
	//curve.AddPoint( Coords(120,220) );
	curve.AddPoint( Coords(250,10) );
	curve.AddPoint( Coords(210,130) );
	curve.AddPoint( Coords(250,245) );
	curve.AddPoint( Coords(140,180) );
	curve.AddPoint( Coords(10,230) );
	curve.AddPoint( Coords(45,100) );

	curve.SetCyclic();
	//Sample
	curve.Sample( 15 );


	//Output
	/*PrepareOutput();
	for( unsigned i = 0; i < curve.Size(); ++i ) {
		PrintPoint( curve[i] );
	}*/
	PrintPointSet( std::cout, curve );
	cout << "\n\n";
	
	PrintCurve( std::cout, curve );


	cout << "\n\n";
	
	curve.SplitSegment( 2 );
	curve.Sample( 15 );

	PrintPointSet( std::cout, curve );
	cout << "\n\n";
	PrintCurve( std::cout, curve );

	CloseOutput();

	return 0;
}