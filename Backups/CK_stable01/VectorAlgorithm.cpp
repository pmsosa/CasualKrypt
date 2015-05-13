#include <iostream>
#include <fstream>
#include <string>
#include<streambuf>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <vector>
#include <sstream>
using namespace std;


int main(){

string flow;
ifstream TextBuffer("text.txt");
TextBuffer.seekg(0, ios::end);
flow.reserve(TextBuffer.tellg());
TextBuffer.seekg(0, ios::beg);
flow.assign((istreambuf_iterator<char>(TextBuffer)),istreambuf_iterator<char>());
    
ofstream outFile("out1.txt");

string password = "PwnSosa";

//vvvvv/////////////////////////////////////ACTUAL NEW CODE////////////////////////////////////////////vvvvv//

/////Pre-Calculations

int PocketSize = password.length(); //Each Pocket has this number of characters.
int NumFullPockets = flow.length() / PocketSize;  //Number of COMPLETE Pockets.
int LastPocketSize = flow.length() % NumFullPockets; //This is the Size of the last Pocket. (if it is zero then the flow.length is divisible by the password.length
vector<string> cyVector;


/////Forming The Pockets

//cout << "\n\nFlow Size: " << flow << endl;
//cout << "Pocket Size: " << PocketSize <<endl;
//cout << "Num Of Full Pockets: " << NumFullPockets <<endl;


for (int i= 0; i < flow.length(); i+=(PocketSize)){
    cout << "Pocket: " << flow.substr(i,PocketSize) << endl;
    cyVector.push_back(flow.substr(i,PocketSize));
}

cout << "\nVectorSize: " << cyVector.size() << endl;

/////Creating Anarchy (Encrypting)

cout << "\nEncrypting" << endl;

for (int i=0; i<(cyVector.size()-2);i+=2){ //(VectorSize-1 = Final Packet.) -1 Second to last packet
    string temp = cyVector[i];
    cyVector[i] = cyVector[i+1];
    cyVector[i+1] = temp;
    
    
}

for (int i= 0; i<cyVector.size();i++){

cout << "Pocket: " << cyVector[i] << endl;
}

/////Establishing Order (Decrypting)

cout << "\nDecrypting" << endl;

for (int i=0; i<(cyVector.size()-2);i+=2){ //(VectorSize-1 = Final Packet.) -1 Second to last packet
    string temp = cyVector[i];
    cyVector[i] = cyVector[i+1];
    cyVector[i+1] = temp;
    
    
}

flow = "";

for (int i= 0; i<cyVector.size();i++){
    flow.append(cyVector[i]);
//cout << "Pocket: " << cyVector[i] << endl;
}

cout << flow;

outFile.


//^^^^^^//////////////////////////////////////////ACTUAL NEW CODE///////////////////////////////////////^^^^^^//

outFile.close();


    return 0;
}
