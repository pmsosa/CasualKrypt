/**
    CS-11 Final Project
    CasualKrypt Main
    Purpose: Data Encryption/Decryption

    @author Pedro M. Sosa
    @version 3.0 5/20/2012
*/

//CONSTANTS//
const bool ENC = true;
const bool DEC = false;

//NECESARY FOR ENCRYPT/DECRYPT CLASS
const int LOW_ASCII = 32;
const int HIGH_ASCII = 126;
const int ASCII_RANGE = 94; 

//LIBRARIES
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream> 
#include<streambuf>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include <vector>
#include <signal.h>
#include <windows.h>
#include <Winuser.h>
#include "base64.cpp" //Base64 Library (written by René Nyffenegger)
#include "EncryptClass.cpp"
#include "DecryptClass.cpp"

using namespace std;


/**
    Checks for any invalid command input.

    @param argv array of given arguments
    @param argc argv array size
    @param i current argument place to be checked
    @return boolean variable that expresses wether the
    argument is valid or invalid
*/
bool checkParse(char* argv[],int argc,int i);

/**
    Makes a copy of a file

    @param file name of file
    @return name of copied file
*/
char* fileCopy(char* file);

/**
    Parses the Arguments given

    @param argv array of given arguments
    @param argc argv array size
    @return integer number that expresses error codes
*/
int argumentparser(int argc, char*argv[]);

/**
    Prints CasualKrypt Logo and Motto!

    @return integer number that expresses error code
*/
int easter();

/**
    Prints Help and other Information

    @param info number that defines which info to print
    @return integer number that expresses error codes
*/
int printMetaInfo(int info);

/**
    Displays any error message

    @param errorCode number that defines which error code to print
    @return integer number that expresses error codes
*/
int displayError(int& errorCode);

/**
    Handles Terminate Signal Number 2 (Ctrl+C) 

    @param signum signal number
*/
void signal_callback_handler(int signum);

/**
    Detaches the commandline and makes it run on the background.
*/
void stealth();

//MAIN FUNCTION//
int main(int argc, char*argv[]) {

    // Register signal and signal handler
   signal(SIGINT, signal_callback_handler);

    if (argc <= 1){
    cout << "\nFile Cyphering:\n casualkrypt -[mode] -[options] -o [OUTfile] -i [INfile] -p [Password]\n\n";
    cout << "On-The-Go Cyphering:\n casualkrypt -[mode] -[options] -o [OUTfile] -p [Password] -msg\n";
    cout << "\n\nTry 'casualkrypt --help' for more options\n";
    }
    else {
        int errorcode = 0;
        errorcode = argumentparser(argc, argv);
        displayError(errorcode);
    }



    //encrypt("PwnSosa",(char*)"TrueT.ckt",(char*)"OutFile1.pms");
    //decrypt("PwnSosa",(char*)"OutFile1.pms",(char*)"TheBigDeal.ckt");

    //cout << "hit enter!";
    //int debug;
    //cin >> debug;

    return 0;
}


///FUNCTIONS///


//PARSING CHECKER
bool checkParse(char* argv[],int argc,int i){
    const char* OPTIONS[] = { "-e","-encrypt","-d","-decrypt","-msg","-b64","-l","-r","-i","-o","-p","--help","--about"};
    const int OPT_LENGTH = sizeof(OPTIONS)/sizeof(*OPTIONS);

    if (!(argc > (i+1))){
        return false;
    }
    else { 
        char* parse = argv[i+1];
        
        for (int i = 0; i < OPT_LENGTH; i++){
            if (strcmp(parse,OPTIONS[i])== 0){
                return false;
            }
        }
    }
    return true;
}

