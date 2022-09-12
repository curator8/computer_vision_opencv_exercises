
#include <iomanip>
#include <assert.h>
#include<iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector> 
#include<cmath>




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
	std::cout << "rgb channels for [0,0] pixel..." << merged.at<cv::Vec3b>(0, 0) << std::endl;
	std::cout << "red..." << red_intensity << std::endl;
	std::cout << "blue..." << blue_intensity << std::endl;
	std::cout << "green..." << green_intensity << std::endl;


	return { merged, r, b, g };


}

std::vector<std::map<int, double>> pdf(cv::Mat each_pixels_rbg) {
	//not checking if it's gray scaled yet

	//map to create frequency table for red, blue, and green maps
	std::map<int, double> red_mp, blue_mp, green_mp;

	//total_pixels 
	int total_pixels = each_pixels_rbg.rows * each_pixels_rbg.cols;

	std::cout << "the total number of pixels (M X N) is..." << total_pixels << std::endl;

	int rows = each_pixels_rbg.rows;
	int cols = each_pixels_rbg.cols;

	//creates a frequency table for colored images 
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

	//iterates through 0 to 255 to assign 0s to missing intensity values
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


	std::cout << "probability distribution for red channel: " << std::endl;

	for (auto& value : red_mp) {
		std::cout << value.first << ": " << double(value.second) / double(total_pixels) << std::endl;
	}


	std::cout << "probability distribution for blue channel: " << std::endl;

	for (auto& value : blue_mp) {

		std::cout << value.first << ": " << double(value.second) / double(total_pixels) << std::endl;
	}

	std::cout << "probability distribution for green channel: " << std::endl;

	for (auto& value : green_mp) {

		std::cout << value.first << ": " << double(value.second) / double(total_pixels) << std::endl;
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

void displayImage(std::string path, std::vector<cv::Mat> rbg_imgs) {
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

int main(int argc, char** argv) {

	//change the filepath, is able to read through sub-directories 
	std::vector<cv::String> list_of_imgs = read_files("C:/Users/Joel/Documents/images", true);

	for (int i = 0; i < list_of_imgs.size(); i++)
	{

		std::cout << list_of_imgs[i] << std::endl;
		std::vector<cv::Mat> rbg_channels_img = split_rbg_channels(list_of_imgs[i]);


		std::vector<std::map<int, double>> rbg_pdf_channels = pdf(rbg_channels_img[0]);

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

		displayImage(list_of_imgs[i], rbg_channels_img);
	}





}