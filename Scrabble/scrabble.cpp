#include <array>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <cmath>
using namespace std;

#define BoardSize 15

enum specialSpace { EM, L3, L2, W3, W2 };

unordered_map<char, short> points{
	{'A', 1},
	{'B', 3},
	{'C', 3},
	{'D', 2},
	{'E', 1},
	{'F', 4},
	{'G', 2},
	{'H', 4},
	{'I', 1},
	{'J', 8},
	{'K', 5},
	{'L', 1},
	{'M', 3},
	{'N', 1},
	{'O', 1},
	{'P', 3},
	{'Q', 10},
	{'R', 1},
	{'S', 1},
	{'T', 1},
	{'U', 1},
	{'V', 4},
	{'W', 4},
	{'X', 8},
	{'Y', 4},
	{'Z', 10},
};

inline bool isLetter(char c) { return c >= 'A' && c <= 'Z'; }

class Move{
	public:
  int x;
  int y;
  bool right;
  string word;
	int value;
  Move(int x, int y, bool right, string word) {
    this->x = x;
    this->y = y;
    this->right = right;
    this->word=word;
  }
	//returns the points gained if this move was placed onto board
	//assumes the move is legal
	int getPoints(const char (&board)[BoardSize][BoardSize]){
		int mainPoints = 0;
		int mainMultiplier = 1;
		int tilesPlaced = 0;
		int totalCrossPoints = 0;
		if(this->right){
			for(int i = 0;i<this->word.length();i++){
				switch(board[this->y][this->x+i]){
					case L3:
						mainPoints+=3*points[this->word[i]];
						break;
					case L2:
						mainPoints+=2*points[this->word[i]];
						break;
					case W2:
						mainPoints+=points[this->word[i]];
						mainMultiplier*=2;
						break;
					case W3:
						mainPoints+=points[this->word[i]];
						mainMultiplier*=3;
						break;
					case EM:
						mainPoints+=points[this->word[i]];
						tilesPlaced++;
						break;
					default:
						mainPoints+=points[this->word[i]];
				}

				//cross words
				int letterPosition;
				int searchTop;
				string crossWord = "";
				int searchY = this->y-1;
				//search to tops
				while(searchY>0&&isLetter(board[searchY][this->x+i])){
					searchY--;
				}
				if(searchY>0){
					searchY++;
				}
				searchTop = searchY;
				letterPosition = y-searchY;
				while(searchY<BoardSize&&(isLetter(board[searchY][this->x+i])||searchY==this->y)){
					crossWord+=board[searchY][this->x+i];
					searchY++;
				}
				if(crossWord.length()>1){
					int crossPoints = 0;
					int crossMultiplier = 1;
					crossWord[letterPosition] = this->word[i];
					for(int j = 0; j<crossWord.length();j++){
						switch(board[searchTop+j][this->x+i]){
							case L3:
								crossPoints+=3*points[crossWord[j]];
								break;
							case L2:
								crossPoints+=2*points[crossWord[j]];
								break;
							case W2:
								crossPoints+=points[crossWord[j]];
								crossMultiplier*=2;
								break;
							case W3:
								crossPoints+=points[crossWord[j]];
								crossMultiplier*=3;
								break;
							default:
								crossPoints+=points[crossWord[j]];
						}
						totalCrossPoints+=crossPoints*crossMultiplier;
					}
				}
			}
			return mainPoints * mainMultiplier + totalCrossPoints;
		}
		else{
			for(int i = 0;i<this->word.length();i++){
				switch(board[this->y+i][this->x]){
					case L3:
						mainPoints+=3*points[this->word[i]];
						break;
					case L2:
						mainPoints+=2*points[this->word[i]];
						break;
					case W2:
						mainPoints+=points[this->word[i]];
						mainMultiplier*=2;
						break;
					case W3:
						mainPoints+=points[this->word[i]];
						mainMultiplier*=3;
						break;
					case EM:
						mainPoints+=points[this->word[i]];
						tilesPlaced++;
						break;
					default:
						mainPoints+=points[this->word[i]];
				}

				//cross words
				int letterPosition;
				string crossWord = "";
				int searchX = this->x-1;
				//search to tops
				while(searchX>0&&isLetter(board[this->y][searchX])){
					searchX--;
				}
				if(searchX>0){
					searchX++;
				}
				int searchTop = searchX;
				letterPosition = x-searchX;
				while(searchX<BoardSize&&(isLetter(board[this->y][searchX])||searchX==this->x)){
					crossWord+=board[this->y][searchX];
					searchX++;
				}
				if(crossWord.length()>1){
					int crossPoints = 0;
					int crossMultiplier = 1;
					crossWord[letterPosition] = this->word[i];
					for(int j = 0; j<crossWord.length();j++){
						switch(board[this->y+i][searchTop+j]){
							case L3:
								crossPoints+=3*points[crossWord[j]];
								break;
							case L2:
								crossPoints+=2*points[crossWord[j]];
								break;
							case W2:
								crossPoints+=points[crossWord[j]];
								crossMultiplier*=2;
								break;
							case W3:
								crossPoints+=points[crossWord[j]];
								crossMultiplier*=3;
								break;
							default:
								crossPoints+=points[crossWord[j]];
						}
						totalCrossPoints+=crossPoints*crossMultiplier;
					}
				}
			}
			return mainPoints * mainMultiplier + totalCrossPoints + tilesPlaced>=7?50:0;
		}
	}