//MAKE A COPY OF ANY FILE
char* fileCopy(char* file){
    
        //Declare both Files. Copy & Original
        fstream fileCopy;
        string copyText;
        
        //Open and copy whats in Original to string
        ifstream TextBuffer(file, ios::binary);
        TextBuffer.seekg(0, ios::end);
        copyText.reserve(TextBuffer.tellg());
        TextBuffer.seekg(0, ios::beg);
        copyText.assign((istreambuf_iterator<char>(TextBuffer)),istreambuf_iterator<char>());
        
        //Copy string to copyFile
        fileCopy.open("OrigCopy.ckt",fstream::out | ios::binary);
        fileCopy << copyText;
        fileCopy.close();
    
    return (char*)"OrigCopy.ckt";
}

//ARGUMENT PARSER
int argumentparser(int argc, char*argv[]) {
    char* inFile = (char*)"";
    char* outFile = (char*)"cypher.txt";
    char* transFile = (char*)"";
    char* inFileOriginal = inFile;
    bool RemoveOrigin = false;
    bool onTheGo = false;
    bool runSilent= false;
    bool openOutput = false;
    int fileSize = 0;
   
    
    int repeat = 0;

    string password = "";
    bool mode = ENC;
    bool modeGiven = false;
    bool inFileGiven = false;
    bool outFileGiven = true;
    bool passGiven = false;
    bool b64 = false;
    bool print = false;
    bool loud = false;
    bool pDisplacement = false;
    bool header = false;
    bool vectorAlgorithm =false;
    
    
    ///THIS IS THE PUBLISHED VERSION
    ///THEREFORE IT IS ONLY NATURAL THAT THE ENCRYPTION PROTOCOL SHOULD BE THE BEST POSSIBLE
    ///SO BY DEFAULT IT WILL HAVE THE FOLLOWING VALUES:
    modeGiven = true;
    b64 = true;
    repeat = 2;
    pDisplacement = true;
    header = true;
    vectorAlgorithm =true;
    //////////////////////

    for(int i = 1; i < argc; i++) {

        if (strcmp(argv[i],"--help")==0){
            return 5;
        }

        else if (strcmp(argv[i],"--about")==0){
            return 6;
        }

        else if (strcmp(argv[i],"--easter")==0){
            return 7;
        }

        //CHECK MODE
        else if (strcmp(argv[i],"-e")==0) {
            mode = ENC;
            modeGiven = true;
        }
        else if (strcmp(argv[i],"-encrypt")==0) {
            mode = ENC;
            modeGiven= true;
        }
        else if (strcmp(argv[i],"-d")==0) {
            mode = DEC;
            modeGiven=true;
        }
        else if (strcmp(argv[i],"-decrypt")==0) {
            mode = DEC;
            modeGiven = true;
        }
        //CHECK OPTIONS
        //-Repeat
        else if (strcmp(argv[i],"-r")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                repeat = atoi(argv[i]);
                if (repeat == 0){
                    return 9;
                }
                if (repeat == 1){
                    repeat = 0; //Because 1 causes a bug.
                }
            } else { return 9;}
        }
        //-Loud
        else if (strcmp(argv[i],"-l")==0) {
            loud = true;
        }
        //-Delete Original
        else if (strcmp(argv[i],"-del")==0) {
            RemoveOrigin = true;
        }
        //-Run Background
        else if (strcmp(argv[i],"-s")==0) {
            runSilent= true;
        }
        //-Run Background
        else if (strcmp(argv[i],"-open")==0) {
            openOutput= true;
        }
        //-Base64 Encoding
        else if (strcmp(argv[i],"-b64")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                b64 = (bool) atoi(argv[i]);
            }
        }
        //-Vector Algorithm
        else if (strcmp(argv[i],"-v")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                vectorAlgorithm = (bool) atoi(argv[i]);
            }
        }
        //-Strong
        else if (strcmp(argv[i],"-strong")==0) {
            b64 = true;
            repeat = 2;
            pDisplacement = true;
            header = true;
        }
        //-Password Displacement
        else if (strcmp(argv[i], "-pdis")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                pDisplacement = (bool) atoi(argv[i]);
            }
        }
        //-Header
        else if (strcmp(argv[i], "-h")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                header = (bool) atoi(argv[i]);
            }
        }


        //CHECK FILES
        else if (strcmp(argv[i],"-i")==0) {
            if (onTheGo == false){
                if (checkParse(argv,argc,i)){
                    i++;
                    inFile = argv[i];
                    ifstream testFile(inFile);
                    if (!testFile.fail()){
                        inFileGiven = true;
                        testFile.seekg (0, ios::end);
                        fileSize = testFile.tellg();
                        testFile.close();
                    }
                }
            }
            else {return 8;}
        }
        else if (strcmp(argv[i],"-o")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                outFile = argv[i];

                ifstream testFile(outFile);
                if (testFile){
                    outFileGiven = true;
                }
            }
        }

        //CHECK IF IT IS ON-THE-GO//
        else if (strcmp(argv[i],"-msg")==0) {
            if (inFileGiven == false){
                onTheGo = true;
            }
            else{ return 8;}
        }

        //CHECK PASSWORD
        else if (strcmp(argv[i],"-p")==0) {
            if (argc > (i+1)){
                i++;
                password = argv[i];
                passGiven = true;
            }
        }
        
        else {cout <<"\nParameter: " <<  argv[i] << " not recognized.\n"; return 10;}
    }
    
    //ON THE GO ENCRYPTION/DECRYPTION
    if (onTheGo == true){
            string clearText = " ";
            cout << "\nWrite your message: \n\n";
            getline( cin, clearText );
            //Create the TempFile
            inFile = (char*) "TempCKTFile.ckt";
            inFileGiven = true;
            //Fill the File with the cleartext
            fstream clearFile;
            clearFile.open(inFile,fstream::out);
            clearFile << clearText;
            clearFile.close();
            //Makesure it is Erased Eventually.
            RemoveOrigin = true;
        }

    //Print Variable - Do we want to print the enc/dec process?
    //if (loud == true || onTheGo == true){ print = true;}
    print = loud;
        
    //If everything is parsed and correct then lets enc/dec!  
    if (modeGiven && inFileGiven && outFileGiven && passGiven) {
        
        //IF GO SILENT, THEN DISAPPEAR
        if (runSilent){
            stealth();
        }
        
        
        //ENCRYPTION MODE
        if (mode == ENC) {
            
            //Copy Original File - Precaution in case somthing goes wrong. We don't want to damage the original.
            inFileOriginal = inFile;
            inFile = fileCopy(inFile);
            
            Encryptor message;
            
            for (int i = 0; i<=repeat; i++) {

                //REPEAT, SWITCHING FILES
                if (i != 0){
                    transFile = outFile;
                    outFile = inFile;
                    inFile = transFile;
                }
                
                message.encrypt(password, inFile, outFile, true, b64, pDisplacement,vectorAlgorithm, print,header);
            }
            
        }
        else if (mode == DEC) {
            
            //Copy Original File - Precaution in case somthing goes wrong. We don't want to damage the original.
            inFileOriginal = inFile;
            inFile = fileCopy(inFile);
            
            Decryptor message;
            
            for (int i = 0; i<=repeat; i++) {
            
                if (i != 0){
                    transFile = outFile;
                    outFile = inFile;
                    inFile = transFile;
                }
                
                message.decrypt(password, inFile, outFile, true, b64, pDisplacement,vectorAlgorithm, print,header);
            }
        }
        
        if (openOutput){
            /*Seems overly complicated just to open a simple file with the Commandline
              But it turns out that the difference between cmd "text.txt" and cmd "start text.txt"
              Is that without the "start" the program will wait for the file to finish executing before continuing.
              With the "start" the program will simply start another detached process. -12:22AM 5/19/2012 (ima gonna go to sleep now)
            */
            stringstream tempStream;
            string cmd;
            tempStream << "start " << outFile << endl;
            getline(tempStream,cmd);
            system(cmd.c_str());
        }
        
        if (RemoveOrigin){
            if (remove(inFileOriginal) != 0){cout << "Couldn't delete original file"<<endl;}
        }
        
    } 
    else if (modeGiven !=true){ return 3;}
    else if (inFileGiven !=true){return 2;}
    else if (passGiven !=true) {return 4;}
    else {return 1;}

    //Loud Printing of Variables for Debuggin Purpose
    if (loud) {
        cout << "<>-----------<>\n"
             << "  Mode: " <<mode << endl
             << "  In File: " << inFileOriginal << endl
             << "  FileSize: " << setw(1) << setprecision(2) << fixed << fileSize << " Bytes" << endl
             << "  Out File: " << outFile << endl
             << "  Password: " << password << endl
             << "  Repeats: " << repeat << endl
             << "  Base64: " << (bool) b64 <<endl
             << "  Password Displacement: " <<(bool) pDisplacement<< endl
             << "  Vector Algorithm: " << (bool) vectorAlgorithm << endl
             << "  Delete Original: " << (bool)RemoveOrigin << endl
             << "  Open Output File: " << (bool)openOutput << endl
             << "  Loud : " << loud << "\n\n";

    }

    return 0;
}

