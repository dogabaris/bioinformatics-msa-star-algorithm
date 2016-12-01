#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
using namespace std;
typedef pair<int, pair<string, string> > alignments;
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
      string direction="";
    }
};

pair<int, pair<string, string> > global_alignment(string , string,int,int,int);
ofstream output;

int main() {
  ifstream oku;
  oku.open("input.txt");
  string satir;
  int match,mismatch,gap;//hep highroad seçilecek
  int sayac=0,dnaSayaci=0;
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

  cout<<dna.size()<<endl;

  int tempScore=0;

  for(int ilk=0;ilk < dna.size();ilk++){
    for (int ikinci= 0; ikinci < dna.size(); ikinci++) {
      if(ilk==ikinci)
        continue;
      else{
        alignments temp = global_alignment(dna.at(ilk),dna.at(ikinci),match,mismatch,gap);
        tempScore+=temp.first;

        if(ikinci==dna.size()-1){
          dnaSkor.push_back(tempScore);
          tempScore=0;
        }


        cout<<temp.second.first<<"\n"<< temp.second.second<<endl<<endl;
      }
    }
  }

  for (int i = 0; i < dnaSkor.size(); i++) {
    cout<<dnaSkor.at(i)<<endl;
  }

  cout << "The largest element is " << *max_element(dnaSkor.begin(),dnaSkor.end()) << '\n';
  int position= find(dnaSkor.begin(), dnaSkor.end(), *max_element(dnaSkor.begin(),dnaSkor.end()))-dnaSkor.begin();
  cout<<position<<endl;

  return 0;
}

pair<int, pair<string, string> > global_alignment(string dna1, string dna2,int match,int mismatch,int gap){
  vector<vector<int> > matrix(dna2.size()+2, std::vector<int>(dna1.size()+2, 0));

  yon yonler[dna2.size()+2][dna1.size()+2] = {yon()};

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


  output.open("output.txt", ofstream::out | ofstream::app);

  output << "Matris:"<< endl;
  for(int i = 0; i < dna2.size()+2; i++){//matrisi çizdiriyor
        for(int j = 0; j < dna1.size()+2; j++){

            output << setw(3) << right << matrix[i][j];
        }
        output << endl;
  }

  int rowcount = (sizeof(yonler) / sizeof(*yonler));
  int columncount = (sizeof(*yonler) / sizeof(**yonler));

  output << "Yönler:"<< endl;
  for(int i = 0; i < rowcount; i++){//yonleri çizdiriyor
        for(int j = 0; j < columncount; j++){

            output << setw(3) << left << yonler[i][j].direction ;

        }
        output << endl;
  }

  bool flagDiag=false,flagUp=false,flagRight=false;
  int intDiag=0,intUp=0,intRight=0,largest=0,siradakiRow=rowcount-1,siradakiColumn=columncount-1,score=matrix[dna2.size()+1][dna1.size()+1];
  string alignmentDna1,alignmentDna2;


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


            //output << siradakiColumn << "\t" <<siradakiRow << "\t" << alignmentDna1 << "\t" << alignmentDna2 << "\t" << score << "\t" << flagDiag << "\t" << flagUp << "\t" << flagRight << "\t" << intDiag << "\t" << intUp  << "\t" << intRight << endl;

            flagDiag=false;
            flagRight=false;
            flagUp=false;

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
                output << alignmentDna1 << " " << alignmentDna2 <<endl;
                break;
            }


        }

        std::cout << score << std::endl;
        output<< "score: "<< score <<endl << "\n";
        output<< "-------------------------------------------------------\n";
        output.close();

        return {score, {alignmentDna1,alignmentDna2}};
}
