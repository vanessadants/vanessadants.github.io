#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>
#define R_inicial 5
#define JITTER 3
#define STEP 10

using namespace std;
using namespace cv;

//esses valores serão modificados conforme aumentarmos os limiares T1 e T2 
double RAIO=R_inicial;

Mat image, points;

int width, height;

void pontilhismo(){
  int x,y,gray;
  vector<int> yrange;
  vector<int> xrange;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);
    
  iota(xrange.begin(), xrange.end(), 0); 
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());
  
  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }
  
  
}


void canny(){
  int x,y,gray;
  Mat border;
  int i,nIteracoes=10;
  int t2=1;

  for(i=0;i<nIteracoes;i++){
    Canny(image, border, t2, 3*t2);
    t2+=10;

    //refinar pointilhismo
    //para os pontos da borda desenhar circulos menores
    for(x=0;x<height;x++){
      for(y=0;y<width;y++){
        if(border.at<uchar>(x,y)==255){
          gray = image.at<uchar>(x,y); 
          circle(points,cv::Point(y,x),RAIO,CV_RGB(gray,gray,gray),-1,CV_AA);
        }
      }
    }
    //diminuir o raio para ponto menor
    RAIO=int(RAIO-R_inicial/(1.0*nIteracoes));

  }
  
}


int main(int argc, char** argv){
  Mat copiaOriginal,copiaRefinada;
  // guarda tecla capturada
  char key;
  //determinar se quer refinar pontilhismo ou nao
  bool troca=1;
  //carregar imagem
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  srand(time(0));
  
  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }


  //altura e largura da imagem
  width=image.size().width;
  height=image.size().height;

  //prepara a janela para exibição da imagem
  namedWindow("Pointilhismo", 1);
  //construção da imagem pointilhista básica
  pontilhismo();
  //guarda copia
  points.copyTo(copiaOriginal);

  //utilizar canny para refinar algoritmo
  canny();
  points.copyTo(copiaRefinada);

  while(1){

    key = (char) waitKey(10);
    if( key == 27 ) break; // esc pressed!
    if(key=='t'){
      if(troca)
        copiaRefinada.copyTo(points);
      else
        copiaOriginal.copyTo(points);
      
      troca=!troca;
    }
    //resultados
    imshow("Pointilhismo", points);
  }

  imwrite("pontos.jpg", points);
  return 0;
}
