#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include <vector>
using namespace std;
ifstream input;
ofstream output;


class Node { 
	public:
		map<char, Node*> nmap;
		map<char, Node*>::iterator mi;
		char transition;
		bool acceptedState = false;
		string state; 
		Node(string s) {
			state = s;
		}
		void makeMap(char c, Node *n);
		Node getState(char c);
};

void Node::makeMap(char c, Node *n) {
	nmap[c] = n; 
}


ifstream ifile(char*f) {   //input file
ifstream in;
in.open(f, ifstream::in);
    if(!in.is_open()) {
        cerr << "ERROR: could not open file " << f << endl;
        exit(1); 
    }
		return in;
}

int main(int argc, char **argv) {
	input = ifile(argv[1]);
	map<string, Node*> mState;  //map string to node object
	map<string, Node*>::iterator miState; //iterator
	vector<string> states; //holds state strings to print out path
	string node1;
	string node2;
	char transition; 
	
	//check arguments
	if(argc != 4) {
		cerr << "ERROR: Incorrect number of arguments" << endl;
		return 1;
	}
	
	//read from file and build state machine
	while (input) {
	input >> node1 >> transition >> node2;
	
	//check to see if states are present in mState map
	miState = mState.find(node1); 
	if (miState == mState.end()) {
		mState[node1] = new Node(node1);
	}
	
	miState = mState.find(node2);
	if (miState == mState.end()) {
		mState[node2] = new Node(node2);
	}
	
	 
	//map first node to the next node, using the transition as a key
	mState[node1]->makeMap(transition,mState[node2]);

	}
	    
		//print out the state machine
		cout << "***STATE MACHINE***" << endl;
		cout << "STARTING STATE: " << argv[2] << endl;
		cout << "ACCEPTING STATE: " << argv[3] << endl;
		for(miState = mState.begin(); miState != mState.end(); miState++) {
			 string state = miState->first;
			 cout << state << ": ";
			 Node *n = mState[state]; //used to access node object to obtain its map to the next node
			 for(n->mi = n->nmap.begin(); n->mi != n->nmap.end(); n->mi++) {
				 char trans = n->mi->first; 
				 string nextState = n->mi->second->state;
				 cout << "(" << trans << ", " << nextState << ") ";
			 }
			 cout << endl;
		}
		
		cout << "==================================" << endl;
		
	
	//set up accepted state
	mState[argv[3]]->acceptedState = true; 
	 
	//enter sequence and store the path into a vector
	char s;
	string currentState = mState[argv[2]]->state;
	states.push_back(currentState);
	cout << "Enter sequence: ";
	while(cin >> s) { 
        mState[currentState]->mi = mState[currentState]->nmap.find(s); 
		if (mState[currentState]->mi == mState[currentState]->nmap.end()) {
			cerr << "'" << s << "' is not an acceptable entry"  << endl;  	
			return 1;
		} 
		currentState = mState[currentState]->nmap[s]->state; 
		states.push_back(currentState);
	    }
	
	//print out the path 
	cout << "Path: ";
	for(int i = 0; i < states.size(); i++) {
		cout << states[i] << " ";
	} 
    
	//check to see if the final state was the accepted state
	if(mState[states[states.size()-1]]->acceptedState) { 
		cout << endl << "Accepted!" << endl; 
	}
	else {
		cout << endl << "Rejected!" << endl;
	}
	
	//deallocate memory
	for(miState = mState.begin(); miState != mState.end(); miState++) {
			 delete miState->second;		
	}
	//close file
	input.close();
}


