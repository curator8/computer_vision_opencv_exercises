#include <opencv2/imgcodecs.hpp>
#include <iomanip>
#include <vector>
#include <assert.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utility.hpp>


#include <opencv2/opencv.hpp>
#include <string>
#include <vector> 
#include <string> 




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


//create a function that checks to see if the image has one channel or 3 channels 
//then read through every pixel and make a dictionary of each intensity
// from there you can actually actually get teh count_of_intensity / total_intensity values 
//


// returns an array with probability distribution
std::vector<float> pdf(std::vector<cv::String> list_of_images) {

	

	static int a[5];
	for (int i = 0; i < 5; i++)
	{
		a[i] = i;
	}

	return a;

}

void pmf() {

}

void mean() {

}


void variance() {

	
}

void displayImage(std::string path) {
	//get a Mat object (a n-dimensional array) with img
	cv::Mat img = cv::imread(path);
	//show image(s)
	imshow("Image", img);
	//super important lets the image stay
	cv::waitKey(0);
}

int main() {

	std::vector<cv::String> list_of_imgs = read_files("C:/Users/Joel/Documents/images", true );

	/*for (int i = 0; i < list_of_imgs.size(); i++) 
	{
		displayImage(list_of_imgs[i]); 
	}*/
	


	



} 