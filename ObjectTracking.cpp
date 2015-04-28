#include "ObjectTracking.hpp"

//Diese Funktion erstellt 3 Fenster und 8 Trackbars
void ObjectTracking::setWindowSettings(){

	namedWindow("Kalibrierung",1);
	namedWindow("Farbe1",1);
	//namedWindow("Trackfenster",1);
	namedWindow("Farbe2",1);

	moveWindow("Farbe1",800,50);
	moveWindow("Farbe2",800,50);
	moveWindow("Kalibrierung",50,50);

	cvCreateTrackbar("LowerH", "Farbe1", &lowerH, 180, NULL);
	cvCreateTrackbar("UpperH", "Farbe1", &upperH, 180, NULL);

	cvCreateTrackbar("LowerS", "Farbe1", &lowerS, 256, NULL);
	cvCreateTrackbar("UpperS", "Farbe1", &upperS, 256, NULL);

	cvCreateTrackbar("LowerV", "Farbe1", &lowerV, 256, NULL);
	cvCreateTrackbar("UpperV", "Farbe1", &upperV, 256, NULL);

	cvCreateTrackbar("LowerH2", "Farbe2", &lowerH2, 180, NULL);
	cvCreateTrackbar("UpperH2", "Farbe2", &upperH2, 180, NULL);

	cvCreateTrackbar("LowerS2", "Farbe2", &lowerS2, 256, NULL);
	cvCreateTrackbar("UpperS2", "Farbe2", &upperS2, 256, NULL);

	cvCreateTrackbar("LowerV2", "Farbe2", &lowerV2, 256, NULL);
	cvCreateTrackbar("UpperV2", "Farbe2", &upperV2, 256, NULL); 

	cvCreateTrackbar("rectangleSizeH", "Kalibrierung", &rHeight, 100, NULL);  
	cvCreateTrackbar("rectangleSizeW", "Kalibrierung", &rWidth, 100, NULL);

	setTrackbarPos("rectangleSizeH", "Kalibrierung", 50);
	setTrackbarPos("rectangleSizeW", "Kalibrierung", 50);
}

bool ObjectTracking::init(int camNum){
	farbe = false;
	kalibrierung = true;

	hue2 = 0;
	lowerH = 0;
	lowerS = 0;
	lowerV = 0;
	upperH = 180;
	upperS = 256;
	upperV = 256;
	lowerH2 = 0;
	lowerS2 = 0;
	lowerV2 = 0;
	upperH2 = 180;
	upperS2 = 256;
	upperV2 = 256;
	Gegenstand = 1;

	cap.open(0); // open the default camera
	if(!cap.isOpened())  // check if we succeeded
		return false;	


	Mat testframe; //testframe für die Auflösung der Kamera
	cap >> testframe;

	swmaske.create(testframe.rows,testframe.cols,CV_8UC1); 
	swmaske2.create(testframe.rows,testframe.cols,CV_8UC1);// erstellen der Schwarz-Weiß-Maske    
	tracking.create(testframe.rows,testframe.cols,CV_8UC3);// erstellen des Bilds in dem Der Trackpfad aufgezeichnet wird

	return true;
}

