/* Trzy metody kalibracji stolu:

1) goodFeaturesToTrack: https://docs.opencv.org/3.0-beta/modules/imgproc/doc/feature_detection.html#goodfeaturestotrack
2) Canny: https://docs.opencv.org/3.4.0/df/d0d/tutorial_find_contours.html
3) minAreaRect z tej strony: https://stackoverflow.com/questions/26583649/opencv-c-rectangle-detection-which-has-irregular-side - obecnie zaimplementowana w funkcji "kalibracja"

//// 1) goodFeaturesToTrack (start) ////

vector<Point2f> krawedzie;

goodFeaturesToTrack(klatka_szara, krawedzie, 4, 0.01, 10, Mat(), 3, 3, false, 0.04); //wykrywanie krawedzi

//rysowanie prostokata na podstawie wykrytych krawedzi
line(klatka, krawedzie[0], krawedzie[1], Scalar(255, 0, 0));
line(klatka, krawedzie[1], krawedzie[2], Scalar(0, 255, 0));
line(klatka, krawedzie[2], krawedzie[3], Scalar(0, 0, 255));
line(klatka, krawedzie[3], krawedzie[0], Scalar(255, 255, 255));

//// 1) goodFeaturesToTrack (end) ////

//// 2) Canny (start) ////

Mat canny_output;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

Canny(klatka_szara, canny_output, 100, 100 * 2, 3); //100 w obu miejscach to czulosc, max to 255

findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

for (size_t i = 0; i < contours.size(); i++) drawContours(klatka, contours, (int)i, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());

//// 2) Canny (end) ////

*/

#include "opencv2/highgui.hpp" //wyświetlanie okna
#include "opencv2/imgproc.hpp" //operacje na obrazie
#include <iostream>

#define ID_KAMERY 1 //0 - pierwsza kamera, 1 - druga kamera itd. - zmienic w razie potrzeby

using namespace cv;
using namespace std;

//// Zmienne globalne (start) ////

Mat video; //tu przechowujemy surowe klatki z kamery

Mat linie; //tu przechowujemy rysowane linie

Mat klatka; //docelowa klatka, na ktorej rysujemy docelowy prostokat poprzez kalibracje

Mat3b okno_kalibracji; //obiekt okna kalibracji

Rect przycisk_kalibracji; //prostokat reprezentujacy przycisk kalibracji

string nazwa_okna_kalibracji = "Kalibracja stolu";

//// Zmienne globalne (end) ////

