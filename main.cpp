
#include <iomanip>
#include <assert.h>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector> 
#include <cmath>
#include <string>
#include < stdlib.h >

std::vector<cv::String> read_files(std::string filepath, bool yes_sub_directory);
std::vector<cv::Mat> split_rbg_channels(cv::String path);
std::vector<std::map<int, double>> pdf(cv::Mat each_pixels_rbg);
int pdf_sum(std::map<int, double> given_pdf);
double mean(std::map<int, double> given_pdf);
double variance(std::map<int, double> given_pdf, double mean);
double nth_central_moment(std::map<int, double> given_pdf, double mean, int nth);
void displayImage(std::string path, std::vector<cv::Mat> rbg_imgs, std::string gray_scaled_image_response);
void calcHistogram(std::vector<cv::String> list_of_imgs);
void displayHistogram(std::string path);

int main() {


	//for example: C:/Users/Joel/Documents/images
	std::string file_path = "";

	//varaible whether subdirectories are present or not
	std::string subdirectories_present = "false";
	bool subdirectories_present_bool = false;

	//menu input
	char menu_input = 'a';

	//exit menu 
	std::string continue_or_not = "yes";



	do {

		std::cout << "enter file path (make sure not to include space in filepath): ";
		std::cin >> file_path;

		//try
		//catch


		std::cout << "are there subdirectories? Enter yes or no: ";
		std::cin >> subdirectories_present;
		std::cout << std::endl;

		if (subdirectories_present == "yes") {
			subdirectories_present_bool = true;
		}

		//returns a list of filepaths 
		std::vector<cv::String> list_of_imgs = read_files(file_path, bool(subdirectories_present_bool));

		std::cout << "choose operation for image. a to e:" << std::endl;
		std::cout << "a: histogram analysis" << std::endl;
		std::cout << "b: coming soon..." << std::endl;
		std::cout << "c: coming soon..." << std::endl;
		std::cout << "d: coming soon..." << std::endl;
		std::cout << "e: coming soon..." << std::endl;

		std::cin >> menu_input;


		switch (menu_input) {
		case 'a':
			calcHistogram(list_of_imgs);
			break;
		default:
			std::cout << "invalid option";
		}


		std::cout << "would you like to continue? Enter yes or no: ";
		std::cin >> continue_or_not;

	} while (continue_or_not != "no");

	return 0;
}

std::vector<cv::String> read_files(std::string filepath, bool yes_sub_directory)
{
	std::string folder(filepath);
	std::vector<cv::String> filenames;
	//last parameter is true for recursion 
	//glob function returns a vector of filepaths. 
	cv::glob(folder, filenames, yes_sub_directory);

	for (int i = 0; i < filenames.size(); i++)
	{

		std::cout << "reading..." << filenames[i] << std::endl;
	}

	std::cout << std::endl;

	return filenames;


}

std::vector<cv::Mat> split_rbg_channels(cv::String path) {


	//reads in an image and assigns to Mat
	std::map<int, int> Mp;
	//Mats for images and pixel intensities 
	cv::Mat img = cv::imread(path), merged;
	cv::Mat r, b, g;
	//splits each pixel into their respective channels 3
	cv::Mat rbg_channels[3];

	cv::split(img, rbg_channels);
	r = rbg_channels[2];
	b = rbg_channels[0];
	g = rbg_channels[1];
	//list of vectors with each value in order of red, blue, green channels 
	std::vector<cv::Mat> each_pixel_rbg = { r, b, g };

	//merges into single mat with each value representing pixel broken into 3 channels
	// each pixel is red, blue, green intensities 
	cv::merge(each_pixel_rbg, merged);

	int red_intensity = merged.at<cv::Vec3b>(0, 0)[0];
	int blue_intensity = merged.at<cv::Vec3b>(0, 0)[1];
	int green_intensity = merged.at<cv::Vec3b>(0, 0)[2];


	//iterates through each pixel and checks to see if is grayscaled or colored image
	/*std::cout << "rgb channels for [0,0] pixel..." << merged.at<cv::Vec3b>(0, 0) << std::endl;
	std::cout << "red..." << red_intensity << std::endl;
	std::cout << "blue..." << blue_intensity << std::endl;
	std::cout << "green..." << green_intensity << std::endl;*/





	return { merged, r, b, g };


}

