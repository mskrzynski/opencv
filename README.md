# opencv

Program C++ z OpenCV (live obraz, śledzenie piłeczki, zaliczanie punktu)
 dla Visual Studio 2017

TODO:
1. Rozdzielenie rysowania pilki (na żywo w pętli) i rysowania stołu (co jakiś czas, poza pętlą)

2. Rysowanie okręgu piłeczki wraz z istniejącą linią:

http://layer0.authentise.com/detecting-circular-shapes-using-contours.html

https://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html (minEnclosingCircle, approxPolyDP - przetestowac te funkcje)

Czy istniejacy obraz HSV (ktory musi byc idealnie okrągły) będzie w tym pomocny?

3. Implementacja nachodzenia piłeczki na prostokąt: https://www.google.pl/search?q=opencv+overlapping i ustawiania flagi na to zdarzenie

4. Przesyłanie flagi do aplikacji C#