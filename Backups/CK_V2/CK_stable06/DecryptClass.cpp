/**
    CS-11 Final Project
    Decryption Class
    Purpose: Deals With Decryption

    @author Pedro M. Sosa
    @version 3.0 5/20/2012
*/

using namespace std;

//CONSTANTS IN USE//
//OBTAINED FROM CK MAIN//
//const int LOW_ASCII = 32;
//const int HIGH_ASCII = 126;
//const int ASCII_RANGE = 94;

class Decryptor{
private:
    string cylines;    //Cyphered Data
    string password;   //Password
    int N;             //Number of Characters Decrypted (Times Looped)
    int TAP;           //Total ASCII Password
    int OAL;           //Original ASCII Letter
    int ACL;           //Algorithm Cyhpered Letter
    string flow;       //The String in which all the manipulation will be done before writing output to file.
    fstream outFile;   //Output File;
    fstream inFile;    //InFile;
    string debugLog;   //Debugging Log
    bool debug;        //Debug (True or False)
    
    
    /**
        Reads input from file.

        @param inFileName Input File Name
    */  
    void read_Input(char* inFileName);
    /**
        Displaces a Character from the password into the Header
        Helps avoid bruteforcing of the data.
    */  
    void password_Displacement();
    /**
        The Main CasualKrypt Algorithm
    */  
    void casual_Algorithm();
    /**
        Creates a Corrupt Random Header to the cyphered data
    */  
    void header_Creation();
    /**
        Anarchy Vector Algorithm
    */  
    void vector_Algorithm();
    /**
        Outputs Cyphered data into file
        @param outFileName Output File Name
    */  
    void output(char* outFileName);
    /**
        If option "loud" is true, It will print any given message to the screen
        @param loud bool that expreses if loud is turned on
        @param message message to print.
    */  
    void log(bool loud, string message);
    /**
        If option "debug" is true, It will print any given message to the debuggin log file
        @param fout file stream to print.
        @param message message to print.
    */  
    void log(fstream& fout, string message);
    
    
public:
    /**
        Default Constructor
        Initializes Debug = false;
    */  
    Decryptor();
    /**
        Debugging Constructor
        @param debugParam is debug on?
        @param debLog Name of the Debugging Log File 
    */  
    Decryptor(bool debugParam, string debLog);
    /**
        Main Decryption Protocol Function.
        @param pass password
        @param inFileName Input File Name
        @param outFileName Output File Name
        @param RemoveOrigin Remove Origin File? (T or F)
        @param b64 Do Base64 Encryption? (T or F)
        @param pDisp Do Password Displacement? (T or F)
        @param vectorAlgorithm Do Anarchy Vector Algorithm? (T or F)
        @param loud Print to Screen? (T or F)
        @param header Add a Corrupt Header? (T or F)
    */  
    void decrypt(string pass, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement, bool vectorAlgorithm, bool loud,bool header);
    /**
        Empties all files in use.
    */
    void empty();    
};


///FUNCTIONS///


Decryptor::Decryptor(){
    //DEFAULT CLASS CONSTRUCTORS

}

Decryptor::Decryptor(bool debugParam, string debLog){
    //OVERLOADED CLASS CONSTRUCTOR//
    debug = debugParam;
    string debugLog = debLog;
    log(true,"Debug Mode Activated");

}

void Decryptor::decrypt(string pass, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement,bool vectorAlgorithm, bool loud,bool header){

    //Convert Passwords to ASCII
    password = pass;
    cylines = "";        //Cyphered Data
    N = 0;                  //Number of Character Decrypted (Times looped)
    TAP = 0;                //Total ASCII Password
    OAL = 0;                //Original ASCII Letter
    ACL = 0;                //Algorithm Crypted Letter
    flow = "";            //The String in which all the manipulation will be done before writing output to file.


    //1. ADD PASSWORD
    for(int i = 0; i <(int)password.length(); i++) {
        TAP += password[i];
    }
    log(loud, "\n<>------Adding Password into ASCII Integer------<>\n");
    log(loud, password); 
    ///////////////////////////////////
    
    //2. READ FROM FILE -> BUFFER -> STRING.
    read_Input(inFileName);
    log(loud, "\n<>------INPUT Reading------<>");
    log(loud, flow);
    ///////////////////////////////////
    
    //3. HEADER
    if (header){
      header_Creation();
      
      log(loud, "\n<>------Header Creation------<>\n");
      log(loud, flow);
    }
    ///////////////////////////////
    
    //4. CK DECRYPTION ALGORITHM
    //  -a. Convert letter to ASCII then sum with TAP;
    //  -b. Check that it doesn't end up out of range (32-126)
    casual_Algorithm();
    
    log(loud, "\n<>------CasualKrypt Algorithm------<>\n");
    log(loud, cylines);
    
   //5. PASSWORD DISPLACEMENT
    if (pDisplacement){
       password_Displacement();
       
       log(loud, "\n<>------Password Displacement------<>\n");
       log(loud, cylines);
    }
    //////////////////////////
    
    //4. ANARCHY VECTOR ALGORITHM //
    if (vectorAlgorithm){
        vector_Algorithm();
        
        log(loud, "\n<>------Vector Algorithm------<>\n");
        log(loud, cylines);
    }
    /////////////////////////

    //3. BASE64 DecryptION //
    if (b64 == true){
        cylines = base64_decode(cylines);
        
        log(loud, "\n<>------Base64------<>\n");
        log(loud, cylines);
    }
    /////////////////////////
    
    //6. OUTPUT TO TEXTFILE
    output(outFileName);
    
    log(loud, "\n<>------FINAL OUTPUT------<>\n");
    log(loud, cylines);

    //7. Option: Delete Original
    if (RemoveOrigin){
        if (remove(inFileName) != 0){cout << "Couldn't delete original file";}
    }

    //ESTO VA A SER ELIMINADO POR EL LOG() FUNCTION.
    //8. Option: Loud Printing
    //if (loud) {
        //cout << "\n\nOriginal Text:\n" << flow1;
        //cout << "\n\nDecrypted Text:\n" <<cylines << "\n\n";}

    
}