std::vector<std::map<int, double>> pdf(cv::Mat each_pixels_rbg, std::string gray_scaled_image_response) {


	//map to create frequency table for red, blue, and green maps
	std::map<int, double> red_mp, blue_mp, green_mp, gray_scaled_mp;

	//total_pixels 
	int total_pixels = each_pixels_rbg.rows * each_pixels_rbg.cols;

	std::cout << std::endl << "the total number of pixels (M X N) is..." << total_pixels << std::endl;

	int rows = each_pixels_rbg.rows;
	int cols = each_pixels_rbg.cols;


	//creates a frequency table for gray_scaled image 
	if (gray_scaled_image_response == "yes") {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				int intensity_value = each_pixels_rbg.at<cv::Vec3b>(i, j)[0];

				if (gray_scaled_mp.find(intensity_value) == gray_scaled_mp.end()) {
					gray_scaled_mp[intensity_value] = double(1) / double(total_pixels);
				}
				else {
					gray_scaled_mp[intensity_value] += double(1) / double(total_pixels);
				}
			}
		}
	}
	//creates a frequency table for colored images 
	else {

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {

				int red = each_pixels_rbg.at<cv::Vec3b>(i, j)[0];
				int blue = each_pixels_rbg.at<cv::Vec3b>(i, j)[1];
				int green = each_pixels_rbg.at<cv::Vec3b>(i, j)[2];


				//red channel
				if (red_mp.find(red) == red_mp.end()) {
					red_mp[red] = double(1) / double(total_pixels);
				}
				else {
					red_mp[red] += double(1) / double(total_pixels);
				}

				//blue channel
				if (blue_mp.find(blue) == blue_mp.end()) {
					blue_mp[blue] = double(1) / double(total_pixels);
				}
				else {
					blue_mp[blue] += double(1) / double(total_pixels);
				}

				//green channel
				if (green_mp.find(green) == green_mp.end()) {
					green_mp[green] = double(1) / double(total_pixels);
				}
				else {
					green_mp[green] += double(1) / double(total_pixels);
				}


			}
		}


	}

	//iterates through 0 to 255 to assign 0s to missing intensity values

	if (gray_scaled_image_response == "yes") {
		for (int i = 0; i <= 255; i++) {
			if (gray_scaled_mp.find(i) == gray_scaled_mp.end()) {
				gray_scaled_mp[i] = 0;
			}
		}
	}
	else {
		for (int i = 0; i <= 255; i++) {
			if (red_mp.find(i) == red_mp.end()) {
				red_mp[i] = 0;
			}


			//blue channel
			if (blue_mp.find(i) == blue_mp.end()) {
				blue_mp[i] = 0;
			}


			//green channel
			if (green_mp.find(i) == green_mp.end()) {
				green_mp[i] = 0;
			}
		}
	}



	if (gray_scaled_image_response == "yes") {
		std::string print_grayscaled = "";

		std::cout << "would you like to print probability distribution for gray scaled image? Enter yes or no." << std::endl;
		std::cin >> print_grayscaled;

		for (auto& value : gray_scaled_mp) {
			if (print_grayscaled == "yes") {
				std::cout << value.first << ": " << double(value.second) / double(total_pixels) << std::endl;
			}
			else {
				break;
			}
		}

		return { gray_scaled_mp };

	}

	std::string print_red = "";
	std::string print_blue = "";
	std::string print_green = "";

	std::cout << "would you like to print probability distribution for red channel? Enter yes or no." << std::endl;
	std::cin >> print_red;

	for (auto& value : red_mp) {
		if (print_red == "yes") {
			std::cout << value.first << ": " << double(value.second) / double(total_pixels) << std::endl;
		}
		else {
			break;
		}
	}



	std::cout << "would you like to print probability distribution for blue channel? Enter yes or no." << std::endl;
	std::cin >> print_blue;

	for (auto& value : blue_mp) {
		if (print_blue == "yes") {
			std::cout << value.first << ": " << double(value.second) / double(total_pixels) << std::endl;
		}
		else {
			break;
		}
	}

	std::cout << "would you like to print probability distribution for green channel? Enter yes or no." << std::endl;
	std::cin >> print_green;

	for (auto& value : green_mp) {
		if (print_green == "yes") {
			std::cout << value.first << ": " << double(value.second) / double(total_pixels) << std::endl;
		}
		else {
			break;
		}
	}


	return { red_mp, blue_mp, green_mp };

}

