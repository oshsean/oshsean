#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "std_msgs/Int32.h"


using namespace cv;

static const std::string OPENCV_WINDOW = "Camera window";
std::vector<cv::Point2f> mc(8);
std::vector<std::vector<cv::Point>> contours;
int run_flag = 0;
int player_num = 0;
int player_turn = 0;

void playerCallback(const std_msgs::Int32::ConstPtr& turn)
  {
    ROS_INFO("jaspers thing: [%d]", turn->data);
    if (player_turn % 2 == 0) {
      player_num = 1;
    } else {
      player_num = 0;
    }
    player_turn++;

	  return;
  }

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  ros::Subscriber player_turn_sub_;
  ros::Publisher coord_pub_;
  

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/image_raw", 1,
      &ImageConverter::imageCb, this);
    player_turn_sub_ = nh_.subscribe("talker", 10, playerCallback);
    coord_pub_ = nh_.advertise<std_msgs::Int32MultiArray>("coordArray", 100);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    std_msgs::Int32MultiArray coord_array;

    cv::Scalar player_colour0 = cv::Scalar(0, 0, 0);
    cv::Scalar player_colour1 = cv::Scalar(0, 0, 0);
    if (player_num == 0){
      player_colour0 = cv::Scalar(0, 0, 230);
      player_colour1 = cv::Scalar(10, 10, 255);
    } else {
      player_colour0 = cv::Scalar(0, 230, 230);
      player_colour1 = cv::Scalar(10, 255, 255);
    }
    

    cv::Mat thresh, cornerstone, close_corner, canny1, close_image, open_image, erode_image, canny_output;
    
    std::vector<cv::Vec4i> hierarchy;

    // Input Quadilateral or Image plane coordinates
    Point2f inputQuad[4]; 
    // Output Quadilateral or World plane coordinates
    Point2f outputQuad[4];
        
    // Lambda Matrix
    Mat lambda( 2, 4, CV_32FC1 );
    //Input and Output Image;
    Mat input, output;
    
    //Load the image
    input = cv_ptr->image;
    // Set the lambda matrix the same type and size as input
    lambda = Mat::zeros( input.rows, input.cols, input.type() );

    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    // These four pts are the sides of the rect box used as input 

    cv::inRange(cv_ptr->image, cv::Scalar(230, 0, 0), cv::Scalar(255, 10, 10), cornerstone);
    double morph_size = 3;
    cv::Mat element = getStructuringElement(0, cv::Size(2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );
    cv::morphologyEx(cornerstone, close_corner, cv::MORPH_CLOSE, element);
    cv::Canny(close_corner, canny1, 50, 150, 3);
    
    
    
    // 
    if (run_flag == 0) {
      cv::findContours(canny1, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
      // for (int i = 0; i < contours.size(); i++) {
      //       std::cout<<contours[i]<<std::endl;
      //   }
      std::cout<<contours[6][0]<<contours[4][5]<<contours[0][4]<<contours[2][2]<<std::endl;

      // std::vector<cv::Moments> mu(contours.size());
      //   for (int i = 0; i < contours.size(); i++) {
      //       mu[i] = moments(contours[i], false);
      //   }
        
      //   for (int i = 0; i < contours.size(); i++){
      //       mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
      //   }
      // std::cout<<mc<<std::endl;
      run_flag = 1;
    }
    inputQuad[0] = contours[6][0];
    inputQuad[1] = contours[4][5];
    inputQuad[2] = contours[0][4];
    inputQuad[3] = contours[2][2];


    // inputQuad[0] = Point2f( 0, 0 );
    // inputQuad[1] = Point2f( 500, 0);
    // inputQuad[2] = Point2f( 500, 500);
    // inputQuad[3] = Point2f( 0, 500);  
    // The 4 points where the mapping is to be done , from top-left in clockwise order
    outputQuad[0] = Point2f( 0,0 );
    outputQuad[1] = Point2f( input.cols-1,0);
    outputQuad[2] = Point2f( input.cols-1,input.rows-1);
    outputQuad[3] = Point2f( 0,input.rows-1  );

    // Get the Perspective Transform Matrix i.e. lambda 
    lambda = getPerspectiveTransform( inputQuad, outputQuad );
    // Apply the Perspective Transform just found to the src image
    warpPerspective(input,output,lambda,output.size() );

    //Display input and output
    // imshow("Input",input);
    // imshow("Output",output);

    cv::inRange(output, player_colour0, player_colour1, thresh);
    //cv::bitwise_and(cv_ptr->image, thresh, mask);
    morph_size = 3;
    element = getStructuringElement(0, cv::Size(2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );
    cv::morphologyEx(thresh, close_image, cv::MORPH_CLOSE, element);

    // morph_size = 4;
    // element = getStructuringElement(0, cv::Size(2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );
    // cv::morphologyEx(close_image, open_image, cv::MORPH_OPEN, element);

    morph_size = 15;
    element = getStructuringElement(2, cv::Size(2*morph_size + 1, 2*morph_size+1 ), cv::Point( morph_size, morph_size ) );
    cv::erode(close_image, erode_image, element);

    cv::Canny(erode_image, canny_output, 50, 150, 3);
    std::vector<std::vector<cv::Point>> contours1;
    cv::findContours(canny_output, contours1, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0,0));
    
    
    int flag = 1;
    if (contours1.size() > 0 && flag == 1){
      std::vector<cv::Moments> mu(contours1.size());
      for (int i = 0; i < contours1.size(); i++) {
          mu[i] = moments(contours1[i], false);
      }
      std::vector<cv::Point2f> mc(contours1.size());
      for (int i = 0; i < contours1.size(); i++){
          mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
      }
      
      double p_x = mc[10].x;
      double p_y = mc[10].y;
      // double w_x = 0;
      // double w_y = 0;
      
      double g_x = p_x*500/800;
      double g_y = p_y*500/800;
      int pub_x = int (g_x);
      int pub_y = int (g_y);


      std::cout<<"world pos: "<<pub_x<<" "<<pub_y<<std::endl;
      std::cout<<"counter pos: "<<p_x<<" "<<p_y<<std::endl;
      
      
      coord_array.data.clear();
      coord_array.data.push_back(pub_x);
      coord_array.data.push_back(pub_y);
      coord_array.data.push_back(player_turn);


      cv::Mat drawing(canny_output.size(), CV_8UC3, cv::Scalar(255,255,255));

      cv::Scalar colour = cv::Scalar(167, 151, 0);
      for( int i = 0; i < contours1.size(); i++) {
          
          cv::drawContours(drawing, contours1, i, colour, 2, 8, hierarchy, 0, cv::Point());
          cv::circle( drawing, mc[i], 4, colour, -1, 8, 0 );   
      }
      cv::imshow(OPENCV_WINDOW, drawing);
      cv::waitKey(3);
    } else {
      cv::imshow(OPENCV_WINDOW, output);
      cv::waitKey(3);
    }


    // Draw an example circle on the video stream
    //if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      //cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    // Update GUI Window
    

    // Output modified video stream
    coord_pub_.publish(coord_array);
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}