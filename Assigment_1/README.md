
# Zadanie 1
Autor: Jakub Kucecka
AIS ID: 92068

## Funkcie
Ukaldanie a nacitanie obrazu
 - imread()
 - imwrite()

Binarizacia
 - threshold()

Morfologicka analyza
 - getStructuringElement()
 - dilate()
 - erode()
 - bitwise_xor()

Konturova analyza
 - findContours()
 - fitEllipse()

Vykreslenie
 - elipse()
 - putText()

Filtrovanie obazu
 - filter2D()
 - normalize()
 - 	GaussianBlur()
 - Canny()
 - Sobel()

## Kod
Ako prve sme si vykonali nacitanie obrazka.
```
	Mat image = imread("../../Assigment_1/Tv7.png", IMREAD_GRAYSCALE);
```
![Obrazok](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7.png)

Obrazok sme binarizovali pomocou funkcie threshold z grazscale na binarny obrazok.
```
	threshold(image, thresholded, 90, 255, THRESH_BINARY);
```
![Binarizacia](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FThresholded.png)

Vykonali sme prvotnu morfologicku alanyzu v podobe dilatacia, cim sme odstranili prvotny sum.
```
	auto elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
	dilate(thresholded, dilatedF, elipse_kernel);
	imwrite("../../Assigment_1/Tv7FDilate1.png", dilatedF);
```
![Prvotna dilatacia](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FDilate1.png)

Metoda uzavretia nam pomohla zjednotit jednotlive male bloby buniek.
```
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	erode(dilatedF, closing, elipse_kernel);
	dilate(closing, closing, elipse_kernel);
```
![Prvotny closing](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FClosing.png)

Opat sme pouzili dilataciu, kedze sme closingom spojili male bloby, otvoril sa nam priestor na odstranenie sumu s vacsim strukturalnym elementom.
```
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));
	dilate(closing, dilatedS, elipse_kernel);
```
![Druhotna dilatacia](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FDilate2.png)

Nasledne sme pouzili opat closing. Teraz sme uz uzavreli celu bunku.
```
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(47, 47));
	erode(dilatedS, closing2, elipse_kernel);
	dilate(closing2, closing2, elipse_kernel);
```
![Druhotny closing](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FClosing2.png)

Opat sme pouzili dilataciu este s vacsim strukturalnym elementom aby sme odstranili aj posledny su, ktory ostal v obrazku.
```
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	dilate(closing2, dilatedT, elipse_kernel);
```
![Tretia dilatacia](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FDilate3.png)

Nasledne sme pouzili otvorenie, ktore zabezpecilo rozdelenie niektorych spojenych buniek.
```
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(32, 32));
	dilate(dilatedT, opening, elipse_kernel);
	erode(opening, opening, elipse_kernel);
```
![Openning](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FOpenning.png)

Posledna dilatacia sluzi iba na zmensenie buniek aby sme neskor vedeli urobit rozdiel a vytiahli tak kontury.
```
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(opening, dilated, elipse_kernel);
```
![Finalna dilatacia](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FDilate4.png)

Ako bolo spomenute vyssie, urobili sme rozdiel medzi poslednou dilataciou a openningom. Cim sme dostali kontury buniek.
```
	bitwise_xor(opening, dilated, diff);
```
![Rozdiel](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FDiff.png)

Pomocou funkcie findContours sme z obrazku rozdielov vybrali jednotlive pozicie tychto kontur, cim sme zacali konturovu analyzu. Zo ziskanych kontur sme vytvorili elipsy, ktore museli splnat podmienku, ze kontura ma aspon 4 body.
```
	findContours(diff, finalContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<RotatedRect> minEllipse(finalContours.size());
	for (size_t i = 0; i < finalContours.size(); i++)
	{
		if (finalContours[i].size() >= 5)
		{
			minEllipse[i] = fitEllipse(finalContours[i]);
		}
	}
```