int pdf_sum(std::map<int, double> given_pdf) {

	double total_sum = 0;

	for (auto& value : given_pdf) {
		total_sum += value.second;
	}

	return total_sum;
}

double mean(std::map<int, double> given_pdf) {
	double average_intensity = 0;
	for (auto& value : given_pdf) {
		average_intensity += value.first * value.second;
	}

	return average_intensity;
}

double variance(std::map<int, double> given_pdf, double mean) {
	double variance = 0;
	for (auto& value : given_pdf) {
		variance += pow(value.first - mean, 2) * value.second;
	}

	return variance;
}

double nth_central_moment(std::map<int, double> given_pdf, double mean, int nth) {
	double central_moment = 0;
	for (auto& value : given_pdf) {
		central_moment += pow(value.first - mean, nth) * value.second;
	}

	return central_moment;
}

void calcHistogram(std::vector<cv::String> list_of_imgs) {

	//skip photo yes or not 
	std::string skip_photo = "none";

	//is image grayscaled?
	std::string gray_scaled_image_response = "";

	//variables for image processing 
	std::vector<cv::Mat> rbg_channels_img;
	std::vector<std::map<int, double>> rbg_pdf_channels;


	for (int i = 0; i < list_of_imgs.size(); i++)
	{

		//asking user whether they'd like to skip image or not 
		std::cout << "would you like to skip " << list_of_imgs[i] << "?" << " Enter yes or no: ";
		std::cin >> skip_photo;

		if (skip_photo == "yes") {
			std::cout << "photo skipped." << std::endl;
			continue;
		}

		//asking user whether image is grayscaled or not
		std::cout << "is " << list_of_imgs[i] << " a grayscaled image?" << " Enter yes or no: ";
		std::cin >> gray_scaled_image_response;
		std::cout << std::endl;

		if (gray_scaled_image_response == "yes") {
			std::cout << "processing grayscaled image..." << std::endl;
			std::vector<cv::Mat> rbg_channels_img = split_rbg_channels(list_of_imgs[i]);
			std::vector<std::map<int, double>> rbg_pdf_channels = pdf(rbg_channels_img[0], gray_scaled_image_response);

			std::cout << std::endl;
			std::cout << "these are statistics for image";
			std::cout << "sum of probabilities..." << pdf_sum(rbg_pdf_channels[0]) << std::endl;


			std::cout << std::endl;
			double gray_scaled_mean = mean(rbg_pdf_channels[0]);

			std::cout << "average intensity..." << gray_scaled_mean << std::endl;

			std::cout << std::endl;
			std::cout << "variance..." << variance(rbg_pdf_channels[0], gray_scaled_mean) << std::endl;


			std::cout << std::endl;
			std::cout << "skewness..." << nth_central_moment(rbg_pdf_channels[0], gray_scaled_mean, 3) << std::endl;


			std::cout << std::endl;
			std::cout << "kurtosis..." << nth_central_moment(rbg_pdf_channels[0], gray_scaled_mean, 4) << std::endl;


			displayImage(list_of_imgs[i], rbg_channels_img, gray_scaled_image_response);
			/*displayHistogram(list_of_imgs[i]);*/
		}
		else {
			std::cout << "processing colored image..." << std::endl;
			std::vector<cv::Mat> rbg_channels_img = split_rbg_channels(list_of_imgs[i]);
			std::vector<std::map<int, double>> rbg_pdf_channels = pdf(rbg_channels_img[0], gray_scaled_image_response);

			std::cout << std::endl;
			std::cout << "these are statistics for image";

			std::cout << std::endl;
			std::cout << "sum of probabilities within red channel..." << pdf_sum(rbg_pdf_channels[0]) << std::endl;
			std::cout << "sum of probabilities within blue channel..." << pdf_sum(rbg_pdf_channels[1]) << std::endl;
			std::cout << "sum of probabilities within green channel..." << pdf_sum(rbg_pdf_channels[2]) << std::endl;

			std::cout << std::endl;
			double red_mean = mean(rbg_pdf_channels[0]);
			double blue_mean = mean(rbg_pdf_channels[1]);
			double green_mean = mean(rbg_pdf_channels[2]);
			std::cout << "average intensity within red channel..." << red_mean << std::endl;
			std::cout << "average intensity within blue channel..." << blue_mean << std::endl;
			std::cout << "average intensity within green channel..." << green_mean << std::endl;

			std::cout << std::endl;
			std::cout << "variance within red channel..." << variance(rbg_pdf_channels[0], red_mean) << std::endl;
			std::cout << "variance within blue channel..." << variance(rbg_pdf_channels[1], blue_mean) << std::endl;
			std::cout << "variance within green channel..." << variance(rbg_pdf_channels[2], green_mean) << std::endl;

			std::cout << std::endl;
			std::cout << "skewness within red channel..." << nth_central_moment(rbg_pdf_channels[0], red_mean, 3) << std::endl;
			std::cout << "skewness within blue channel..." << nth_central_moment(rbg_pdf_channels[1], blue_mean, 3) << std::endl;
			std::cout << "skewness within green channel..." << nth_central_moment(rbg_pdf_channels[2], green_mean, 3) << std::endl;

			std::cout << std::endl;
			std::cout << "kurtosis within red channel..." << nth_central_moment(rbg_pdf_channels[0], red_mean, 4) << std::endl;
			std::cout << "kurtosis within blue channel..." << nth_central_moment(rbg_pdf_channels[1], blue_mean, 4) << std::endl;
			std::cout << "kurtosis within green channel..." << nth_central_moment(rbg_pdf_channels[2], green_mean, 4) << std::endl;

			displayImage(list_of_imgs[i], rbg_channels_img, gray_scaled_image_response);
			/*displayHistogram(list_of_imgs[i]);*/
		}
	}

}

