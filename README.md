# opencv

Program OpenCVPingPoint dla projektu "PingPoint"

TODO/Notatki:

Pamiętaj o zmiennej Licz Punkty

1. Lepsza kalibracja piłki (auto HSV i inna magia)
2. Przycisk do rozpoczynania zliczania (oddzielne okno)
3. Liczenie odbić, a zarazem liczenie punktów i wysyłanie sygnałów
4. wygląd, optymalizacja
5. sprawdzić aplikację na innym środowisku (laptop), opisać co trzeba zrobić!
Visual C++ 2015 lub 2017 redistributable / Universal C Runtime / OpenCV dll

Wspomnieć o inny programie do ustawiania parametrów kamery

wytyczne dotyczące kalibracji: 
- stół z brzegiem 
- piłka na stole leżąca 

FullHD 50FPS (za wolno, trzeba niższą rozdzielczość)

przestarzałe info, może się jednak przydac:

Rysowanie okręgu piłeczki wraz z istniejącą linią:

http://layer0.authentise.com/detecting-circular-shapes-using-contours.html

https://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html (minEnclosingCircle, approxPolyDP - przetestowac te funkcje)

Czy istniejacy obraz HSV (ktory musi byc idealnie okrągły) będzie w tym pomocny?

