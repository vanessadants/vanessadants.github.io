#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool temBolha(Mat image,int i,int j){
  
  int l,c;
  CvPoint p;
  Mat aux=image.clone(); //não queremos sobrescrescer a imagem principal

  p.x=0;
  p.y=0;
  floodFill(aux,p,255); //pintamos o fundo de branco
  
  while(aux.at<uchar>(i,j)!=255){
    //expandir em 8 direções
    //cima
    l=i-1;
    c=j;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      l--;
    }
    //baixo
    l=i+1;
    c=j;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      l++;
    }
    //esquerda
    l=i;
    c=j-1;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      c--;
    }
    //direita
    l=i;
    c=j+1;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      c++;
    }

    //lateral superior esquerda
    l=i-1;
    c=j-1;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      l--;
      c--;
    }
    //lateral superior direita
    l=i-1;
    c=j+1;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      l--;
      c++;
    }
    //lateral inferior esquerda
    l=i+1;
    c=j-1;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      l++;
      c--;
    }
    //lateral inferior direita
    l=i+1;
    c=j+1;
    while(aux.at<uchar>(l,c)!=255){
      if(aux.at<uchar>(l,c)==0) //achamos uma região preta
        return true;
      l++;
      c++;
    }
    i++;
    j++;
  }

  return false;
  
}


int main(int argc, char** argv){
  Mat image;
  int width, height;
  int nobjects, nbolhas;
  int i,j;

  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;


  //despreza bolhas existentes na borda da imagem
  p.x=0;
  p.y=0;

  // busca objetos com buracos presentes
  for(i=0; i<height; i++){
    if(image.at<uchar>(i,0) == 255){
      p.x=0;
      p.y=i;
      floodFill(image,p,0);
    }
    if(image.at<uchar>(i,width-1) == 255){
      p.x=width-1;
      p.y=i;
      floodFill(image,p,0);
    }
  }

  for(j=0; j<width; j++){
    if(image.at<uchar>(0,j) == 255){
      p.x=j;
      p.y=0;
      floodFill(image,p,0);
    }
    if(image.at<uchar>(height-1,j) == 255){
      p.x=j;
      p.y=height-1;
      floodFill(image,p,0);
    }
  }


  // busca objetos com buracos presentes,lembrar de desconsiderar mais de 1 buraco
  
  p.x=0;
  p.y=0;

  nobjects=0;
  nbolhas=0;
  
  for(i=0; i<height; i++){
    for(j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        // achou um objeto
        nobjects++;
        p.x=j;
        p.y=i;
        floodFill(image,p,nobjects);
        //verificar se tem ao menos 1 bolha
        if(temBolha(image,i,j)){
          nbolhas++;
        }
      }
    }
    
  }//ao fim teremos nobjets presentes, bem como o número de objetos com ao menos 1 bolha

  cout<<"Temos "<<nobjects<<", dos quais "<<nbolhas<<" possuem ao menos 1 bolha.\n";

  //exibir e salvar os resultados
  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}
