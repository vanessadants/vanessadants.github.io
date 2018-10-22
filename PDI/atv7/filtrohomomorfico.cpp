#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



const int MAX=10;
int dft_M, dft_N;

double C,D0,gammaH,gammaL;

int highFrequency_slider = 20;
int highFrequency_slider_max = 50;

int lowFrequency_slider = 2;
int lowFrequency_slider_max = 10;

int smoothness_slider = 1;
int smoothness_slider_max = 100;

int cutoff_slider = 5;
int cutoff_slider_max = 200;

Mat complexImage, tmp,imageFiltered;
Mat padded, filter;
Mat image; 
Mat_<float> realInput, zeros;
vector<Mat> planos;

char TrackbarName[50];



// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
  Mat aux, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(aux);  D.copyTo(A);  aux.copyTo(D);

  // C <-> B
  C.copyTo(aux);  B.copyTo(C);  aux.copyTo(B);
}


void homomorphicFiltering(){

  // a função de transferência (filtro frequencial) deve ter o
  // mesmo tamanho e tipo da matriz complexa
  filter = Mat(padded.size(), CV_32FC2, Scalar(0));

  // cria uma matriz temporária para criar as componentes real
  // e imaginaria do filtro ideal
  tmp = Mat(dft_M, dft_N, CV_32F);

  // prepara o filtro passa-baixas ideal
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      tmp.at<float>(i,j)= (gammaH-gammaL)*(1-expf((-1.0*C*(pow(i-dft_M/2.0,2)+pow(j-dft_N/2.0,2)))/(pow(D0,2))))+gammaL;
    }
  }

  // cria a matriz com as componentes do filtro e junta
  // ambas em uma matriz multicanal complexa
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);

}

void applyFilter(void){
  
  //limpa os planos
  planos.clear();

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  // cria a compoente real  
  realInput = Mat_<float>(padded);
  //realInput += Scalar::all(1);
  //log(realInput,realInput);

  // insere as duas componentes no array de matrizes
  planos.push_back(realInput);
  planos.push_back(zeros);

  // combina o array de matrizes em uma unica
  // componente complexa
  merge(planos, complexImage);

  // calcula o dft
  dft(complexImage, complexImage);

  // realiza a troca de quadrantes
  deslocaDFT(complexImage);

  //redimensiona e normaliza para remoção de bordas
  resize(complexImage,complexImage,padded.size());
  normalize(complexImage,complexImage,0,1,CV_MINMAX);

  //calcula o filtro homomorfico
  homomorphicFiltering();

  // aplica o filtro frequencial
  mulSpectrums(complexImage,filter,complexImage,0);

  // troca novamente os quadrantes
  deslocaDFT(complexImage);

  // calcula a DFT inversa
  idft(complexImage, complexImage);

  // limpa o array de planos
  planos.clear();

  // separa as partes real e imaginaria da
  // imagem filtrada
  split(complexImage, planos);
  //exp(planos[0],planos[0]);

  // normaliza a parte real para exibicao
  normalize(planos[0], planos[0], 0, 1, CV_MINMAX);

  //imagem filtrada recebe o planos[0]
  imageFiltered = planos[0].clone();

  printf("D0: %f\n", D0);
  printf("C: %f\n", C);
  printf("Alta Frequencia: %f\n", gammaH);
  printf("Baixa Frequencia: %f\n", gammaL);
    
  imshow("Homomorphic filtering", imageFiltered);
  imshow("Original", image);


}

void on_trackbar_highFrequency(int, void*){
  gammaH =  highFrequency_slider/MAX;
  applyFilter();
}


void on_trackbar_lowFrequency(int, void*){
  gammaL =  lowFrequency_slider/MAX;
  applyFilter();
}


void on_trackbar_smoothness(int, void*){
  C =  smoothness_slider;
  applyFilter();
}


void on_trackbar_cutoff(int, void*){
  D0 =  cutoff_slider;
  applyFilter();
}


int main(int argvc, char** argv){
  
  //carrega a imagem passada como argumento
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

   // identifica os tamanhos otimos para
  // calculo do FFT
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  // realiza o padding da imagem
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  //prepara a janela para exibição da imagem
  namedWindow("Homomorphic filtering", 1);
  
  //setar HighFrequency (reflectância)
  sprintf( TrackbarName, "GammaH->High Frequency: " );
  createTrackbar( TrackbarName, "Homomorphic filtering",
          &highFrequency_slider,
          highFrequency_slider_max,
          on_trackbar_highFrequency );
  on_trackbar_highFrequency(highFrequency_slider, 0 );
  
  //setar LowFrequency (iluminância)
  sprintf( TrackbarName, "GammaL->Low Frequency: " );
  createTrackbar( TrackbarName, "Homomorphic filtering",
          &lowFrequency_slider,
          lowFrequency_slider_max,
          on_trackbar_lowFrequency );
  on_trackbar_lowFrequency(lowFrequency_slider, 0 );

  //setar smoothness (Suavidade da curva do filtro)
  sprintf( TrackbarName, "C->Smoothness of the curve: " );
  createTrackbar( TrackbarName, "Homomorphic filtering",
          &smoothness_slider,
          smoothness_slider_max,
          on_trackbar_smoothness );
  on_trackbar_smoothness(smoothness_slider, 0 );  

  //setar cutoff (D0-> frequência de corte -> raio do filtro)
  sprintf( TrackbarName, "D0->Cutoff Frequency: " );
  createTrackbar( TrackbarName, "Homomorphic filtering",
          &cutoff_slider,
          cutoff_slider_max,
          on_trackbar_cutoff );
  on_trackbar_cutoff(cutoff_slider, 0 );  

  applyFilter();;
  waitKey(0);
  return 0;
}
