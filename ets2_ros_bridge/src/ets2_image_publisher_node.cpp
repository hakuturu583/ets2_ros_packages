//headers in ros
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

//headers in X11
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xrender.h>

//headers in stl
#include <vector>
#include <iostream>
#include <strstream>

//headers in OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void getImageFromDisplay(std::vector<uint8_t>& Pixels, int& Width, int& Height, int& BitsPerPixel,XID xid)
{
  Display* display = XOpenDisplay(nullptr);
  //Window root = DefaultRootWindow(display);

  XCompositeRedirectWindow(display, xid, CompositeRedirectAutomatic);

  XWindowAttributes attributes = {0};
  XGetWindowAttributes(display, xid, &attributes);

  Width = attributes.width;
  Height = attributes.height;

  XImage* img = XGetImage(display, xid, 0, 0 , Width, Height, AllPlanes, ZPixmap);
  BitsPerPixel = img->bits_per_pixel;
  Pixels.resize(Width * Height * 4);

  memcpy(&Pixels[0], img->data, Pixels.size());

  XDestroyImage(img);
  XCloseDisplay(display);
}

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "ets2_image_publisher_node");
  ros::NodeHandle nh("~");
  image_transport::ImageTransport it(nh);
  image_transport::Publisher image_pub = it.advertise(ros::this_node::getName()+"/image", 10);
  int Width = 1024;
  int Height = 768;
  int Bpp = 0;
  std::vector<std::uint8_t> Pixels;
  ros::Rate rate(10);
  std::string window_id;
  nh.getParam(ros::this_node::getName()+"/window_id", window_id);
  ROS_INFO_STREAM("window_id = " << window_id);
  int window_id_int;
  sscanf(window_id.c_str(), "%x", &window_id_int);
  XID Xid = window_id_int;
  while(ros::ok())
  {
    getImageFromDisplay(Pixels, Width, Height, Bpp, Xid);
    cv::Mat img = cv::Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]);
    //std::vector<uchar> buf;
    //cv::imencode(".jpg", img, buf);
    //img = cv::imdecode(cv::Mat(buf), 1);
    cv::cvtColor(img, img, CV_RGBA2RGB);
    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();
    image_pub.publish(msg);
    rate.sleep();
  }
  return 0;
}