void Decryptor::read_Input(char* inFileName){
    ifstream inFile(inFileName, ios::binary);

    inFile.seekg(0, ios::end);          //Select Text form beginning to end
    flow.reserve(inFile.tellg());       //Reserve The lenght of the text for said string
    inFile.seekg(0, ios::beg);          //Return the pointer to the start

    flow.assign((istreambuf_iterator<char>(inFile)),istreambuf_iterator<char>());
}

void Decryptor::password_Displacement(){
    int dispNum = 1+(TAP%(password.length()));
    int dispLetter = cylines[dispNum] - password[(password.length()-1)];
    //cout <<(int)dispLetter << "=" << (int)cylines[dispNum] << " - " << (int)password[(password.length()-1)] <<endl;
    //cout <<(char)dispLetter << "=" << (char)cylines[dispNum] << " - " << (char)password[(password.length()-1)] <<endl;
    //cout <<dispNum << endl;
    while (dispLetter < 32) {
        dispLetter +=94;
    }
    //cout <<(int)dispLetter<< ": " << (char)dispLetter<<endl ;
    cylines[dispNum] = dispLetter;
}

void Decryptor::casual_Algorithm(){
 for(int i = 0; i <(int)flow.length(); i++) {
        ACL = flow[i];
        //if (ACL ==10){OAL = 10;} //Backspace - No deberia pasar nada (?)
        //if (ACL ==13){OAL = 13;} //Enter
        if ((ACL >= LOW_ASCII) && (ACL <=HIGH_ASCII)) {
            OAL = ACL-TAP+N;
            N-=1;
            while (OAL < LOW_ASCII) {
                OAL +=ASCII_RANGE;
            }
        }
        else {
            OAL = ACL;   //ACA NUEVO
        }
        cylines.append(1,(char)OAL);
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
    outFile.open(outFileName,fstream::out | ios::binary);
    outFile << cylines;
    outFile.close();
}

void Decryptor::vector_Algorithm(){
    
    //Precalculations
    
    int PocketSize = password.length(); //Each Pocket has this number of characters.
    int NumFullPockets = cylines.length() / PocketSize;  //Number of COMPLETE Pockets.
    int LastPocketSize = cylines.length() % NumFullPockets; //This is the Size of the last Pocket. (if it is zero then the flow.length is divisible by the password.length
    vector<string> cyVector;    //Vector that wil be used to store the pockets.

    //Forming the Packets inside the Vector

    for (int i= 0; i < cylines.length(); i+=(PocketSize)){
        cyVector.push_back(cylines.substr(i,PocketSize));
    }
    
    //Enforcing Order (Decryption)
    
    for (int i=0; i<(cyVector.size()-2);i+=2){ //(VectorSize-1 = Final Packet.) -1 Second to last packet
        string temp = cyVector[i];
        cyVector[i] = cyVector[i+1];
        cyVector[i+1] = temp; 
    }
    
    //Reforming the String
    cylines = ""; 
    for (int i= 0; i<cyVector.size();i++){
        cylines.append(cyVector[i]);
    }

}

void Decryptor::empty(){
    if (outFile.good()){
        outFile.close();
    }
    if (inFile.good()){
        inFile.close();
    }
}

void Decryptor::log(bool loud, string message){
    if (loud){
        cout << message << endl;
    }
    if (debug){
        fstream fout(debugLog.c_str(),fstream::app);
        log(fout, message);
        fout.close();
        return;
        
    }
    
}

void Decryptor::log(fstream& fout, string message){
    fout << message;
    return;
}