//EASTER EGG (because all programs should have one!)
int easter(){
cout <<"\n"
<<"               --.`                                " << endl
<<"             `o+osso+/:-`                         " << endl
<<"              -yssysso++o+-                       " << endl
<<"              :ysssysoo+++++     `                " << endl
<<"              :s+oysy+o++++++  -+ys.              " << endl
<<"              `syyssyo++++++o. /oyo::-.           " << endl
<<"               -yyyyyo+++--+o/ `ss `-//+:`        " << endl
<<"                `+yyyy++o`   `` -y:   `:/:/:       " << endl
<<"              -+sssso+++       /y`     .///+`     " << endl
<<"             `o+sooso++o/    `:/:-       ///+.    " << endl
<<"             .o++o++o++os   .:/+//`       ://+`   " << endl
<<"             .o+oo++++++o+:oo..y/`         ./:/   " << endl
<<"             /++o+++++o+ososs+/y`           `/o   " << endl
<<"             o+++oooyssooo+oy++o              o`  " << endl
<<"             /o++++o/+s++s+so:y:                  " << endl
<<"             -+ooooo+ooo+o.` :y`                  " << endl
<<"             /+++++++++++o.  +s                   " << endl
<<"             +++++++++++++:  sy                   " << endl
<<"            `o++++++++++++/ `sy`                  " << endl
<<"            ++++++++++++++o..sy/                  " << endl
<<"           /ooo++++++++++++o-sss                  " << endl
<<"         -+ooo++++++++++++++:oyy.                 " << endl
<<" ...-:/+ooo+++++++++++++++++ooso`                 " << endl
<<" ....`` ``...---------------.` " << endl
<<"--------------------------------------------------" <<endl
<<"             DEAD MAN TELL NO TALE"                 <<endl
<<"--------------------------------------------------" <<endl
<<"Special Thanks To: Mom & Pops, Luigi, Ikus, Almojarifazgo,\nand all my cousins around the globe" <<endl;
return 0;
}

