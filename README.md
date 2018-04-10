# opencv

Program C++ z OpenCV (live obraz, śledzenie piłeczki, zaliczanie punktu)
 dla Visual Studio 2017

TODO:
0. Opisac jak uruchomic projekt i jak go obslugiwac 

1. Ustalenie najlepszej metody do rysowania prostokata reprezentujacego stół (usprawnic wszystkie trzy istniejace metody)

2. Implementacja nachodzenia piłeczki na prostokąt: https://www.google.pl/search?q=opencv+overlapping i ustawiania flagi na to zdarzenie

3. Przesyłanie flagi do aplikacji C#, tworzenie prostokąta zgodnie z aplikacją (przecięcie na pół itp.)

4. Upiększenie interfejsu jak to tylko potrzebne

5. Zmiana projektu z Debug na Release

6. Optymalizacja






przestarzałe info, może się jednak przydac:
Rysowanie okręgu piłeczki wraz z istniejącą linią:

http://layer0.authentise.com/detecting-circular-shapes-using-contours.html

https://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html (minEnclosingCircle, approxPolyDP - przetestowac te funkcje)

Czy istniejacy obraz HSV (ktory musi byc idealnie okrągły) będzie w tym pomocny?

