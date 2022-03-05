#define _CRT_SECURE_NO_WARNINGS

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// without filters
int main(int argc, char** argv)
{
	// nacitanie obrazka
	Mat image = imread("../../Assigment_1/FullCeils/Tv7.png", IMREAD_GRAYSCALE);

	// binarizacia
	Mat thresholded;
	threshold(image, thresholded, 90, 255, THRESH_BINARY);
	imwrite("../../Assigment_1/FullCeils/Tv7FThresholded.png", thresholded);

	// dilatacia
	Mat dilatedF;
	auto elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
	dilate(thresholded, dilatedF, elipse_kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FDilate1.png", dilatedF);

	// closing
	Mat closing;
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	erode(dilatedF, closing, elipse_kernel);
	dilate(closing, closing, elipse_kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FClosing.png", closing);

	// dilatacia
	Mat dilatedS;
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));
	dilate(closing, dilatedS, elipse_kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FDilate2.png", dilatedS);

	// closing
	Mat closing2;
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(47, 47));
	erode(dilatedS, closing2, elipse_kernel);
	dilate(closing2, closing2, elipse_kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FClosing2.png", closing2);

	// dilatacia
	Mat dilatedT;
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	dilate(closing2, dilatedT, elipse_kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FDilate3.png", dilatedT);

	// oppening
	Mat opening;
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(32, 32));
	dilate(dilatedT, opening, elipse_kernel);
	erode(opening, opening, elipse_kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FOpenning.png", opening);

	// dilatacia
	Mat dilated;
	elipse_kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(opening, dilated, elipse_kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FDilate4.png", dilated);

	// rozdiel
	Mat diff;
	bitwise_xor(opening, dilated, diff);
	imwrite("../../Assigment_1/FullCeils/Tv7FDiff.png", diff);

	// konturova analyza
	vector<vector<Point> > finalContours;
	findContours(diff, finalContours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<RotatedRect> minEllipse(finalContours.size());
	for (size_t i = 0; i < finalContours.size(); i++)
	{
		if (finalContours[i].size() >= 5)
		{
			minEllipse[i] = fitEllipse(finalContours[i]);
		}
	}

	// vykreslenie hran
	Mat drawing;
	cvtColor(image, drawing, COLOR_GRAY2RGB);
	int index = 0;
	for (size_t i = 0; i < finalContours.size(); i++)
	{
		// podmienky na maximalnu a minimalnu sirku a vysku
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

	// vypis poctu buniek
	printf("Ceil count: %d\n", index);\
	// Ceil count: 48
	// Ceil count: 274

	// ulozenie vysledneho obrazka
	imwrite("../../Assigment_1/FullCeils/Tv7FOutput.png", drawing);

	// nacitanie filtracnych jadier
	Mat kernel = imread("../../Assigment_1/kernel.png", IMREAD_GRAYSCALE);
	Mat kernel2 = imread("../../Assigment_1/kernel2.png", IMREAD_GRAYSCALE);
	threshold(kernel, kernel, 0, 255, THRESH_BINARY);
	threshold(kernel2, kernel2, 0, 255, THRESH_BINARY);
	imwrite("../../Assigment_1/FullCeils/Tv7FKernel1.png", kernel);
	imwrite("../../Assigment_1/FullCeils/Tv7FKernel2.png", kernel2);

	// convolucia prvym filtrom
	Mat filtered;
	filter2D(image, filtered, CV_32FC1, kernel);
	normalize(filtered, filtered, 0, 1, cv::NORM_MINMAX);
	imwrite("../../Assigment_1/FullCeils/Tv7FConv1.png", filtered);

	// convolucia druhym filtrom
	Mat filtered2;
	filter2D(image, filtered2, CV_32FC1, kernel2);
	normalize(filtered2, filtered2, 0, 1, cv::NORM_MINMAX);
	imwrite("../../Assigment_1/FullCeils/Tv7FConv2.png", filtered2);

	// rozmazanie pomocou gausovho filtra
	Mat blur;
	GaussianBlur(drawing, blur, Size(3, 3), 0);
	imwrite("../../Assigment_1/FullCeils/Tv7FGaus.png", blur);

	// kontury pomocou canny algoritmu
	Mat canny;
	Canny(drawing, canny, 10, 30);
	imwrite("../../Assigment_1/FullCeils/Tv7FCanny.png", canny);

	// vynatie hran pomocou sobelovho filtra
	Mat sobelx, sobely, sobelxy;
	Sobel(drawing, sobelx, CV_64F, 1, 0, 5);
	Sobel(drawing, sobely, CV_64F, 0, 1, 5);
	Sobel(drawing, sobelxy, CV_64F, 1, 1, 5);
	imwrite("../../Assigment_1/FullCeils/Tv7FSobelx.png", sobelx);
	imwrite("../../Assigment_1/FullCeils/Tv7FSobely.png", sobely);
	imwrite("../../Assigment_1/FullCeils/Tv7FSobelxy.png", sobelxy);

	waitKey();
	return 0;
}