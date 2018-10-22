#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



double d=0;
int l1=0,l2=0,h=0;
int width,height;
int centro=0;

int d_slider = 0;
int d_slider_max = 100;

int center_slider = 0;
int center_slider_max = 100;

int height_slider = 0;
int height_slider_max = 100;

VideoCapture cap;

Mat image;
Mat image_blurred;
Mat ganhoImage;
Mat ganhoImage_blurred;
Mat imageTiltShift;

char TrackbarName[50];

void makeBlurredImage(){
  Mat result, result1;
  int absolut=1;
  image.copyTo(result);
  
  float media[] = {1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1,
           1,1,1,1,1,1,1,1,1};
  Mat mask(9,9,CV_32F,media),mask1;

  mask = Mat(9, 9, CV_32F, media);
  scaleAdd(mask, 1/81.0, Mat::zeros(9,9,CV_32F), mask1);
  mask = mask1;

  result.convertTo(result1, CV_32F);
  filter2D(result1, image_blurred, result1.depth(), mask, Point(1,1), 0);
  if(absolut){
      image_blurred=abs(image_blurred);
  }
  image_blurred.convertTo(image_blurred, CV_8U);
  

}

void blend(){
  int i,j;
  image.copyTo(ganhoImage);
  image.copyTo(ganhoImage_blurred);
  image.copyTo(imageTiltShift);

  //ganho da imagem de entrada
  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      ganhoImage.at<uchar>(i,j)= 255*(1/2.0)*(tanh((i-l1)/(d*1.0))-tanh((i-l2)/(d*1.0)));
    }
  }

  //ganho da imagem borrada
  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      ganhoImage_blurred.at<uchar>(i,j)= 255- ganhoImage.at<uchar>(i,j);      
    }
  }

  //gerar imagem final como combinação
  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      imageTiltShift.at<uchar>(i,j)= (ganhoImage.at<uchar>(i,j)/255.0)*image.at<uchar>(i,j)+(ganhoImage_blurred.at<uchar>(i,j)/255.0)*image_blurred.at<uchar>(i,j);

    }
  }
}


void on_trackbar_height(int, void*){
  h =  height_slider*height/100;
  l1 = centro - h/2;
  l2 = centro + h/2;

  if(l1<0){
    l1=0;
  } 
  if(l2>height){
    l2=height;
  }

}

void on_trackbar_center(int, void*){
  centro=center_slider*height/100;
  l1 = centro - h/2;
  l2 = centro + h/2;

  if(l1<0){
    l1=0;
  } 
  if(l2>height){
    l2=height;
  }
}

void on_trackbar_decaimento(int, void*){
  //gerar as imagens para combinação a posterior das duas imagens
  d= d_slider;
}



int main(int argvc, char** argv){

  cap.open(0); 
  if(!cap.isOpened()) 
    return -1;
  width=cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  namedWindow("tiltshift", 1);

  while(1){
    cap >> image;  
    cvtColor(image, image, CV_BGR2GRAY);
    //criar a imagem borrada
    makeBlurredImage();
    
    //setar a altura
    sprintf( TrackbarName, "Altura: " );
    createTrackbar( TrackbarName, "tiltshift",
            &height_slider,
            height_slider_max,
            on_trackbar_height );
    on_trackbar_height(height_slider, 0 );
    
    //definir cetro do foco e calcular l1 e l2  
    sprintf( TrackbarName, "Centro: ");
    createTrackbar( TrackbarName, "tiltshift",
  				  &center_slider,
  				  center_slider_max,
  				  on_trackbar_center );
    on_trackbar_center(center_slider, 0 );

    //calcula decaimento e junta as 2 imagens
    sprintf( TrackbarName, "Decaimento: " );
    createTrackbar( TrackbarName, "tiltshift",
            &d_slider,
            d_slider_max,
            on_trackbar_decaimento );

    on_trackbar_decaimento(d_slider, 0 );
    blend();
    imshow("tiltshift",imageTiltShift);
    waitKey(1);

  }
  //waitKey(0);
  return 0;
}