void displayImage(std::string path, std::vector<cv::Mat> rbg_imgs, std::string gray_scaled_image_response) {

	if (gray_scaled_image_response == "yes") {
		//get a Mat object (a n-dimensional array) with img
		cv::Mat img = cv::imread(path);
		//shows original, r, b, and g images
		cv::imshow("Image", img);
		//super important lets the image stay
		cv::waitKey(0);
	}
	else {
		//get a Mat object (a n-dimensional array) with img
		cv::Mat img = cv::imread(path);
		//shows original, r, b, and g images
		cv::imshow("Image", img);
		cv::imshow("red", rbg_imgs[1]);
		cv::imshow("blue", rbg_imgs[2]);
		cv::imshow("green", rbg_imgs[3]);
		//super important lets the image stay
		cv::waitKey(0);
	}
}

void displayHistogram(std::string path) {
	// read a color image
	cv::Mat src_img = cv::imread(path);
	if (src_img.empty()) {
		throw (std::string("Unable to access the image file ") + path);
	}

	std::vector<cv::Mat> bgr_planes;
	split(src_img, bgr_planes);

	// how many bins for the histogram?
	int hist_size = 256;

	// set the ranges ( for B,G,R )
	// the upper limit is exclusive
	float range[] = { 0, 256 };
	const float* hist_range[] = { range };

	// set histogram parameters
	bool uniform = true;
	bool accumulate = false;

	// compute the histogram
	cv::Mat b_hist, g_hist, r_hist;

	calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &hist_size, hist_range, uniform, accumulate);

	calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &hist_size, hist_range, uniform, accumulate);

	calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &hist_size, hist_range, uniform, accumulate);


	// draw the histograms for B, G and R planes
	int hist_w = 512;
	int hist_h = 400;

	int bin_w = cvRound((double)hist_w / hist_size);

	cv::Mat hist_img(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	// normalize the result to ( 0, histImage.rows )
	normalize(b_hist, b_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());

	normalize(g_hist, g_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());

	normalize(r_hist, r_hist, 0, hist_img.rows, cv::NORM_MINMAX, -1, cv::Mat());


	// draw histogram for each channel]
	for (int i = 1; i < hist_size; i++) {
		line(hist_img, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
		line(hist_img, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))), cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
		line(hist_img, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))), cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, 8, 0);
	}

	// save histogram image to disk
	/*imwrite("histogram.jpg", hist_img);*/

	// display image and its histogram
	/*imshow("Source image", src_img);*/
	imshow("Image histogram", hist_img);
};