void ObjectTracking::update()
{
	//überprüfen ob eine Gegenstandsfarbe kalibriert wurde, wenn nicht voreingestellte Werte nehmen
	if(!farbe)
	{

		hueLower = 180;
		hue2 = 0;
		saturationLower = 255;
		saturationUpper = 0;
		valueLower = 255;
		valueUpper = 0;

	}
	Mat frame;	//Kamerabild
	Mat frameHSV;//Kamerbild in HSV Farben


	cap >> frame; // neues frame von der Kamera abrufen
	GaussianBlur(frame,frame,Size(7,7),3,3); // Bild mit Gaussian Blur Filter glätten
	cvtColor(frame, frameHSV,  CV_BGR2HSV); //Kameraframe von BGR Farben in HSV Farben umwandeln


	//Eckpunkte des Kalibrierungsfeldes setzen
	Point upperleft((frame.cols/2)-rHeight,(frame.rows/2)-rWidth);
	Point downright((frame.cols/2)+rHeight,(frame.rows/2)+rWidth);

	//solange keine Farbe kalibriert wurde oder wenn "j" für die kalibrierung gedrückt wurde wird ein Rotes Rechteck angezeigt


	//Abfragen der "j" Taste für das Kalibrierungfeld
	v = cvWaitKey(1);

	if((char)v==106 ){

		kalibrierung = true;

	}

	if(!farbe || kalibrierung)
	{

		rectangle(frame,upperleft,downright,Scalar(0,0,255),2);

	}
	//Wenn "k" gedrückt wird, dann werden die Farben aus dem roten Rechteck zur Erkennung des Gegenstandes gesetzt

	c = cvWaitKey(1);

	if((char)c==107 && kalibrierung == true)
	{ 
		cout << Gegenstand << endl;
		hueLower = 180;
		hue2 = 0;
		saturationLower = 255;
		saturationUpper = 0;
		valueLower = 255;
		valueUpper = 0;
		farbe = true;
		kalibrierung = false;

		//verschachtelte for-Schleife läuft alle Pixel im roten Rechteck durch und ermittel die Höchsten und Niedrigsten Werte im Feld um die Schwarz-Weiß Maske zu setzen
		for(int y = upperleft.y+1; y < downright.y; y++)
		{

			for(int x = upperleft.x+1; x < downright.x; x++)
			{				

				if(hueLower > frameHSV.at<Vec3b>(y,x)[0])
				{
					if(Gegenstand == 1){
						hueLower = frameHSV.at<Vec3b>(y,x)[0];
						setTrackbarPos("LowerH", "Farbe1", hueLower);
					}
					if(Gegenstand == 2){
						hueLower = frameHSV.at<Vec3b>(y,x)[0];
						setTrackbarPos("LowerH2", "Farbe2", hueLower);
					}
				}

				if(hue2 < frameHSV.at<Vec3b>(y,x)[0])
				{	
					if(Gegenstand == 1){
						hue2 = frameHSV.at<Vec3b>(y,x)[0];
						setTrackbarPos("UpperH", "Farbe1", hue2);
					}
					if(Gegenstand == 2){
						hue2 = frameHSV.at<Vec3b>(y,x)[0];
						setTrackbarPos("UpperH2", "Farbe2", hue2);
					}

				}

				if(saturationLower > frameHSV.at<Vec3b>(y,x)[1])
				{	
					if(Gegenstand == 1){
						saturationLower = frameHSV.at<Vec3b>(y,x)[1];
						setTrackbarPos("LowerS", "Farbe1", saturationLower);
					}
					if(Gegenstand == 2){
						saturationLower = frameHSV.at<Vec3b>(y,x)[1];
						setTrackbarPos("LowerS2", "Farbe2", saturationLower);
					}
				}

				if(saturationUpper < frameHSV.at<Vec3b>(y,x)[1])
				{	
					if(Gegenstand == 1){
						saturationUpper = frameHSV.at<Vec3b>(y,x)[1];
						setTrackbarPos("UpperS", "Farbe1", saturationUpper);
					}
					if(Gegenstand == 2){
						saturationUpper = frameHSV.at<Vec3b>(y,x)[1];
						setTrackbarPos("UpperS2", "Farbe2", saturationUpper);
					}
				}

				if(valueLower > frameHSV.at<Vec3b>(y,x)[2])
				{	
					if(Gegenstand == 1){
						valueLower = frameHSV.at<Vec3b>(y,x)[2];
						setTrackbarPos("LowerV", "Farbe1", valueLower);
					}
					if(Gegenstand == 2){
						valueLower = frameHSV.at<Vec3b>(y,x)[2];
						setTrackbarPos("LowerV2", "Farbe2", valueLower);
					}
				}

				if(valueUpper < frameHSV.at<Vec3b>(y,x)[2])
				{	
					if(Gegenstand == 1){
						valueUpper = frameHSV.at<Vec3b>(y,x)[2];
						setTrackbarPos("UpperV", "Farbe1", valueUpper);
					}
					if(Gegenstand == 2){
						valueUpper = frameHSV.at<Vec3b>(y,x)[2];
						setTrackbarPos("UpperV2", "Farbe2", valueUpper);
					}
				}	

				if(Gegenstand == 3){
					Gegenstand = 0;
				}
			}				
		}Gegenstand ++;
	}



	//Erstellen der Schwarz-Weiß-Maske anhand der gesetzten Unter-/Oberwerte
	inRange(frameHSV, Scalar(lowerH,lowerS,lowerV), Scalar(upperH,upperS,upperV), swmaske);


	//Erstellen der Schwar-Weiß-Maske anhand der gesetzten Unter-/Oberwerte
	inRange(frameHSV, Scalar(lowerH2,lowerS2,lowerV2), Scalar(upperH2,upperS2,upperV2), swmaske2);


	//ermitteln des Mittelpunktes der weißen Pixel
	Xzähler = 1;
	Yzähler = 1;
	Xmittel = 0;
	Ymittel = 0;
	Xzähler2 = 1;
	Yzähler2 = 1;
	Xmittel2 = 0;
	Ymittel2 = 0;

	for(int y = 0 ; y < frame.rows ; y++)
	{

		for(int x = 0; x<frame.cols; x++)
		{

			if(swmaske.at<uchar>(y,x) == 255)
			{			 
				Xmittel +=x;
				Xzähler ++;
				Ymittel += y;
				Yzähler ++;
			}
			if(swmaske2.at<uchar>(y,x) == 255)
			{			 
				Xmittel2 +=x;
				Xzähler2 ++;
				Ymittel2 += y;
				Yzähler2 ++;
			}
		}
	}

	// Mittelwert für X und Y ausrechnen
	Xmittel = Xmittel/Xzähler;
	Ymittel = Ymittel/Yzähler;
	Xmittel2 = Xmittel2/Xzähler2;
	Ymittel2 = Ymittel2/Yzähler2;

	//Trackpfad im Bild "tracking" zeichnen
	line(tracking, Point(Xmittelalt,Ymittelalt), Point(Xmittel,Ymittel), Scalar(0,255,255),1);
	circle(tracking, Point(Xmittel,Ymittel) , 1, Scalar(0,0,255),1);
	line(tracking, Point(Xmittelalt2,Ymittelalt2), Point(Xmittel2,Ymittel2), Scalar(255,0,0),1);
	circle(tracking, Point(Xmittel2,Ymittel2) , 1, Scalar(255,0,255),1);

	Xmittelalt = Xmittel;
	Ymittelalt = Ymittel;	
	Xmittelalt2 = Xmittel2;
	Ymittelalt2 = Ymittel2;


	int range = 50;

	//Bilder anzeigen
	imshow("Kalibrierung",frame);
	imshow("Farbe1",swmaske);
	imshow("Farbe2",swmaske2);
	//Trackingfenster nur zum Überprüfen des Trackingpfades
	//imshow("fenster3",tracking);

}

bool ObjectTracking::ended()
{
	//Warte 10mS		
	int c = cvWaitKey(1);
	//Beende die Schleife wenn "ESC" gedrückt wird
	if((char)c==27 ) return true;
	else return false;
}