Najdene elipsy sme vykreslili do povodneho obrazka. Tie ktore splnali minimalne a maximalne poziadavky sme vykreslili zelenou farbou a tie, ktore nesplnanli podmienky su cervenou. Pridali sme aj poradie uspesne najdenych buniek modrou farbou.
```
	cvtColor(image, drawing, COLOR_GRAY2RGB);
	int index = 0;
	for (size_t i = 0; i < finalContours.size(); i++)
	{
		if (minEllipse[i].size.height < 170 && minEllipse[i].size.width < 170
			&& minEllipse[i].size.height > 50 && minEllipse[i].size.width > 50)
		{
			index += 1;
			Scalar color = Scalar(0, 255, 0);
			ellipse(drawing, minEllipse[i], color, 3);
			putText(drawing, std::to_string(index), Point(minEllipse[i].center.x, minEllipse[i].center.y), cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(255, 0, 0), 3);
		}
		else {
			Scalar color = Scalar(0, 0, 255);
			ellipse(drawing, minEllipse[i], color, 3);
		}
	}
```

Vypisali sme celkovy pocet uspesne najdenych buniek v obrazku. Vidime ale, ze dve zretelne bunky algoritmus nenasiel a nenasiel aj niektore na okraji, dve oznacil ako nepravdive, nakolko sa spojili s okolitym sumom.
```
	printf("Ceil count: %d\n", index);\
	// Ceil count: 274
```
![Vysledny obrazok](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FOutput.png)

Pri prvom zadani sme si vyskusali aj filtraciu obrazka. V prvom rade sme si nacitali jednotlive kernely na filtraciu.
```
	Mat kernel = imread("../../Assigment_1/kernel.png", IMREAD_GRAYSCALE);
	Mat kernel2 = imread("../../Assigment_1/kernel2.png", IMREAD_GRAYSCALE);
	threshold(kernel, kernel, 0, 255, THRESH_BINARY);
	threshold(kernel2, kernel2, 0, 255, THRESH_BINARY);
```
![Filter 1](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FKernel1.png)
![Filter 2](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FKernel2.png)

Pouzili sme fukciu filter2D, ktory nahradza convoluciu a nasledne sme obrazok normalizovali. Ziskali sme tak stredy jednotlivych buniek pomerne jednoznacne.
```
	filter2D(image, filtered, CV_32FC1, kernel);
	normalize(filtered, filtered, 0, 1, NORM_MINMAX);
```
![Convolucia s prvym filtrom](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FConv1.png)

Vykonali sme aj convoluciu s druhym filtrom, ktory je pravidelnejsi ale vyznamny rozdiel na obrazku neneastal.
```
	filter2D(image, filtered2, CV_32FC1, kernel2);
	normalize(filtered2, filtered2, 0, 1, cv::NORM_MINMAX);
```
![Convolucia s druhym filtrom](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FConv2.png)

Vyskusali sme si aj rozmazanie brazka pomocou gausovho filtra.
```
	GaussianBlur(drawing, blur, Size(3, 3), 0);
```
![Rozmazanie gausovym filtrom]()

Hrany v obrazku sme hladali pomocou canny algoritmu.
```
	Canny(drawing, canny, 10, 30);
```
![Hranovy canny filter](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FCanny.png)

Tiez sme si vyskusali sobelov filter ako uz vertikalny tak horizontalny a aj aj.
```
	Sobel(drawing, sobelx, CV_64F, 1, 0, 5);
	Sobel(drawing, sobely, CV_64F, 0, 1, 5);
	Sobel(drawing, sobelxy, CV_64F, 1, 1, 5);
```
![Hranovy horizontalny sobelov filter](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FSobelx.png)
![Hranovy vertikalny sobelov filter](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FSobely.png)
![Hranovy obojsmerny sobelov filter](https://github.com/JakubKucecka/PoVid/blob/master/Assigment_1/FullCeils/Tv7FSobelxy.png)