	bool operator==(const Move &other){
		return this->x==other.x&&
					 this->y==other.y&&
			     this->right==other.right&&
			     this->word==other.word;
	}
};
ostream& operator<<(ostream &os, const Move &move){
			os << (move.right?"horizontal ":"vertical ")<< move.word<<" at ("<< move.x<<", "<<move.y<<")";
			return os;
	}
const char alphabet[26] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                           'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                           'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

char board[BoardSize][BoardSize] = {
    {W3, EM, EM, L2, EM, EM, EM, W3, EM, EM, EM, L2, EM, EM, W3},
    {EM, W2, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, W2, EM},
    {EM, EM, W2, EM, EM, EM, L2, EM, L2, EM, EM, EM, W2, EM, EM},
    {L2, EM, EM, W2, EM, EM, EM, L2, EM, EM, EM, W2, EM, EM, L2},
    {EM, EM, EM, EM, W2, EM, EM, EM, EM, EM, W2, EM, EM, EM, EM},
    {EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM},
    {EM, EM, L2, EM, EM, EM, L2, EM, L2, EM, EM, EM, L2, EM, EM},
    {W3, EM, EM, L2, EM, EM, EM, W2, EM, EM, EM, L2, EM, EM, W3},
    {EM, EM, L2, EM, EM, EM, L2, EM, L2, EM, EM, EM, L2, EM, EM},
    {EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, L3, EM},
    {EM, EM, EM, EM, W2, EM, EM, EM, EM, EM, W2, EM, EM, EM, EM},
    {L2, EM, EM, W2, EM, EM, EM, L2, EM, EM, EM, W2, EM, EM, L2},
    {EM, EM, W2, EM, EM, EM, L2, EM, L2, EM, EM, EM, W2, EM, EM},
    {EM, W2, EM, EM, EM, L3, EM, EM, EM, L3, EM, EM, EM, W2, EM},
    {W3, EM, EM, L2, EM, EM, EM, W3, EM, EM, EM, L2, EM, EM, W3},
};

vector<string> readWordList(string filename) {
  fstream file;
  string word;
  vector<string> words;
  // opening file
  file.open(filename.c_str());
  // extracting words from the file
  while (file >> word) {
    words.push_back(word);
  }
	words.shrink_to_fit();
  return words;
}

const vector<string> wordlist = readWordList("wordlist.txt");

bool isInWordlist(string word){
	  int min = 0;
    int max = wordlist.size() - 1;
    while(min <= max){
        int middle = (min + max) / 2;
        if(wordlist[middle] < word){
            min = middle + 1;
				}
        else if(wordlist[middle] > word){
            max = middle - 1;
				}
        else{
            return true;
				}
		}
    return false;
}

