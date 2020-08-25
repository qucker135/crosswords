#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

enum RC{ROW = true, COLUMN = false};

class Record{
	const int bx,by; //coordinates of beginning
	const bool Row;  //is record a ROW or a COLUMN?
	const int length;
	string content;
public:
	Record(int bx,int by,bool R,int l,string c):bx(bx),by(by),Row(R),length(l){
		content = c;
	}
	string getContent(){return content;}
	void setContent(string c){content = c;}

	bool getRow(){return Row;}
	int getLength(){return length;}
	int getX(){return bx;}
	int getY(){return by;}
};


void show(vector<Record>& rl,const int sizeX,const int sizeY){
	for(int i=0;i<sizeX;i++){
		for(int j=0;j<sizeY;j++){
			bool isLetter = false; //checks if coordinates (i,j) contain a letter or a hash (#)
			for(vector<Record>::iterator it = rl.begin(); it!=rl.end(); it++){
				switch(it->getRow()){
					case ROW:
						if(!isLetter && i==it->getX() && j>=it->getY() && j<it->getY()+it->getLength()){
							cout<<(it->getContent())[j-it->getY()];
							isLetter = true;
						}
					break;
					case COLUMN:
						if(!isLetter && j==it->getY() && i>=it->getX() && i<it->getX()+it->getLength())
						{
							cout<<(it->getContent())[i-it->getX()];
							isLetter = true;
						}
					break;
				}
			}
			if(!isLetter) cout<<"#";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

void gen(vector<Record>& rl,int sizeX,int sizeY){
	if(rl[rl.size()-1].getContent()!=""){
		show(rl,sizeX,sizeY);
	}
	else{	
		//find the first empty element in the list
		int q = 0;
		while(rl[q].getContent()!=""){q++;}
		//now q indicates on the first empty element
		//open proper dictionary file
		int len = rl[q].getLength();
		ifstream dict(to_string(len)+".txt");
		if(! dict.is_open()) cerr<<"Error: opening dictionary failed!"<<endl;
		else{
			string bfr = "";
			while(dict>>bfr){//run through the dictionary
				bool isAcceptable = true;//check if string bfr is acceptable
				
				//check all crossings with other records
				
				for(int i=0;i<rl.size();i++){
					if(rl[q].getRow()!=rl[i].getRow() && rl[i].getContent()!=""){

						if((rl[q].getRow()==COLUMN) * (rl[i].getX()>=rl[q].getX() && rl[i].getX()<rl[q].getX()+rl[q].getLength() && rl[q].getY()>=rl[i].getY() && rl[q].getY()<rl[i].getY()+rl[i].getLength()) + (rl[q].getRow()==ROW)*(rl[q].getX()>=rl[i].getX() && rl[q].getX()<rl[i].getX()+rl[i].getLength() && rl[i].getY()>=rl[q].getY() && rl[i].getY()<rl[q].getY()+rl[q].getLength())){//if rl[q] and rl[i] have a common element
							
							int indq = (rl[q].getRow()==COLUMN)*(rl[i].getX()-rl[q].getX()) + (rl[q].getRow()==ROW)*(rl[i].getY()-rl[q].getY());
							int indi = (rl[q].getRow()==COLUMN)*(rl[q].getY()-rl[i].getY()) + (rl[q].getRow()==ROW)*(rl[q].getX()-rl[i].getX());
							if(rl[i].getContent()[indi]!=bfr[indq]){ //if character does not match...
								isAcceptable=false;break; 	 //...then it means bfr is not acceptable
							}
								
						}
					}
				}		
				
				if(isAcceptable){
					rl[q].setContent(bfr);
					gen(rl,sizeX,sizeY);
					rl[q].setContent("");
				}
			}
		}
	}
}

int main(){	
	vector<Record> recordList;	
	
	//TEST VALUES
	
	const int sizeX = 5, sizeY = 5;
	
	//test crossword scheme
	
	//   -Y->
	//
	//  #.#.#
	//| #....
	//X .....
	//| #....
	//v .....

	recordList.push_back(Record(0,1,COLUMN,5,""));
	recordList.push_back(Record(1,2,COLUMN,4,""));
	recordList.push_back(Record(0,3,COLUMN,5,""));
	recordList.push_back(Record(1,4,COLUMN,4,""));
	recordList.push_back(Record(1,1,ROW,4,""));
	recordList.push_back(Record(2,0,ROW,5,""));
	recordList.push_back(Record(3,1,ROW,4,""));
	recordList.push_back(Record(4,0,ROW,5,""));
	
	gen(recordList,sizeX,sizeY);

	return 0;
}
