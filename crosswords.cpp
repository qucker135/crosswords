#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

enum RC{ROW = true, COLUMN = false};

class Record{
	const int bx,by;
	const bool Row;
	const int length;
	string content;
public:
	Record(int bx,int by,bool R,int l,string c):bx(bx),by(by),Row(R),length(l){
		this->content = c;
	}
	string getContent(){return content;}
	void setContent(string c){this->content = c;}

	bool getRow(){return Row;}
	int getLength(){return length;}
	int getX(){return bx;}
	int getY(){return by;}
};

class Rules{
	const int a,b,c,d;
public:
	Rules(int a,int b,int c,int d):a(a),b(b),c(c),d(d){}
	int ga(){return a;}
	int gb(){return b;}
	int gc(){return c;}
	int gd(){return d;}
};

//crossword scheme
//#.#.#
//.....
//#....
//##...
//.....

const int rozmiarX = 5, rozmiarY = 5;

void wypisz(const int rozmiarX,const int rozmiarY,vector<Record>& rl){
	for(int i=0;i<rozmiarX;i++){
		for(int j=0;j<rozmiarY;j++){
			bool czyOdpowiada = false;
			for(vector<Record>::iterator it = rl.begin(); it!=rl.end(); it++){
				switch(it->getRow()){
					case ROW:
						if(!czyOdpowiada && i==it->getX() && j>=it->getY() && j<it->getY()+it->getLength()){
							cout<<(it->getContent())[j-it->getY()];
							czyOdpowiada = true;
						}
					break;
					case COLUMN:
						if(!czyOdpowiada && j==it->getY() && i>=it->getX() && i<it->getX()+it->getLength())
						{
							cout<<(it->getContent())[i-it->getX()];
							czyOdpowiada = true;
						}
					break;
				}
			}
			if(!czyOdpowiada) cout<<"#";
		}
		cout<<"\n";
	}
}

void gen(vector<Record>& rl,vector<Rules> rlz,int rozmiarX,int rozmiarY,int poziom=0){
	if(rl[rl.size()-1].getContent()!=""){
		wypisz(rozmiarX,rozmiarY,rl);
	}
	else{	
		//znajdz pierwszy pusty element w liscie
		/*vector<Record>::iterator it = rl.begin();
		while(it->getContent()!=""){it++;}*/
		int q = 0;
		while(rl[q].getContent()!=""){q++;}
		//it wskazuje teraz na pierwszy pusty element
		//otworz odpowiedni slownik
		//int len = it->getLength();
		int len = rl[q].getLength();
		ifstream slownik(to_string(len)+".txt");
		if(! slownik.is_open()) cerr<<"Blad otwarcia slownika!"<<endl;
		else{
			int debugInfo = 0;
			string bfr = "";
			while(slownik>>bfr){
				cerr<<"Poziom: "<<poziom<<endl;
				cerr<<"Dbg: "<<debugInfo<<endl;
				bool czySpelnia = true;
				for(vector<Rules>::iterator itz = rlz.begin(); itz!=rlz.end(); itz++){
					int w1 = itz->ga();
					int w2 = itz->gc();
				
					int ind1 = itz->gb();
					int ind2 = itz->gd();
				
					//jesli zasada itz dotyczy slowa rl[q]
					if(w1==q){
						if(rl[w2].getContent()!=""){
							if(bfr[ind1]!=rl[w2].getContent()[ind2]) czySpelnia = false;
						}
					}
					else if(w2==q){
						if(rl[w1].getContent()!=""){
							if(bfr[ind2]!=rl[w1].getContent()[ind1]) czySpelnia = false;
						}	
					}
				}

				if(czySpelnia){
					rl[q].setContent(bfr);
					gen(rl,rlz,rozmiarX,rozmiarY,poziom+1);
					rl[q].setContent("");
				}
				debugInfo++;
				
			}
		}
	}
}