//PRINTING META INFORMATION
int printMetaInfo(int info){
    if (info == 1){
        cout<<"\nFile Cyphering:\n casualkrypt -[mode] -[options] -o [OUTfile] -i [INfile] -p [Password]\n\n"
            <<"On-The-Go Cyphering:\n casualkrypt -[mode] -[options] -o [OUTfile] -p [Password] -msg"
            <<"\n\n"
            <<"MODES:\n"
            <<" -e , -encrypt : Encrypt File\n"
            <<" -d , -decrypt : Decrypt File"
            <<"\n\n"
            <<"OPTIONS:\n"
            <<" -msg   : Let's you type in your own message on the console.\n"
            <<" -r [x] : (Default: 2) Repeat Enc/Dec [x] amount of times.\n"
            <<" -l     : Prints Data To Screen\n"
            <<" -del   : Deletes input file upon cyphering completion\n"
            <<" -h     : (0-ON, 1-OFF,Default= ON) Adds Encryption Header for added security\n"
            <<" -pdis  : (0-ON, 1-OFF,Default= ON) Password Displacement for added security\n"
            <<" -b64   : (0-ON, 1-OFF,Default= ON) Use Base64 Encoding\n"
            <<" -v     : (0-ON, 1-OFF,Default= ON) Use Anarchy Vector Algorithm\n"
            <<" -s     : Run Silent. Run on Background\n"
            <<" -open  : Open the output file when done.\n"
            <<"\n\n"
            <<"FILES:\n"
            <<" -i [INfile]    : input file to encrypt or decrypt.\n"
            <<" -o [OUTfile]   : output (enc/dec) processed file.\n"
            <<"                 (OPTIONAL - Default: cyphered.txt)\n"
            <<" -p [Password]  : The password.\n\n"
            <<" (If files/passwords have spaces : write between qoutation marks \"test 1\")\n"
            <<" (You can reference files on different directories \"C:\\programs\\foo.txt\")"
            <<"\n\n"
            <<" --about : information about the program\n"
            <<" --help  : displays this usage screen"
            <<"\n\n\n";
    }

    else if (info == 2){
        cout << "\nCasualKrypt 1.5.0\n"
             << "Programmed by: Pedro M. Sosa <konukoii@gmail.com>\n"
             << "Website & Donations: sosavpm.users.sourceforge.net\n\n"
             << "Licenced under GNU GPL\n"
             << "(C) 2012 Pedro Sosa\n"
             << "This is free software; see the source for copying conditions.\n"
             << "There is NO warranty; not even for MERCHTABILITY or FITNESS FOR A PARTICULAR PURPOSE\n"
             << "Base64 Encoding programmed by Rene Nyffenegger\n\n";
    }
    else if (info ==3){easter();}
    return 0;
}

