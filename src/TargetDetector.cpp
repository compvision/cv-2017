#include "TargetDetector.hpp"
#include "Target.hpp"
#include <iostream>
//public methods

TargetDetector::TargetDetector() {
    //do derpy things
}

Target* TargetDetector::processImage(Mat input, bool tar) {

    GaussianBlur(input,input,Size(3,3),31);
    //input = canny(thresholdImage(input,53,58,228,238));
    input = thresholdImage(input,53,58,228,238);
   // imshow("Threshold", input);
    input = canny(input);
   // imshow("Canny", input);
    dilate(input, input, Mat());

    std::vector<std::vector<Point> > contours = contour(input);
    // std::cout << "not contours" << std::endl;
    std::cout << "processimg: before filter contours" << std::endl;
    std::vector<std::vector<Point> > finalContour = filterContours(contours, input, tar);

    std::cout <<"processimg: after filter contours" << std::endl;


   // imshow("Contours",input);
    std::cout << "processimg: imshowed" << std::endl;

    /* if (&finalContour[0] == NULL || &finalContour[1] == NULL) {
         //std::cout << "Null" << std::endl;
         return NULL;
     }*/

    std::cout << "processimg: before contour size check" << std::endl;
    std::cout << finalContour.size() << std::endl;
    std::cout << "processimg: after contour size check" << std::endl;

    if(finalContour.size() == 0)
    {
        return NULL;
    }
    else {
        std::cout << "Making Target" << std::endl;
        Target* toReturn = new Target(finalContour);
        std::cout << "Made Target" << std::endl;
        toReturn -> setTar(tar);
        return toReturn;
    }
}

Mat TargetDetector::canny(Mat input) {
    Canny(input, input, 0, 25, 3,false);
    return input;
}


Mat TargetDetector::thresholdImage(Mat input, int minHue, int maxHue, int minVal, int maxVal) {
    //defining variables
    Mat threshLow;
    Mat threshHigh;
    Mat hueResult;
    //Mat satResult;
    Mat valResult;

    //seperating image into HSV (or in this case, really just HV)
    cv::Mat cvted;
    cv::cvtColor(input, cvted, CV_BGR2HSV);

    std::vector<cv::Mat> separated(3);
    cv::split(cvted, separated);
    Mat hue = separated.at(0).clone();
    //Mat sat = separated.at(1).clone();
    Mat val = separated.at(2).clone();

    //Hue
    threshold(hue, threshLow, minHue, 255, THRESH_BINARY);
    threshold(hue, threshHigh, maxHue, 255, THRESH_BINARY_INV);
    hueResult = threshLow & threshHigh;

    //Saturation
    /* threshold(sat, threshLow, minSat, 255, THRESH_BINARY);
     threshold(sat, threshHigh, maxSat, 255, THRESH_BINARY_INV);
     satResult = threshLow & threshHigh;
    */
    //Value
    threshold(val, threshLow, minVal, 255, THRESH_BINARY);
    threshold(val, threshHigh, maxVal, 255, THRESH_BINARY_INV);
    valResult = threshLow & threshHigh;

    Mat combined = hueResult & valResult; //& satResult;

    return combined;
}

