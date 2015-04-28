#ifndef OBJECTTRACKING_HPP
#define OBJECTTRACKING_HPP

#include "opencv2/opencv.hpp"
#include "iostream"

using namespace cv;
using namespace std;

class ObjectTracking
{
private:
	VideoCapture cap;

	Mat swmaske;
	Mat swmaske2;
	Mat tracking;

	//Variablen deklarieren
	bool trackbarGesetzt;
	bool farbe;
	bool kalibrierung;

	int hueLower,hmittel,hueUpper,saturationLower,smittel,saturationUpper,vmittel,valueLower,valueUpper,Xmittel,Xmittel2,Ymittel,Ymittel2,Xmittelalt,Xmittelalt2,Ymittelalt,Ymittelalt2,v,c,Xzähler,Xzähler2,Yzähler,Yzähler2;
	int rHeight,rWidth;
	int hue2;
	int lowerH;
	int lowerS;
	int lowerV;
	int upperH;
	int upperS;
	int upperV;
	int lowerH2;
	int lowerS2;
	int lowerV2;
	int upperH2;
	int upperS2;
	int upperV2;
	int Gegenstand;


public:
	void setWindowSettings();
	void update();
	bool init(int camNum);
	bool ended();
	int getYmittel() {return Ymittel;}
	int getYmittel2() {return Ymittel2;}
};


#endif