int main(){
	vector<Record> recordList;
	//TEST1 - aaa
	/*
	recordList.push_back(Record(1,0,ROW,5,"aaaaa"));	//N1	0
	recordList.push_back(Record(2,1,ROW,4,"aaaa"));	//N2	1
	recordList.push_back(Record(3,2,ROW,3,"aaa"));	//N3	2
	recordList.push_back(Record(4,0,ROW,5,"aaaaa"));	//N4	3
	recordList.push_back(Record(0,1,COLUMN,3,"aaa"));	//Z1	4
	recordList.push_back(Record(1,2,COLUMN,4,"aaaa"));	//Z2	5
	recordList.push_back(Record(0,3,COLUMN,5,"aaaaa"));	//Z3	6
	recordList.push_back(Record(1,4,COLUMN,4,"aaaa"));	//Z4	7
	*/
	//TEST2 - pusto
	/*
	recordList.push_back(Record(1,0,ROW,5,""));	//N1	0
	recordList.push_back(Record(2,1,ROW,4,""));	//N2	1
	recordList.push_back(Record(3,2,ROW,3,""));	//N3	2
	recordList.push_back(Record(4,0,ROW,5,""));	//N4	3
	recordList.push_back(Record(0,1,COLUMN,3,""));	//Z1	4
	recordList.push_back(Record(1,2,COLUMN,4,""));	//Z2	5
	recordList.push_back(Record(0,3,COLUMN,5,""));	//Z3	6
	recordList.push_back(Record(1,4,COLUMN,4,""));	//Z4	7
	*/
	//TEST3 - action! - TEST2, ale zoptymalizowany
	
	recordList.push_back(Record(1,0,ROW,5,""));
	recordList.push_back(Record(0,1,COLUMN,3,""));
	recordList.push_back(Record(1,2,COLUMN,4,""));
	recordList.push_back(Record(0,3,COLUMN,5,""));
	recordList.push_back(Record(1,4,COLUMN,4,""));
	recordList.push_back(Record(2,1,ROW,4,""));
	recordList.push_back(Record(3,2,ROW,3,""));
	recordList.push_back(Record(4,0,ROW,5,""));

	
	vector<Rules> rulesList;
	//TEST1 / TEST2
	/*
	rulesList.push_back(Rules(0,1,4,1));
	rulesList.push_back(Rules(1,0,4,2));
	rulesList.push_back(Rules(0,2,5,0));
	rulesList.push_back(Rules(1,1,5,1));
	rulesList.push_back(Rules(2,0,5,2));
	rulesList.push_back(Rules(3,2,5,3));
	rulesList.push_back(Rules(0,3,6,1));
	rulesList.push_back(Rules(1,2,6,2));
	rulesList.push_back(Rules(2,1,6,3));
	rulesList.push_back(Rules(3,3,6,4));
	rulesList.push_back(Rules(0,4,7,0));
	rulesList.push_back(Rules(1,3,7,1));
	rulesList.push_back(Rules(2,2,7,2));
	rulesList.push_back(Rules(3,4,7,3));
	*/	
	//wypisz(rozmiarX,rozmiarY,recordList);

	//TEST3
	rulesList.push_back(Rules(0,1,1,1));
	rulesList.push_back(Rules(0,2,2,0));
	rulesList.push_back(Rules(0,3,3,1));
	rulesList.push_back(Rules(0,4,4,0));
	rulesList.push_back(Rules(5,0,1,2));
	rulesList.push_back(Rules(5,1,2,1));
	rulesList.push_back(Rules(5,2,3,2));
	rulesList.push_back(Rules(5,3,4,1));
	rulesList.push_back(Rules(6,0,2,2));
	rulesList.push_back(Rules(6,1,3,3));
	rulesList.push_back(Rules(6,2,4,2));
	rulesList.push_back(Rules(7,2,2,3));
	rulesList.push_back(Rules(7,3,3,4));
	rulesList.push_back(Rules(7,4,4,3));
	

	gen(recordList,rulesList,rozmiarX,rozmiarY);

	return 0;
}