//ERROR DISPLAY
int displayError(int& errorCode){

    if (errorCode == 0){cout << "\nGreat Success!\n";}
    if (errorCode == 1){cout << "\nError: Not enough arguments given!\n";}
    if (errorCode == 2){cout << "\nError: File to Enc/Dec does not exist\n";}
    if (errorCode == 3){cout << "\nError: No Mode Given!\n";}
    if (errorCode == 4){cout << "\nError: No Password Given!\n";}
    if (errorCode == 5){printMetaInfo(1); return 0;} //help
    if (errorCode == 6){printMetaInfo(2);} //about
    if (errorCode == 7){printMetaInfo(3);} //easter Egg
    if (errorCode == 8){cout << "\nInput file given and On-The-Go text recieved! Can't do both at the same time.\n";}
    if (errorCode == 9){cout << "\n'-r' option given with no valid number given\n";}
    if (errorCode != 0){cout << "\nTry 'casualkrypt --help' for usage information\n";}
    return 0;
}

//SIGNAL CALLBACK HANDLER
void signal_callback_handler(int signum){
    cout << "Caught Terminate Signal " << signum;
    
    /* CLEANUP AND CLOSE STUFF HERE
       This is actually written in for the eventual Qt GUI that will be developed
       The whole point of it is that if by any chance this program dies it returns -1
       That way the Qt GUI will get the message and warn the user that the process is dead.
    */
    try {
    
        /*In Theory you could check here wether any file is currently in use
        and then try to close it for good. But, in practice C++ takes care of 
        closing everything up without leaving any curropt files behind.*/
        
        //message.empty();
        cout << "\nGoodbye! (Code 1)\n";
        exit(-1);
    }
    catch (int e){
        //If you fail closing any files, simply quit and don't worry, be happy.
        cout << "\nGoodbye! (Code 2)\n";
        exit(-1);
    }
    
    // Terminate program
    exit(signum);
}

//RUN ON BACKGROUND
void stealth(){
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth,0);
    
    SetPriorityClass(GetCurrentProcess(),BELOW_NORMAL_PRIORITY_CLASS);
    
}