void printBoard(const char (&board)[BoardSize][BoardSize]) {
	cout<<"  |";
	for(int x = 0;x<BoardSize;x++){
		cout<<(x<10?x:x-10)<<"|";
	}
	cout<<endl;
  for (int y = 0; y < BoardSize; y++) {
    cout <<(y<10?" ":"")<< y << "|";
    for (int x = 0; x < BoardSize; x++) {
      if (isLetter(board[y][x])) {
        cout << board[y][x];
      }
      else {
        cout << " ";
      }
      cout << "|";
    }
    cout << endl;
  }
	cout<<endl;
}

void makeMove(Move &move, char (&board)[BoardSize][BoardSize]){
	int x = move.x;
	if(move.word == "HAEMOCYANINS"){
		cout<<"ohno";
	}
	int y = move.y;
	for(int i = 0;i<move.word.length();i++){
		board[y][x] = move.word[i];
		x+=move.right;
		y+=!move.right;
	}
}

//returns a basic point count, disregarding tiles
int pointCount(string &word) {
  return 132;
}
//which letters could be placed such that no invalid words would be created, if the main word is in the direction of parameter right.
array<array<unordered_set<char>, BoardSize>, BoardSize> getAllowedLetters(const char (&board)[BoardSize][BoardSize], bool right){
	array<array<unordered_set<char>, BoardSize>, BoardSize> returnValue;
	for(int x = 0;x<BoardSize;x++){
		for(int y = 0; y<BoardSize;y++){
			if(isLetter(board[y][x])){
				returnValue[y][x].insert(board[y][x]);
			}
			else if(right) {
				string crossWord = "";
				int searchY = y-1;
				int letterPosition; //position of letter in the word
				//search to tops
				while(searchY>0&&isLetter(board[searchY][x])){
					searchY--;
				}
				if(searchY>0){
					searchY++;
				}
				letterPosition = y-searchY;
				while(searchY<BoardSize&&(isLetter(board[searchY][x])||searchY==y)){
					crossWord+=board[searchY][x];
					searchY++;
				}
				if(crossWord.length()>1){
					for(char letter:alphabet){
						crossWord[letterPosition] = letter;
						if(isInWordlist(crossWord)){
							returnValue[y][x].insert(letter);
						}
					}
				}
				else{
					for(char letter:alphabet){
						returnValue[y][x].insert(letter);
					}
				}
			}else{
				string crossWord = "";
				int searchX = x-1;
				int letterPosition; //position of letter in the word
				//search to tops
				while(searchX>0&&isLetter(board[y][searchX])){
					searchX--;
				}
				if(searchX>0){
					searchX++;
				}
				letterPosition = x-searchX;
				while(searchX<BoardSize&&(isLetter(board[y][searchX])||searchX==x)){
					crossWord+=board[y][searchX];
					searchX++;
				}
				if(crossWord.length()>1){
					for(char letter:alphabet){
						crossWord[letterPosition] = letter;
						if(isInWordlist(crossWord)){
							returnValue[y][x].insert(letter);
						}
					}
				}
				else{
					for(char letter:alphabet){
						returnValue[y][x].insert(letter);
					}
				}
			}
		}
	}
	return returnValue;
}

