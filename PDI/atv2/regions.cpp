#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;//matriz de pixels da imagem

  int altura, largura;// altura e largura da imagem
  Vec2i P1, P2; //pontos que o usuário vai informar e servirão para delimitar o retângulo


  image= imread("vanessa.png",CV_LOAD_IMAGE_GRAYSCALE); //carregar a imagem 780 x 1028

  if(!image.data)
    cout << "nao abriu vanessa.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  altura=image.rows;
  largura=image.cols;

  //pedir os pontos ao usuário (ponto Pi(x,y))
  do{
    cout<<"Insira as coordenadas do ponto P1 (x,y):\n";
    cin>>P1[0]>>P1[1];

  }while(P1[0]<0 || P1[0]>=altura || P1[1]<0 || P1[1]>=largura);

  do{
    cout<<"Insira as coordenadas do ponto P2 (x,y):\n";
    cin>>P2[0]>>P2[1];
    
  }while(P2[0]<0 || P2[0]>=altura || P2[1]<0 || P2[1]>=largura);


  //exibir a imagem antes de qualquer modificação
  imshow("janela", image);  
  waitKey();


  //Fazer a região definida por esses pontos ficar em negativo
  int li,lf,ci,cf; //servirá para limitar o retângulo
  li=(P1[0]<=P2[0]?P1[0]:P2[0]);
  lf=(P1[0]>P2[0]?P1[0]:P2[0]);

  ci=(P1[1]<=P2[1]?P1[1]:P2[1]);
  cf=(P1[1]>P2[1]?P1[1]:P2[1]);


  for(int i=li;i<lf;i++){
    for(int j=ci;j<cf;j++){
      image.at<uchar>(i,j)=255-image.at<uchar>(i,j); //negativo da imagem
    }
  }
  
  //imagem com a região retangular definida por P1 e P2 negativa  
  imshow("janela", image);  
  waitKey();

  return 0;
}
