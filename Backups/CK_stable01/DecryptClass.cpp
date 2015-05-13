using namespace std;

class Decryptor{
private:
    string Cylines;    //Cyphered Data
    string password;      //Password
    int N;             //Number of Characters Decrypted (Times Looped)
    int TAP;           //Total ASCII Password
    int OAL;           //Original ASCII Letter
    int ACL;           //Algorithm Cyhpered Letter
    string flow;       //The String in which all the manipulation will be done before writing output to file.
    fstream outFile;   //Output File;
    fstream inFile;    //InFile;
    void read_Input(char* inFileName);
    void password_Displacement();
    void casual_Algorithm();
    void header_Creation();
    void vector_Algorithm();
    void output(char* outFileName);
    void log(bool loud, string message);
public:
    void Decrypt(string pass, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement, bool loud,bool header);
    //string GetRawCypher();
    //void empty();
    //PrintFlow
    //PrintCyLines
    //Empty
    
};

void Decryptor::Decrypt(string pass, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement, bool loud,bool header){

    //Convert Passwords to ASCII
    password = pass;
    Cylines = "";        //Cyphered Data
    N = 0;                  //Number of Character Decrypted (Times looped)
    TAP = 0;                //Total ASCII Password
    OAL = 0;                //Original ASCII Letter
    ACL = 0;                //Algorithm Crypted Letter
    flow = "";            //The String in which all the manipulation will be done before writing output to file.


    //1. ADD PASSWORD
    for(int i = 0; i <(int)password.length(); i++) {
        TAP += password[i];
    }
    ///////////////////////////////////
    
    //2. READ FROM FILE -> BUFFER -> STRING.
    read_Input(inFileName);
    ///////////////////////////////////
    
    //3. HEADER
    if (header){
      header_Creation();
    }
    ///////////////////////////////
    
    //4. CK DECRYPTION ALGORITHM
    //  -a. Convert letter to ASCII then sum with TAP;
    //  -b. Check that it doesn't end up out of range (32-126)
    casual_Algorithm();

    
   //5. PASSWORD DISPLACEMENT
    if (pDisplacement){
       password_Displacement();
    }
    //////////////////////////
    
    //4. ANARCHY VECTOR ALGORITHM //
    cout << "YES";
    vector_Algorithm();
    /////////////////////////

    //3. BASE64 DecryptION //
    if (b64 == true){
            Cylines = base64_decode(Cylines);
    }
    /////////////////////////
    
    //6. OUTPUT TO TEXTFILE
    output(outFileName);

    //7. Option: Delete Original
    if (RemoveOrigin){
        if (remove(inFileName) != 0){cout << "Couldn't delete original file";}
    }

    //ESTO VA A SER ELIMINADO POR EL LOG() FUNCTION.
    //8. Option: Loud Printing
    //if (loud) {
        //cout << "\n\nOriginal Text:\n" << flow1;
        //cout << "\n\nDecrypted Text:\n" <<Cylines << "\n\n";}

    
}

void Decryptor::read_Input(char* inFileName){
    ifstream TextBuffer(inFileName);

    TextBuffer.seekg(0, ios::end);          //Select Text form beginning to end
    flow.reserve(TextBuffer.tellg());       //Reserve The lenght of the text for said string
    TextBuffer.seekg(0, ios::beg);          //Return the pointer to the start

    flow.assign((istreambuf_iterator<char>(TextBuffer)),istreambuf_iterator<char>());
}

void Decryptor::password_Displacement(){
    int dispNum = 1+(TAP%(password.length()));
    int dispLetter = Cylines[dispNum] - password[(password.length()-1)];
    //cout <<(int)dispLetter << "=" << (int)Cylines[dispNum] << " - " << (int)password[(password.length()-1)] <<endl;
    //cout <<(char)dispLetter << "=" << (char)Cylines[dispNum] << " - " << (char)password[(password.length()-1)] <<endl;
    //cout <<dispNum << endl;
    while (dispLetter < 32) {
        dispLetter +=94;
    }
    //cout <<(int)dispLetter<< ": " << (char)dispLetter<<endl ;
    Cylines[dispNum] = dispLetter;
}

void Decryptor::log(bool loud, string message){
    if (loud){
        cout << message << endl;
    }
}

void Decryptor::casual_Algorithm(){
 for(int i = 0; i <(int)flow.length(); i++) {
        ACL = flow[i];
        //if (ACL ==10){OAL = 10;} //Backspace - No deberia pasar nada (?)
        //if (ACL ==13){OAL = 13;} //Enter
        if ((ACL >= 32) && (ACL <=126)) {
            OAL = ACL-TAP+N;
            N-=1;
            while (OAL < 32) {
                OAL +=94;
            }
        }
        else {
            OAL = ACL;   //ACA NUEVO
        }
        Cylines.append(1,(char)OAL);
    }
}

void Decryptor::header_Creation(){
  srand(time(0));
        char pointer = password[(TAP % password.length())];
        for (unsigned i = 0; i<flow.length();i++){
            if (flow[i] == pointer){
                flow.erase(0,(i+1));
                i = flow.length();
            }
        }
}

void Decryptor::output(char* outFileName){
    outFile.open(outFileName,fstream::out);
    outFile << Cylines;
    outFile.close();
}

void Decryptor::vector_Algorithm(){
    
    //Precalculations
    
    int PocketSize = password.length(); //Each Pocket has this number of characters.
    int NumFullPockets = Cylines.length() / PocketSize;  //Number of COMPLETE Pockets.
    int LastPocketSize = Cylines.length() % NumFullPockets; //This is the Size of the last Pocket. (if it is zero then the flow.length is divisible by the password.length
    vector<string> cyVector;    //Vector that wil be used to store the pockets.

    //Forming the Packets inside the Vector

    for (int i= 0; i < Cylines.length(); i+=(PocketSize)){
        cyVector.push_back(Cylines.substr(i,PocketSize));
    }
    
    //Enforcing Order (Decryption)
    
    for (int i=0; i<(cyVector.size()-2);i+=2){ //(VectorSize-1 = Final Packet.) -1 Second to last packet
        string temp = cyVector[i];
        cyVector[i] = cyVector[i+1];
        cyVector[i+1] = temp; 
    }
    
    //Reforming the String
    Cylines = ""; 
    for (int i= 0; i<cyVector.size();i++){
        Cylines.append(cyVector[i]);
    }

}
