using namespace std;

class Encryptor{
private:
    string cylines;    //Cyphered Data
    string password;   //Password
    int N;             //Number of Characters Encrypted (Times Looped)
    int TAP;           //Total ASCII Password
    int OAL;           //Original ASCII Letter
    int ACL;           //Algorithm Cyhpered Letter
    string flow;       //The String in which all the manipulation will be done before writing output to file.
    fstream outFile;   //Output File;
    fstream inFile;    //InFile;
    bool debug;
    void read_Input(char* inFileName);
    void password_Displacement();
    void casual_Algorithm();
    void header_Creation();
    void vector_Algorithm();
    void output(char* outFileName);
    void log(bool loud, string message);
    void log(ofstream& fout, string message);
public:
    Encryptor();
    Encryptor(bool debugParam);
    void encrypt(string pass, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement, bool vectorAlgorithm, bool loud,bool header);
    void empty();
    //string GetRawCypher();
    //void empty();
    //PrintFlow
    //PrintCyLines
    //Empty
    
};

Encryptor::Encryptor(){
    //DEFAULT CLASS CONSTRUCTOR//
    debug = false;    
}

Encryptor::Encryptor(bool debugParam){
    //OVERLOADED CLASS CONSTRUCTOR//
    debug = debugParam;
    log(true,"Debug Mode Activated");

}

void Encryptor::encrypt(string pass, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement,bool vectorAlgorithm, bool loud,bool header){

    //Convert Passwords to ASCII
    password = pass;
    cylines = "";        //Cyphered Data
    N = 0;                  //Number of Character Encrypted (Times looped)
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

    //3. BASE64 ENCRYPTION //
    if (b64 == true){
      //cout << "\nOriginal Text: "<<flow<<endl;
      flow = base64_encode(reinterpret_cast<const unsigned char*>(flow.c_str()), flow.length());
      //cout << "Base64 Encrypted: "<<flow<<endl;
    }
    /////////////////////////
    
    //4. ANARCHY VECTOR ALGORITHM //
    if (vectorAlgorithm){
        vector_Algorithm();
    }
    /////////////////////////
    
   //5. PASSWORD DISPLACEMENT
    if (pDisplacement){
       password_Displacement();
    }
    //////////////////////////
    

    //4. CK ENCRYPTION ALGORITHM
    //  -a. Convert letter to ASCII then sum with TAP;
    //  -b. Check that it doesn't end up out of range (32-126)
    casual_Algorithm();
    
	//6. HEADER
    if (header){
      header_Creation();
    }
    ///////////////////////////////
	
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
        //cout << "\n\nEncrypted Text:\n" <<cylines << "\n\n";}

    
}

void Encryptor::read_Input(char* inFileName){

    ifstream inFile(inFileName);
    string flow1;
    inFile.seekg(0, ios::end);
    flow1.reserve(inFile.tellg());
    inFile.seekg(0, ios::beg);

    flow1.assign((istreambuf_iterator<char>(inFile)),istreambuf_iterator<char>());
    //There is a difference between flow and flow1 for debuggin purpose to be able to compare integrity
    flow = flow1;
}

void Encryptor::password_Displacement(){
    int dispNum = 1+(TAP%(password.length()));
    int dispLetter = flow[dispNum] + password[(password.length()-1)];
    while (dispLetter > 126) {
        dispLetter -=94;
    }
    flow[dispNum] = dispLetter;
}

void Encryptor::casual_Algorithm(){
 for(int i = 0; i <(int)flow.length(); i++) {
        OAL = flow[i];
        //if (OAL ==10){ACL = 10;} //Backspace - No deberia pasar nada (?)
        //if (OAL ==13){ACL = 13;} //Enter
        if ((OAL >= 32) && (OAL <=126)) {
            ACL = OAL+TAP+N;
            N+=1;
            while (ACL > 126) {
                ACL -=94;
            }
        }
        else {
            ACL = OAL;   //ACA NUEVO
        }
        cylines.append(1,(char)ACL);
    }
}

void Encryptor::header_Creation(){
  srand(time(0));
        int bodySize = (1+(rand()%11))+(password.length());
        string body = "";
        char pointer = password[(TAP % password.length())];
        char randLetter =(rand()%33+93);
        for (int i = 0; i<=bodySize;i++){
            randLetter =((rand()%33)+93);
            while (randLetter == pointer){
               randLetter=(rand()%33+93);
            }
            body.append(1,randLetter);
        }
        
        body.append(1,pointer);
        string HC = "";
        HC.append(body);
        HC.append(cylines);
        cylines = HC;
}

void Encryptor::output(char* outFileName){
    outFile.open(outFileName,fstream::out);
    outFile << cylines;
    outFile.close();
}

void Encryptor::vector_Algorithm(){
    
    //Precalculations
    
    int PocketSize = password.length(); //Each Pocket has this number of characters.
    int NumFullPockets = flow.length() / PocketSize;  //Number of COMPLETE Pockets.
    int LastPocketSize = flow.length() % NumFullPockets; //This is the Size of the last Pocket. (if it is zero then the flow.length is divisible by the password.length
    vector<string> cyVector;    //Vector that wil be used to store the pockets.

    //Forming the Packets inside the Vector

    for (int i= 0; i < flow.length(); i+=(PocketSize)){
        cyVector.push_back(flow.substr(i,PocketSize));
    }
    
    //Enforcing Order (Decryption)
    
    for (int i=0; i<(cyVector.size()-2);i+=2){ //(VectorSize-1 = Final Packet.) -1 Second to last packet
        string temp = cyVector[i];
        cyVector[i] = cyVector[i+1];
        cyVector[i+1] = temp; 
    }
    
    //Reforming the String
    flow = ""; 
    for (int i= 0; i<cyVector.size();i++){
        flow.append(cyVector[i]);
    }

}

void Encryptor::empty(){
    if (outFile.good()){
        outFile.close();
    }
    if (inFile.good()){
        inFile.close();
    }
}

void Encryptor::log(bool loud, string message){
    if (loud){
        cout << message << endl;
    }
    if (debug){
        fstream fout("log.txt",fstream::out);
        log(fout, message);
        fout.close();
    }
}

void Encryptor::log(ofstream& fout, string message){
    fout << message;
}

