#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;//matriz de pixels da imagem
  uchar aux;
  int altura, largura;// altura e largura da imagem
  


  image= imread("vanessa.png",CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data)
    cout << "nao abriu vanessa.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  altura=image.rows;
  largura=image.cols;


  //exibir a imagem antes de qualquer modificação
  imshow("janela", image);  
  waitKey();


  
  //para a primeira metade (i<altura/2 && j<largura/2) ||(i>=altura/2 && j>=largura/2)
  //para inverter horizontalmente a coluna 0 recebe a col largura/2, a 1 recebe largura/2+1 e vice versa
  //para inverter verticalmente a linha 0 recebe a lin altura/2, a 1 recebe altura/2+1 e vice versa

  for(int i=0;i<altura/2;i++){
    for(int j=0;j<largura/2;j++){
      aux=image.at<uchar>(i,j);
      image.at<uchar>(i,j)=image.at<uchar>(i+altura/2,j+largura/2);
      image.at<uchar>(i+altura/2,j+largura/2)=aux;

    }
  }

  //para a segunda metade (i<altura/2 && j>=largura/2) || (i>=altura/2 && j<largura/2)
  //para inverter horizontalmente a coluna 0 recebe a col largura/2, a 1 recebe largura/2+1 e vice versa
  //para inverter verticalmente a linha altura/2 recebe a lin 0, a altura/2+1 recebe a lin 1 e vice versa


  for(int i=altura/2;i<altura;i++){
    for(int j=0;j<largura/2;j++){
      aux=image.at<uchar>(i,j);
      image.at<uchar>(i,j)=image.at<uchar>(i-altura/2,j+largura/2);
      image.at<uchar>(i-altura/2,j+largura/2)=aux;

    }
  }
  
  //imagem invertida diagonalmente  
  imshow("janela", image);  
  waitKey();

  return 0;
}
