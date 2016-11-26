#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <limits>
using namespace std;

//yollar matrisi
//tablo matrisi
//tablo doldurulurken yollar matrisine olası yollar eklenecek
//sağ en alttan başlanıp olası yollara göre high road low road ve
//diagonal giderken(çapraz) match-mismatch eklenir, sağdan ya da soldan giderken gap penalty eklenir. En büyük değer yazılır.

struct yon {
  char yondiag;
  char yonright;
  char yonup;
  string direction;
    yon(){
      char yondiag='\0';
      char yonright='\0';
      char yonup='\0';
      string direction;
    }
};

int main() {
  ifstream oku;
  oku.open("input.txt");
  string satir,dna1,dna2;
  int match,mismatch,gap,road;
  int sayac=0;
  vector<vector<int> > matrix;
  //vector<vector<yon>> yonler;


  if (oku.is_open())
    {
      while ( getline(oku, satir) )
      {
        istringstream ss(satir);

        if(sayac==0){
          ss >> match >> mismatch >> gap >> road;
          cout << match << " " << mismatch << " " << gap << " " << road <<endl;

        }else if(sayac==1){
          ss >> dna1;
          cout<< dna1 <<endl;

        }else if(sayac==2){
          ss >> dna2;
          cout<< dna2 <<endl;
          break;
        }

        sayac++;
      }
      oku.close();
    }
  else{
    cout << "input.txt is missing."<<endl;
  }
  oku.close();



  matrix.resize(dna2.size()+2);//yukseklik dna2 genişlik dna1
    for (int i = 0; i < dna2.size()+2; ++i)
      matrix[i].resize(dna1.size()+1);

  yon yonler[dna2.size()+2][dna1.size()+2];

  matrix[1][1]=0;

  for(int i=2;i<dna1.size()+2;i++){//satır - genişlik
      matrix[0][i]=dna1[i-2];
      matrix[1][i]=matrix[1][i-1]+gap;
  }
  for(int k=2;k<dna2.size()+2;k++){//sütun yükseklik
      matrix[k][0]=dna2[k-2];
      matrix[k][1]=matrix[k-1][1]+gap;
  }
//diagonal gel satir ve sütundan 1 er eksik.üstten columdan 1 eksik.yandan rowdan 1 eksik.
  int tmpdiag,tmpup,tmpright;

  for(int r=2;r<dna2.size()+2;r++){//row ,2
    for(int c=2;c<dna1.size()+2;c++){//column
      if(matrix[0][c]==matrix[r][0])
        tmpdiag=match+matrix[r-1][c-1];

      else
        tmpdiag=mismatch+matrix[r-1][c-1];

        tmpup=gap+matrix[r-1][c];
        tmpright=gap+matrix[r][c-1];

        int biggest = tmpdiag;

        if (tmpdiag <= tmpup){
          biggest = tmpup;

          yonler[r][c].direction+='|';
          yonler[r][c].yonup='|';

        }
        if (biggest <= tmpright){
          biggest = tmpright;
          yonler[r][c].direction+='-';
          yonler[r][c].yonright='-';

        }
        if(biggest==tmpdiag){
          yonler[r][c].direction+='\\';
          yonler[r][c].yondiag='\\';
        }


        matrix[r][c]=biggest;
    }
  }
  ofstream output;
  output.open(("output.txt"));
  output << "Matris:"<< endl;
  for(int i = 0; i < dna2.size()+2; i++){//matrisi çizdiriyor
        for(int j = 0; j < dna1.size()+2; j++){

            output << matrix[i][j] << " ";
        }
        output << endl;
  }
  int rowcount = (sizeof(yonler) / sizeof(*yonler));
  int columncount = (sizeof(*yonler) / sizeof(**yonler));

  output << "Yönler:"<< endl;
  for(int i = 0; i < rowcount; i++){//yonleri çizdiriyor
        for(int j = 0; j < columncount; j++){

            output << "(" << yonler[i][j].direction  << ")"<< " ";

        }
        output << endl;
  }

  bool flagDiag=false,flagUp=false,flagRight=false;
  int intDiag=0,intUp=0,intRight=0,largest,siradakiRow=dna2.size(),siradakiColumn=dna1.size(),score=matrix[dna2.size()][dna1.size()];
  string alignmentDna1,alignmentDna2;


  alignmentDna1+=matrix[0][columncount-1];
  alignmentDna2+=matrix[rowcount-1][0];

  while(1){
          if(road==1){
            if(yonler[siradakiRow][siradakiColumn].yondiag == '\\'){
              flagDiag=true;
              intDiag=matrix[siradakiRow-1][siradakiColumn-1];
            }
            if(yonler[siradakiRow][siradakiColumn].yonup=='|'){
              flagUp=true;
              intUp=matrix[siradakiRow-1][siradakiColumn];
            }
            if(yonler[siradakiRow][siradakiColumn].yonright=='-'){
              flagRight=true;
              intRight=matrix[siradakiRow][siradakiColumn-1];
            }

            //hangi yola gideceği karar verilecek high road veya low roada göre sonra siradakirow ve column ona göre değişecek.

            if(intDiag>=intUp && intDiag>=intRight) {//yollardaki en büyük değer bulunuyor
                largest= intDiag;
            }
            if(intUp>=intDiag && intUp>=intRight) {
                largest= intUp;
            }
            if(intRight>=intDiag && intRight>=intUp) {
                largest= intRight;
            }

            if(flagUp==true && largest==intUp){//yukarı giderken dna2 alınır dna1 yerine - konur
              alignmentDna1+='-';
              alignmentDna2+=matrix[siradakiRow][0];
              siradakiRow=siradakiRow-1;
              score=score+matrix[siradakiRow][siradakiColumn];

            }else if(flagDiag==true && largest==intDiag){//diagonal gidiyoken dna1(satır) dna2(sütun) ikisini de alırız
              alignmentDna1+=matrix[0][siradakiColumn];
              alignmentDna2+=matrix[siradakiRow][0];
              siradakiRow=siradakiRow-1;
              siradakiColumn=siradakiColumn-1;
              score=score+matrix[siradakiRow][siradakiColumn];

            }else if(flagRight==true && largest==intRight){//yana giderken dna1 alınır dna2 yerine - konur
              alignmentDna1+=matrix[0][siradakiColumn];
              alignmentDna2+='-';
              siradakiColumn=siradakiColumn-1;
              score=score+matrix[siradakiRow][siradakiColumn];

            }


            //output << siradakiColumn << " " <<siradakiRow << " " << alignmentDna1 << " " << alignmentDna2 << " " << score << " " << flagDiag << " " << flagUp << " " << flagRight << " " << intDiag << " " << intUp  << " " << intRight << endl;
            flagDiag=false;
            flagRight=false;
            flagUp=false;
            intRight=0;
            intUp=0;
            intDiag=0;

            if(siradakiColumn==1 && siradakiRow==1)
              break;

          }else if(road==0){

            if(yonler[siradakiRow][siradakiColumn].yondiag == '\\'){
              flagDiag=true;
              intDiag=matrix[siradakiRow-1][siradakiColumn-1];
            }
            if(yonler[siradakiRow][siradakiColumn].yonup=='|'){
              flagUp=true;
              intUp=matrix[siradakiRow-1][siradakiColumn];
            }
            if(yonler[siradakiRow][siradakiColumn].yonright=='-'){
              flagRight=true;
              intRight=matrix[siradakiRow][siradakiColumn-1];
            }

            //hangi yola gideceği karar verilecek high road veya low roada göre sonra siradakirow ve column ona göre değişecek.

            if(intDiag>=intUp && intDiag>=intRight) {//yollardaki en büyük değer bulunuyor
                largest= intDiag;
            }
            if(intUp>=intDiag && intUp>=intRight) {
                largest= intUp;
            }
            if(intRight>=intDiag && intRight>=intUp) {
                largest= intRight;
            }

            if(flagRight==true && largest==intRight){//yana giderken dna1 alınır dna2 yerine - konur
              alignmentDna1+=matrix[0][siradakiColumn];
              alignmentDna2+='-';
              siradakiColumn=siradakiColumn-1;
              score=score+matrix[siradakiRow][siradakiColumn];
            }
            else if(flagDiag==true && largest==intDiag){//diagonal gidiyoken dna1(satır) dna2(sütun) ikisini de alırız
              alignmentDna1+=matrix[0][siradakiColumn];
              alignmentDna2+=matrix[siradakiRow][0];
              siradakiRow=siradakiRow-1;
              siradakiColumn=siradakiColumn-1;
              score=score+matrix[siradakiRow][siradakiColumn];
            }
            else if(flagUp==true && largest==intUp){//yukarı giderken dna2 alınır dna1 yerine - konur
               alignmentDna1+='-';
               alignmentDna2+=matrix[siradakiRow][0];
               siradakiRow=siradakiRow-1;
               score=score+matrix[siradakiRow][siradakiColumn];

              }

            //output << siradakiColumn << " " <<siradakiRow << " " << alignmentDna1 << " " << alignmentDna2 << " " << score << " " << flagDiag << " " << flagUp << " " << flagRight << " " << intDiag << " " << intUp  << " " << intRight << endl;
            flagDiag=false;
            flagRight=false;
            flagUp=false;
            intRight=0;
            intUp=0;
            intDiag=0;

            if(siradakiColumn==1 && siradakiRow==1)
              break;

          }

        }
        reverse(alignmentDna1.begin(), alignmentDna1.end());
        reverse(alignmentDna2.begin(), alignmentDna2.end());
        cout << alignmentDna1 << " " << alignmentDna2;

        output<< "dna1: "<<alignmentDna1<<"\ndna2: "<<alignmentDna2<<endl;
        output<< "score: "<< score <<endl << "\n";
        output.close();
  return 0;
}
