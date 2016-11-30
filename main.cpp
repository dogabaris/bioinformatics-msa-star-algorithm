#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
using namespace std;

//yollar matrisi
//tablo matrisi
//tablo doldurulurken yollar matrisine olası yollar eklenecek
//sağ en alttan başlanıp olası yollara göre high road low road ve
//diagonal giderken(çapraz) match-mismatch eklenir, sağdan ya da soldan giderken gap penalty eklenir. En büyük değer yazılır.

//http://www.site.uottawa.ca/~lucia/courses/5126-11/lecturenotes/12-13MultipleAlignment.pdf mismatch/gap puanlarına göre yeni tablo oluşturur.En az skoru olanı center seçer ona göre bir daha
// hizalar

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
  string satir;
  int match,mismatch,gap;//hep highroad seçilecek
  int sayac=0,dnaSayaci=0;
  vector<vector<int> > matrix;
  vector<string> dna;
  vector<int> dnaSkor;

  if (oku.is_open())
    {
      while ( getline(oku, satir) )
      {
        istringstream ss(satir);

        if(sayac==0){
          ss >> match >> mismatch >> gap;
          cout << match << " " << mismatch << " " << gap << endl;

        }else{
          dna.push_back(ss.str());
          cout<< dna.at(sayac-1) <<" size " << dna.at(sayac-1).size()<<endl;
          dnaSayaci++;
        }

        sayac++;
      }
      oku.close();
    }
  else{
    cout << "input.txt is missing."<<endl;
  }
  oku.close();//okundu

  ofstream output;
  output.open(("output.txt"));
  cout<< "sayac " << dnaSayaci<<endl;
  int score,i,i2;
  int hangiDna=0;


  for(i=0;i<dna.at(hangiDna).size();i++) {

    try{
    for(i2=0;i2<dna.at(i2).size();i2++) {

      try{
        if(i==i2){//aynı dnalara bakıyorsa skor değerlendirmesi yapma
          continue;
        }else{//farklı dnalara bakıyorsa skor bul
          cout<< "dna2 " << dna.at(i2).size() << " dna1 "<< dna.at(i).size() <<endl;
          matrix.resize(dna.at(i2).size()+2);//yukseklik dna.at(i2) genişlik dna.at(i)
            for (int k = 0; k < dna.at(i2).size()+2; k++)
              matrix[k].resize(dna.at(i).size()+1);


          yon yonler[dna.at(i2).size()+2][dna.at(i).size()+2];

          matrix[1][1]=0;

          for(int satiri=2;satiri<dna.at(i).size()+2;satiri++){//satır - genişlik
              matrix[0][satiri]=dna.at(i)[satiri-2];
              matrix[1][satiri]=matrix[1][satiri-1]+gap;
          }
          for(int sutuni=2;sutuni<dna.at(i2).size()+2;sutuni++){//sütun - yükseklik
              matrix[sutuni][0]=dna.at(i2)[sutuni-2];
              matrix[sutuni][1]=matrix[sutuni-1][1]+gap;
          }
        //diagonal gel satir ve sütundan 1 er eksik.üstten columdan 1 eksik.yandan rowdan 1 eksik.
          int tmpdiag,tmpup,tmpright;

          for(int r=2;r<dna.at(i2).size()+2;r++){//row ,2
            for(int c=2;c<dna.at(i).size()+2;c++){//column
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




          output << "Matris:"<< endl;
          for(int r = 0; r < dna.at(i2).size()+2; r++){//matrisi çizdiriyor
                for(int c = 0; c < dna.at(i).size()+2; c++){
                    output << matrix[r][c] << "\t";
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
          int intDiag=0,intUp=0,intRight=0,largest=0,siradakiRow=rowcount-1,siradakiColumn=columncount-1;
          score=matrix[dna.at(i2).size()+1][dna.at(i).size()+1];
          string alignmentDna1,alignmentDna2;

          //cout<<score<<endl;

        while(1){

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

                  //high road siradakirow ve column ona göre değişecek.

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

                  output<< siradakiColumn+1 << "\t" <<siradakiRow+1 << "\t" << alignmentDna1 << "\t" << alignmentDna2 << "\t" << score << "\t" << flagDiag << "\t" << flagUp << "\t" << flagRight << "\t" << intDiag << "\t" << intUp  << "\t" << intRight << endl;


                  flagUp=false;
                  flagDiag=false;
                  flagRight=false;

                  if(yonler[siradakiRow][siradakiColumn].direction!=yonler[2][2].direction){
                    intUp=matrix[siradakiRow][siradakiColumn-1];
                    intDiag=matrix[siradakiRow-1][siradakiColumn-1];
                    intRight=matrix[siradakiRow-1][siradakiColumn];
                  }else{
                    intUp=-100;
                    intDiag=-100;
                    intRight=-100;
                  }

                  if(siradakiColumn<=2 || siradakiRow<=2){
                    alignmentDna1+=matrix[0][2];
                    alignmentDna2+=matrix[2][0];
                    reverse(alignmentDna1.begin(), alignmentDna1.end());
                    reverse(alignmentDna2.begin(), alignmentDna2.end());
                    output<< i<<"\t"<<i2 <<"\t" << alignmentDna1 << " " << alignmentDna2 <<endl;
                    //alignmentDna1.clear();
                    //alignmentDna2.clear();
                    //cout << alignmentDna1 << " " << alignmentDna2<<endl;

                    break;
                  }


              }

              //cout << alignmentDna1 << " " << alignmentDna2<<endl;
              score+=score;
              output<< "dna1: "<<alignmentDna1<<"\ndna2: "<<alignmentDna2<<endl;
              output<< "score: "<< score <<endl << "\n";
            }


            dnaSkor.push_back(score);

          }catch (const std::out_of_range) {
            break;
            }
        }
        //cout << i << '\n';
      }catch (const out_of_range) {
          cout << i << '\n';
          break;
        }
      }

      cout<<"end"<<endl;
      output<< "-------" <<endl;
      output.close();

    return 0;
}
