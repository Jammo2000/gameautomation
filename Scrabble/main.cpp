#include <cxxabi.h>

#include <algorithm>
#include <limits>
#include <string>
#include <functional>
#include "solver.h"

using namespace std;
void toUpperCase(string &s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return toupper(c); });
}

template <typename T>
string type_name() {
    int status;
    string tname = typeid(T).name();
    char *demangled_name =
        abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
    if (status == 0) {
        tname = demangled_name;
        free(demangled_name);
    }
    return tname;
}

template <class outputType>
outputType terminalInput(const string &prompt) {
    outputType input;

    while (true) {
        cout << prompt;
        if (!(cin >> input)) {
            cout << "Please enter " << type_name<outputType>() << " only."
                 << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            return input;
        }
    }
}

template <class outputType>
outputType terminalInputValidate(const string &prompt, const string &invalidPrompt, const function<bool(string)>& validator) {
    outputType input;

    while (true) {
        cout << prompt;
        if (!(cin >> input)) {
            cout << "Please enter " << type_name<outputType>() << " only."
                 << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
						if(validator(input)){
            	return input;
						}
						else{
							cout << invalidPrompt<< endl;
            	cin.clear();
							cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						}
        }
    }
}

bool getBool(const string &prompt) {
    char input;
    while (true) {
        cout << prompt;
        if (!(cin >> input)) {
            cout << "Please enter y or n only." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            input = toupper(input);
            if (input == 'Y') {
                return true;
            } else if (input == 'N') {
                return false;
            } else {
                cout << "Please enter y or n only." << endl;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

int getInt(const string &prompt) {
    int input = 0;

    while (true) {
        cout << prompt;
        if (!(cin >> input)) {
            cout << "Please enter an integer only." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            return input;
        }
    }
}
void updateTileSet(AlphabetSet &tileSet) {
    string input;
    cout << "Enter the drawn tiles, then press enter: ";
    cin >> input;
    toUpperCase(input);
    cout << input;
    tileSet.insert(input);
}
void printTiles(const AlphabetSet& tiles){
		string tileString;
		for(char i = 'A';i<='Z';i++){
			tileString.append(tiles.at(i), i);
			if(tiles.at(i)>0){
				tileString+=' ';
			}
		}
		cout<<"My tiles are "<< tileString <<endl;
}
int main() {
    int maxTiles = terminalInput<int>("How many tiles are in a rack in this game? ");
		string tileInput = terminalInputValidate<string>(
			"What tiles did I draw? ",
			"Please enter "+to_string(maxTiles)+" tiles.",
			[maxTiles](string input){
				return input.length() == maxTiles;
		});
		toUpperCase(tileInput);
		tiles.insert(tileInput);
		while(true){
			cout<<"Here is the board"<<endl;
			board.print();
			if(!getBool("Is it my turn? (y/n):")){
				bool isCorrect = false;
				Move opponentMove(0,0,false,"YEET");
				while(!isCorrect){
					int x = getInt("What is the x of the beginning of the opponent's move? (leftmost is 0): ");
					int y = getInt("What is the y of the beginning of the opponent's move? (topmost is 0): ");
					bool right = getBool("Is the opponent's move going right? (y/n)");
					string word = terminalInput<string>("What word did the opponent play? ");
					toUpperCase(word);
					opponentMove = Move(x,y,right,word);
					Board testBoard = board;
					testBoard.applyMove(opponentMove);
					testBoard.print();
					isCorrect = getBool("Is this correct? (y/n) ");
				}
				int points = opponentMove.getPoints(board);
				cout<<"I believe this move got "<<points<<" points. (but i'm probably wrong)"<<endl;
				board.applyMove(opponentMove);
				board.print();
				cout<<endl;
			}else{
				printTiles(tiles);
				vector<Move> moves = getLegalMoves(board, tiles);
				cout << moves.size() << " legal moves found" << endl;
        cout << "evaluating moves" << endl;
        for (size_t i = 0; i < moves.size(); i++) {
            moves[i].value = moves[i].getPoints(board);
        }
        cout << "sorting moves by value" << endl;
        sort(moves.begin(), moves.end(),
             [](const Move &a, const Move &b) { return b.value < a.value; });
				cout<<"Best move found"<<endl;
				bool isAcceptable = false;
				size_t index = 0;
				while(!isAcceptable){
						Board testBoard = board;
						testBoard.applyMove(moves[index]);
						cout<<"Move #"<<index<<": "<<moves[index]<<endl;
						testBoard.print();
						isAcceptable = getBool("Is this move acceptable? (y/n) ");
						index++;
				}
				index--;
				AlphabetSet tilesUsed = moves[index].tilesUsed(board);
				unsigned tilesNeeded = tilesUsed.total();
				board.applyMove(moves[index]);
				tiles.remove(tilesUsed);
				board.print();
				cout<<"I believe this move got "<<moves[index].value<<" points. (but i'm probably wrong)"<<endl;
				string newTiles = terminalInputValidate<string>(
					"Enter the "+to_string(tilesNeeded)+ " new tiles, or fewer if there aren't enough",
					"Invalid, please try again",
					[tilesNeeded](string input){
						unsigned length = input.length();
						if(length>tilesNeeded){
							cout<<"That is too many tiles"<<endl;
							return false;
						}
						else if(length<tilesNeeded){
							return getBool("Are you sure there aren't any more tiles? (y/n) ");
						}
						else{
							return true;
						}
				});
				toUpperCase(newTiles);
				tiles.insert(newTiles);
				printTiles(tiles);
			}
		}
    return 0;
}