std::vector<std::vector<Point> > TargetDetector::contour(Mat input) {
    std::vector<std::vector<Point> > contours;

    findContours(input, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    return contours;
}

double TargetDetector::angle(cv::Point p1, cv::Point p2, cv::Point p0) {
    double dx1 = p1.x - p0.x;
    double dy1 = p1.y - p0.y;
    double dx2 = p2.x - p0.x;
    double dy2 = p2.y - p0.y;
    return atan(dy1/dx1)-atan(dy2/dx2); //in rad
}

std::vector<std::vector<Point> > TargetDetector::filterContours(std::vector<std::vector<Point> > contours, cv::Mat img, bool tar)
{
    //bool tar: true = gears, false = boiler
    std::vector<cv::Point> arcContour;
    std::vector<cv::Point> outputContour;

    std::vector<std::vector<cv::Point> > gearVector;
    std::vector<std::vector<cv::Point> > boilerVector;

//    int tarNum = 0;
    if (tar == false)
    {
		int maxArea = 0;	
		for(int j = 0; j< contours.size(); j++)
        {	
		
			//Scalar color(255,0,0);
      		//cv::drawContours(img, contours, -1, color, 1);
            cv::RotatedRect rect = cv::minAreaRect(contours[j]);
			//std::vector<cv::Point> corners(4);
			cv::Point2f corners[4];			
			rect.points(corners);

            std::cout << " corners " << sizeof(corners) << std::endl;
            std::vector<cv::Point> cornersInt;
            for(int i = 0 ; i < 4; i++)
            {
             //   std::cout << "corners at " << i << ": " << corners[i] << std::endl;
                cv::Point point((int)corners[i].x, (int)corners[i].y);
                cornersInt.push_back(point);
            }    
			
			Target tempOne(cornersInt);
			if( rect.angle < 23)
			{
				if(tempOne.getWidth() > tempOne.getHeight() && contourArea(cornersInt) > 250)
				{
					if(tempOne.getHeight()/tempOne.getWidth() < 0.45  && tempOne.getHeight()/tempOne.getWidth() > 0.1) 		
					{
	                	std::cout << "found target" << std::endl;
						bool duplicate = false;					
						for(int j = 0; j < boilerVector.size(); j++)
	                    {
	                        Target tempTwo(boilerVector[j]);
	                        if(abs(tempOne.getCenter().x - tempTwo.getCenter().x) < 7 && abs(tempOne.getCenter().y - tempTwo.getCenter().y) < 7 ) //add 	area check
	                        {
	                            duplicate = true;
	                        }
	
	                    }
						if(duplicate == false)
	                    {           
							boilerVector.push_back(cornersInt);
						}
					}
                    
				}	
			}  
		}

        std::cout << "boiler vector size " << sizeof(boilerVector) << std::endl;
		//Scalar color(255,0,0);
        //cv::drawContours(img, boilerVector, -1, color, 1);
		    
		if(boilerVector.size() > 0)
        {
			for(int i = 0; i < boilerVector.size(); i++)
			{
                std::cout << "i : " << i << std::endl;
				Target tempOne(boilerVector[i]);
				for(int j = i+1; j < boilerVector.size(); j++)
				{
                    std::cout << "j: " << j << std::endl;
					Target tempTwo(boilerVector[j]);

					if(abs(tempOne.getCenter().x - tempTwo.getCenter().x) < 14 && abs(tempOne.getWidth()-tempTwo.getWidth()) < 16)
					{
						
                    		    std::cout << "passed the center checks" << std::endl;
								std::vector<std::vector<cv::Point> > returnVector;
                    	    
                				if(tempOne.getCenter().x < tempTwo.getCenter().x)
                				{
		            		    	returnVector.push_back(boilerVector[i]);
		            		    	returnVector.push_back(boilerVector[j]);
		            			}
		            			else
		            			{
		            		    	returnVector.push_back(boilerVector[j]);
		            		    	returnVector.push_back(boilerVector[i]);
		            			}
	
		            			Scalar color(255,0,0);
		            			cv::drawContours(img, returnVector, -1, color, 1);
		            			std::cout << "found boiler: " << std::endl;
		            			std::cout << "target one center : " << tempOne.getCenter() << std::endl;
		            			std::cout << "target two center : " << tempTwo.getCenter() << std::endl;
		            			return returnVector;
				
					}
				}	
			}	
        }
    }
    else
    {
        std::cout << "filtercontour: before gear detection " << std::endl;
        for(int j = 0; j < contours.size(); j++)
        {
            approxPolyDP(contours[j], outputContour, (cv::arcLength(cv::Mat(contours.at(j)), true) * 0.02), true);
            if (contourArea(outputContour) > 100 && outputContour.size() == 4)
            {
                double maxCosine = 0;
                for(int j = 2; j <=4; j++)
                {
                    double cosine;
                    try {
                        cosine = fabs(cos(angle(outputContour.at(j%4), outputContour.at(j-2), outputContour.at(j-1))));
                    }
                    catch(std::exception e) {
                        std:: cout << e.what();
                    }
                    maxCosine = MAX(maxCosine, cosine);
                }
                //filters out contours that don't have only 90deg anlges
                bool duplicate = false;
                if(maxCosine < .5)
                {
                    Target tempTwo(outputContour);
                    for(int i = 0; i < gearVector.size(); i++)
                    {
                        Target tempOne(gearVector[i]);
                        if(abs(tempOne.getCenter().x - tempTwo.getCenter().x) < 7 ) //add area check
                        {
                            duplicate = true;
                        }

                    }

                    if(duplicate == false && (tempTwo.getWidth() < tempTwo.getHeight() ))
                    {
						if(tempTwo.getHeight()/tempTwo.getWidth() < 3.3 && tempTwo.getHeight()/tempTwo.getWidth() > 1.7)
                        	gearVector.push_back(outputContour);
                    }
                }
            }
        }

       // Scalar color(255,0,0);
      //  cv::drawContours(img, gearVector, -1, color, 10);
        for(int j = 0; j < gearVector.size(); j++)
        {
            Target temp(gearVector[j]);
            std::cout << "center of gear vector index " << j << " = " << temp.getCenter() << std::endl;
        }

        std::cout << "before for loop gear Vector size : " << gearVector.size() << std::endl;
        double minVal = 10000000;
        int minI = 0;
        int minK = 1;
        for( int i = 0; i < gearVector.size(); i++)
        {
            std::cout << "new i : " << i << std::endl;
            std::cout << " gearVector size " << gearVector.size() << std::endl;
            Target* tempOne = new Target(gearVector[i]);
            for(int k = i+1; k < gearVector.size(); k++)
            {
                std::cout << "i : " << i << "k : " << k << std::endl;
                Target* tempTwo = new Target(gearVector[k]);

                double val = cv::matchShapes(gearVector[i], gearVector[k], CV_CONTOURS_MATCH_I1, 0);
                std::cout << " val: " << val << std::endl;

                if(val < minVal)
                {
                    minVal = val;
                    minI = i;
                    minK = k;
                }
                delete tempTwo;
            }
            delete tempOne;
        }

        if(gearVector.size() > 1)
        {
            std::cout << "min Val: " << minVal << std::endl;
            Target* tempOne = new Target(gearVector[minI]);
            Target* tempTwo = new Target(gearVector[minK]);
            if(minVal < 0.6 && abs(tempTwo->getCenter().y - tempOne->getCenter().y) < 40)
            {
                std::vector<std::vector<cv::Point> > returnVector;

                if(tempOne->getCenter().x < tempTwo->getCenter().x)
                {
                    returnVector.push_back(gearVector[minI]);
                    returnVector.push_back(gearVector[minK]);
                }
                else
                {
                    returnVector.push_back(gearVector[minK]);
                    returnVector.push_back(gearVector[minI]);
                }

                Scalar color(255,0,0);
                cv::drawContours(img, returnVector, -1, color, 10);
                std::cout << "found gear: " << std::endl;
                std::cout << "target one center : " << tempOne->getCenter() << std::endl;
                std::cout << "target two center : " << tempTwo->getCenter() << std::endl;
                delete tempOne;
                delete tempTwo;
                return returnVector;
            }
        }
    }
	/*	if(gearVector.size() > 0)
        {
			for(int i = 0; i < gearVector.size(); i++)
			{
                std::cout << "i : " << i << std::endl;
				Target tempOne(gearVector[i]);
				for(int j = i+1; j < gearVector.size(); j++)
				{
                    std::cout << "j: " << j << std::endl;
					Target tempTwo(gearVector[j]);

					if(abs(tempOne.getCenter().y - tempTwo.getCenter().y) < 27 && abs(tempOne.getHeight()-tempTwo.getHeight()) < 35 )
					{
						
                    		    std::cout << "passed the center checks" << std::endl;
								std::vector<std::vector<cv::Point> > returnVector;
                    	    
                				if(tempOne.getCenter().x < tempTwo.getCenter().x)
                				{
		            		    	returnVector.push_back(gearVector[i]);
		            		    	returnVector.push_back(gearVector[j]);
		            			}
		            			else
		            			{
		            		    	returnVector.push_back(gearVector[j]);
		            		    	returnVector.push_back(gearVector[i]);
		            			}
	
		            			Scalar color(255,0,0);
		            			cv::drawContours(img, returnVector, -1, color, 5);
		            			std::cout << "found boiler: " << std::endl;
		            			std::cout << "target one center : " << tempOne.getCenter() << std::endl;
		            			std::cout << "target two center : " << tempTwo.getCenter() << std::endl;
		            			return returnVector;
				
					}
				}	
			}
        }
	}*/
    std::cout << "filtercontour: before return an empty vector "<<  std::endl;
    return std::vector<std::vector<cv::Point> >();
}



/*Scalar color(255,0,0);
Scalar color2(0,0,255);
cv::drawContours(img, gearVector, -1, color, 10);
cv::drawContours(img, boilerVector, -1, color2, 10);
*/