vector<Move> getLegalMoves(const char (&board)[BoardSize][BoardSize]){
	vector<Move> moves;
	bool hasCharacter = false;
	for(int x = 0;x<BoardSize;x++){
		for(int y = 0;y<BoardSize;y++){
			if(isLetter(board[y][x])){
				hasCharacter = true;
			}
		}
	}

	bool dirs[] = {false, true};
	float prevCompletion = 0;
	if(hasCharacter){
		for(bool right:dirs){
			auto validLetters = getAllowedLetters(board, right);
			for(int wordIndex = 0;wordIndex<wordlist.size();wordIndex++){
				float completion = (float)(wordIndex+wordlist.size()*right)/(wordlist.size()*2);
				if(floor(completion*100)>floor(prevCompletion*100)){
					cout<<"finding move, "<<floor(completion*100)<<"% complete"<<endl;
					prevCompletion = completion;
				}
				string word = wordlist[wordIndex];
				int wordLength = word.length();
				int maxX = right?BoardSize-wordLength:BoardSize-1;
				int maxY = right?BoardSize-1:BoardSize-wordLength;
				for(int x = 0;x<maxX;x++){
					for(int y = 0;y<maxY;y++){
						bool hasNeighbors = false;
						bool placesTiles = false;
						if(right){
							if(x>0&&isLetter(board[y][x-1])){
								continue;
							}
							if(x<BoardSize&&isLetter(board[y][x+wordLength])){
								continue;
							}
							for(int i = 0;i<wordLength;i++){
								if(!isLetter(board[y][x+i])){
									if(!validLetters[y][x+i].contains(word[i])){
										goto nextWord;
									}
									placesTiles = true;
								}
								else{
									if(board[y][x+i]!=word[i]){
										goto nextWord;
									}
									//crosses another word
									hasNeighbors = true;
								}
								if(!hasNeighbors){
									//touches on side
									if(
										(y>0&&isLetter(board[y-1][x+i]))||
										(y<BoardSize-1&&isLetter(board[y+1][x+i]))
									){
										hasNeighbors = true;
									}
								}
							}
						}
						else{
							if(y>0&&isLetter(board[y-1][x])){
								continue;
							}
							if(y<BoardSize&&isLetter(board[y+wordLength][x])){
								continue;
							}
							for(int i = 0;i<wordLength;i++){
								if(!isLetter(board[y+i][x])){
									if(!validLetters[y+i][x].contains(word[i])){
										goto nextWord;
									}
									placesTiles = true;
								}
								else{
									if(board[y+i][x]!=word[i]){
										goto nextWord;
									}
									//crosses another word
									hasNeighbors = true;
								}
								if(!hasNeighbors){
									//touches on side
									if(
										(x>0&&isLetter(board[y+i][x-1]))||
										(x<BoardSize-1&&isLetter(board[y+i][x+1]))
									){
										hasNeighbors = true;
									}
								}
							}
						}
						if(hasNeighbors&&placesTiles){
							Move move(x,y,right,word);
							moves.push_back(move);
						}
						nextWord:;
					}
				}
			}
		}
	}else{//First move
		for(string word:wordlist){
			if(word.length()<3){
					continue;//rule nobody remembers
			}
			for(int x = 0;x<BoardSize;x++){
				for(int y = 0;y<BoardSize;y++){
					for(bool right:dirs){
						int endX = x + (word.length()-1)*right;
						int endY = y + (word.length()-1)*!right;
						if(endX>=BoardSize||endY>=BoardSize){
							continue;
						}
						int center = BoardSize/2;
						if(x==center){
							if(y<=center&&endY>=center){
								Move m(x,y,right, word);

								moves.push_back(m);
							}
						}
						else if(y==center){
							if(x<=center&&endX>=center){
								Move m(x,y,right, word);
								moves.push_back(m);
							}
						}
					}
				}
			}
		}
	}
	return moves;
}

int main() {
  cout << wordlist.size()<<" words loaded to dictionary"<<endl;
	while(true){
		vector<Move> moves = getLegalMoves(board);
		cout<<moves.size()<<" legal moves found"<<endl;
		cout<<"evaluating moves"<<endl;
		for(size_t i = 0;i<moves.size();i++){
			moves[i].value = moves[i].getPoints(board);
		}
		cout<<"sorting moves by value"<<endl;
		sort(moves.begin(), moves.end(), [](const Move &a, const Move &b){
			return b.value<a.value;
		});
		int moveChoice = 0;
		string input = "0";
		do{
			moveChoice = stoi(input);
			char newBoard[BoardSize][BoardSize];
			memcpy(newBoard,board,BoardSize*BoardSize*sizeof(char));
			makeMove(moves[moveChoice], newBoard);
			cout<<"Move #"<<moveChoice<<", for "<<moves[moveChoice].getPoints(board)<<" points:"<<endl;
			printBoard(newBoard);
			cout<<"Enter new move #, or s to select: ";
			cin>>input;
		}while(input!="s");
		makeMove(moves[moveChoice], board);
		cout<<"Move made:"<<endl;
		printBoard(board);
	}
	cout<<"done";
}