void kalibracja(int event, int x, int y, int flags, void* userdata) //funkcja kalibracji
{
	klatka = video.clone(); //docelowa klatka, na ktorej rysujemy docelowy prostokat poprzez kalibracje metoda ponizej

	Mat klatka_szara; //szara klatka do wykrywania prostokatu, wykorzystywana we wszystkich metodach

	if (event == EVENT_LBUTTONDOWN && przycisk_kalibracji.contains(Point(x, y))) //wykrywamy klikniecie myszki w prostokat reprezentujacy przycisk
	{
		cvtColor(video, klatka_szara, CV_BGR2GRAY); //utworzenie szarej klatki

		//// KOD KALIBRACJI (start) (TODO: WYBRAC NAJLEPSZA METODE!!!) ////

		Mat maska;
		threshold(klatka_szara, maska, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU); //czulosc

		vector<vector<Point>> kontury;
		vector<Vec4i> hierarchia;
		findContours(maska, kontury, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //tworzymy kontury

		int biggestContourIdx = -1;
		double biggestContourArea = 0;

		for (int i = 0; i < kontury.size(); i++)
		{
			drawContours(klatka, kontury, i, Scalar(0, 100, 0), 1, 8, hierarchia, 0, Point()); //zielone, pomocnicze linie, finalnie do usuniecia

			double ctArea = contourArea(kontury[i]);
			if (ctArea > biggestContourArea)
			{
				biggestContourArea = ctArea;
				biggestContourIdx = i;
			}
		}

		RotatedRect boundingBox = minAreaRect(kontury[biggestContourIdx]);

		Point2f krawedzie[4];
		boundingBox.points(krawedzie);

		line(klatka, krawedzie[0], krawedzie[1], Scalar(255, 255, 255));
		line(klatka, krawedzie[1], krawedzie[2], Scalar(255, 255, 255));
		line(klatka, krawedzie[2], krawedzie[3], Scalar(255, 255, 255));
		line(klatka, krawedzie[3], krawedzie[0], Scalar(255, 255, 255));

		//// KOD KALIBRACJI (end) ////

		klatka.copyTo(okno_kalibracji(Rect(0, przycisk_kalibracji.height, klatka.cols, klatka.rows))); //rysowanie klatki z prostokatem w oknie
	}

	imshow(nazwa_okna_kalibracji, okno_kalibracji);
	waitKey(1);
}

int WinMain(int argc, char** argv)
{
	//// Zmienne inicjalizowane raz na poczatku (start) ////

	int LowH = 0;
	int HighH = 179;

	int LowS = 0;
	int HighS = 255;

	int LowV = 0;
	int HighV = 255;

	double ostatnieX = -1;
	double ostatnieY = -1;

	int licznik = 0; //licznik resetowania rysowanie linii

	Mat videoHSV; //obraz w postaci HSV

	Mat videoProgowe; //czarno-bialy obraz sluzacy do kalibracji wykrywania linii

	//// Zmienne inicjalizowane raz na poczatku (end) ////

	//// Obsluga kamery (start) ////

	VideoCapture capture; //klasa przechwytywania video

	capture.open(ID_KAMERY); //otwieramy kamere

	if (!capture.isOpened()) // sprawdzamy czy podłączono kamerę w celu uniknięcia BSOD
	{
		cout << "Podlacz choc jedna kamere do komputera!" << endl << endl;
		return -1;
	}

	//// Obsluga kamery (end) ////

	//// Okno kalibracji (start) ////

	capture.read(video); //przechwytywanie tymczasowej, pierwszej klatki bez prostokata, w celu utworzenia okna kalibracji

	namedWindow(nazwa_okna_kalibracji); //tworzenie okna kalibracji

	przycisk_kalibracji = Rect(0, 0, video.cols, 25); //tworzenie przycisku kalibracji

	okno_kalibracji = Mat3b(video.rows + przycisk_kalibracji.height, video.cols, Vec3b(0, 0, 0)); //tworzenie okna kalibracji

	okno_kalibracji(przycisk_kalibracji) = Vec3b(200, 200, 200); //rysowanie przycisku kalibracji w oknie

	video.copyTo(okno_kalibracji(Rect(0, przycisk_kalibracji.height, video.cols, video.rows))); //rysowanie tymczasowej klatki w oknie

	setMouseCallback(nazwa_okna_kalibracji, kalibracja); // aktywacja funkcji "kalibracja"

	imshow(nazwa_okna_kalibracji, okno_kalibracji); // pokazywanie okna kalibracji

	//// Okno kalibracji (end) ////

	//// Okno sterowania wartościami HSV (Hue, Saturation, Value) (start) ////

	namedWindow("Kalibracja pilki"); //tworzenie okna sterowania

	createTrackbar("Low H", "Kalibracja pilki", &LowH, 179);
	createTrackbar("High H", "Kalibracja pilki", &HighH, 179);

	createTrackbar("Low S", "Kalibracja pilki", &LowS, 255);
	createTrackbar("High S", "Kalibracja pilki", &HighS, 255);

	createTrackbar("Low V", "Kalibracja pilki", &LowV, 255);
	createTrackbar("High V", "Kalibracja pilki", &HighV, 255);

	//// Okno sterowania wartościami HSV (Hue, Saturation, Value) (end) ////

	//// Wykrywanie obiektu i rysownie linii (start) ////

	while (1)
	{
		capture.read(video); //ciagle przechwytywanie nowej klatki kamery do wyswietlania linii

		//resetujemy linie co jakiś czas (trzeba to zoptymalizować)
		licznik++;
		if (licznik = 6000000) 
		{
			licznik = 0;
			linie = Mat::zeros(video.size(), CV_8UC3); //tworzenie pustego obrazu o rozmiarze kamery w postaci RGB
		}

		cvtColor(video, videoHSV, COLOR_BGR2HSV); //konwersja obrazu do postaci HSV (Hue, Saturation, Value)

		inRange(videoHSV, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), videoProgowe); //Tworzymy obraz na postawie aktualnych wartości suwaków

		//operacje morfologiczne (usuwamy małe obiekty i dziury)
		erode(videoProgowe, videoProgowe, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(videoProgowe, videoProgowe, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		dilate(videoProgowe, videoProgowe, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(videoProgowe, videoProgowe, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		Moments momenty = moments(videoProgowe); //https://docs.opencv.org/3.4.0/d8/d23/classcv_1_1Moments.html

		double m01 = momenty.m01;
		double m10 = momenty.m10;
		double mPowierzchnia = momenty.m00;

		if (mPowierzchnia > 10000)
		{
			//obliczanie pozycji obiektu
			double X = m10 / mPowierzchnia;
			double Y = m01 / mPowierzchnia;

			if (ostatnieX >= 0 && ostatnieY >= 0 && X >= 0 && Y >= 0) line(linie, Point((int)X, (int)Y), Point((int)ostatnieX, (int)ostatnieY), Scalar(0, 0, 255), 1); //rysowanie linii

			ostatnieX = X;
			ostatnieY = Y;
		}

		//// Wykrywanie obiektu i rysownie linii (end) ////

		imshow("Obraz HSV", videoProgowe); //pokazywanie obrazu HSV

		imshow("Linie", video + linie); //pokazywanie kamery z liniami

		if (waitKey(1) == 27) break; //czekamy na klawisz 'esc'
	}

	return 0;
}
