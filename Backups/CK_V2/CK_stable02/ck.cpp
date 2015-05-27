//LAST WORKED ON: CTC 5:00 March 21 2012
//CONDITION: FINAL STABLE.
//BRANCH: UBUNTU SOFTWARE CENTER

#include <iostream>
#include <fstream>
#include <string>
#include<streambuf>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include "base64.cpp" //Base64 Library (written by René Nyffenegger)
using namespace std;


//Global Variables
const bool ENC = true;
const bool DEC = false;

//ENCRYPTION MODULE
int encrypt(string password, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement, bool loud,bool header) {

    //Convert Passwords to ASCII
    string Cylines = "";        //Cyphered Data
    int N = 0;                  //Number of Character Encrypted (Times looped)
    int TAP = 0;                //Total ASCII Password
    int OAL = 0;                //Original ASCII Letter
    int ACL = 0;                //Algorithm Crypted Letter
    string flow = "";            //The String in which all the manipulation will be done before writing output to file.
    fstream outFile;            //OUTPUT FILE


    //1. ADD PASSWORD
    for(int i = 0; i <(int)password.length(); i++) {
        TAP += password[i];

    }

    //2. READ FROM FILE -> BUFFER -> STRING.
    ifstream TextBuffer(inFileName);

    string flow1;
    TextBuffer.seekg(0, ios::end);
    flow1.reserve(TextBuffer.tellg());
    TextBuffer.seekg(0, ios::beg);

    flow1.assign((istreambuf_iterator<char>(TextBuffer)),istreambuf_iterator<char>());
    flow = flow1;
    ///////////////////////////////////

    //3. BASE64 ENCRYPTION //
    if (b64 == true){
      //cout << "\nOriginal Text: "<<flow<<endl;
      flow = base64_encode(reinterpret_cast<const unsigned char*>(flow.c_str()), flow.length());
      //cout << "Base64 Encrypted: "<<flow<<endl;
    }
    /////////////////////////
    
    
   //5. PASSWORD DISPLACEMENT
        if (pDisplacement){ //passDisp
           int dispNum = 1+(TAP%(password.length()));
           int dispLetter = flow[dispNum] + password[(password.length()-1)];
           //cout <<(int)dispLetter << "=" << (int)flow[dispNum] << " + " << (int)password[(password.length()-1)] <<endl;
           //cout <<(char)dispLetter << "=" << (char)flow[dispNum] << " + " << (char)password[(password.length()-1)] <<endl;
           //cout <<dispNum << endl;
           while (dispLetter > 126) {
                dispLetter -=94;
            }
           //cout <<(int)dispLetter<< ": " << (char)dispLetter<<endl;
           flow[dispNum] = dispLetter;
           
        }
    //////////////////////////
    

    //4. CK ENCRYPTION ALGORITHM
    //  -a. Convert letter to ASCII then sum with TAP;
    //  -b. Check that it doesn't end up out of range (32-126)

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
        Cylines.append(1,(char)ACL);
    }

	//6. HEADER
    if (header){
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
        HC.append(Cylines);
        Cylines = HC;
    }
    ///////////////////////////////
	

    //6. OUTPUT TO TEXTFILE
    outFile.open(outFileName,fstream::out);
    outFile << Cylines;
    outFile.close();

    //7. Option: Delete Original
    if (RemoveOrigin){
        if (remove(inFileName) != 0){cout << "Couldn't delete original file";}
    }

    //8. Option: Loud Printing
    if (loud) {
        cout << "\n\nOriginal Text:\n" << flow1;
        cout << "\n\nEncrypted Text:\n" <<Cylines << "\n\n";
    }


    return 0;
}

//DECRYPTION MODULE
int decrypt(string password, char* inFileName, char* outFileName, bool RemoveOrigin, bool b64, bool pDisplacement, bool loud,bool header) {

    //Convert Passwords to ASCII
    string Cylines = "";        //Cyphered Data
    int N = 0;                  //Number of Character Encrypted (Times looped)
    int TAP = 0;                //Total ASCII Password
    int OAL = 0;                //Original ASCII Letter
    int ACL = 0;                //Algorithm Crypted Letter
    string flow = "";            //The String in which all the manipulation will be done before writing output to file.
    fstream outFile;            //OUTPUT FILE

    //1. ADD PASSWORD
    for(int i = 0; i <(int)password.length(); i++) {
        TAP += password[i];
    }

    //2. READ FROM FILE -> BUFFER -> STRING.
    ifstream TextBuffer(inFileName);

    TextBuffer.seekg(0, ios::end);          //Select Text form beginning to end
    flow.reserve(TextBuffer.tellg());       //Reserve The lenght of the text for said string
    TextBuffer.seekg(0, ios::beg);          //Return the pointer to the start

    flow.assign((istreambuf_iterator<char>(TextBuffer)),istreambuf_iterator<char>());
    ///////////////////////////////////

    //6. HEADER
    if (header){
        srand(time(0));
        char pointer = password[(TAP % password.length())];
        for (unsigned i = 0; i<flow.length();i++){
            if (flow[i] == pointer){
                flow.erase(0,(i+1));
                i = flow.length();
            }
        }
    }
    ///////////////////////////////
    
    //3. CK DECRYPTION ALGORITHM
    //  -a. Convert letter to ASCII then sum with TAP;
    //  -b. Check that it doesn't end up out of range (32-126)
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

    
    //5. PASSWORD DISPLACEMENT
        if (pDisplacement){ //passDisp
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
    //////////////////////////
    
    
    //4. BASE64 DECRYPTION //
        if (b64 == true){
            Cylines = base64_decode(Cylines);
        }
    /////////////////////////


    //6. OUTPUT TO TEXTFILE
    outFile.open(outFileName,fstream::out);
    outFile << Cylines;
    outFile.close();

    //7. Option: Delete Origin

    if (RemoveOrigin){
        if (remove(inFileName) != 0){cout << "Couldn't delete original file";}
    }

    //8. Option: Loud Printing
    if (loud) {
        cout << "\n\nCyphered Text:\n" << flow;
        cout << "\n\nDecrypted Text:\n" << Cylines << "\n\n";
    }

    return 0;
}

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
        ifstream TextBuffer(file);
        TextBuffer.seekg(0, ios::end);
        copyText.reserve(TextBuffer.tellg());
        TextBuffer.seekg(0, ios::beg);
        copyText.assign((istreambuf_iterator<char>(TextBuffer)),istreambuf_iterator<char>());
        
        //Copy string to copyFile
        fileCopy.open("OrigCopy.ckt",fstream::out);
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
    
    
    ///THIS IS THE PUBLISHED VERSION
    ///THEREFORE IT IS ONLY NATURAL THAT THE ENCRYPTION PROTOCOL SHOULD BE THE BEST POSSIBLE
    modeGiven = true;
    b64 = true;
    repeat = 2;
    pDisplacement = true;
    header = true;
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
        //-Base64 Encoding
        else if (strcmp(argv[i],"-b64")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                b64 = (bool) atoi(argv[i]);
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
                    ifstream testfile(inFile);
                    if (testfile){
                        inFileGiven = true;
                    }
                }
            }
            else {return 8;}
        }
        else if (strcmp(argv[i],"-o")==0) {
            if (checkParse(argv,argc,i)){
                i++;
                outFile = argv[i];

                ifstream testfile(outFile);
                if (testfile){
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
    if (loud == true || onTheGo == true){ print = true;}
        
    //If everything is parsed and correct then lets enc/dec!  
    if (modeGiven && inFileGiven && outFileGiven && passGiven) {
        
        //ENCRYPTION MODE
        if (mode == ENC) {
            
            //Copy Original File - Precaution in case somthing goes wrong. We don't want to damage the original.
            inFileOriginal = inFile;
            inFile = fileCopy(inFile);
            
            for (int i = 0; i<=repeat; i++) {

                //REPEAT, SWITCHING FILES
                if (i != 0){
                    transFile = outFile;
                    outFile = inFile;
                    inFile = transFile;
                }
                
                encrypt(password, inFile, outFile, true, b64, pDisplacement, print,header);
            }
        }
        else if (mode == DEC) {
            
            //Copy Original File - Precaution in case somthing goes wrong. We don't want to damage the original.
            inFileOriginal = inFile;
            inFile = fileCopy(inFile);
        
            for (int i = 0; i<=repeat; i++) {
            
                if (i != 0){
                    transFile = outFile;
                    outFile = inFile;
                    inFile = transFile;
                }

                decrypt(password, inFile, outFile, true, b64, pDisplacement, print,header);
            }
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
             << "  Out File: " << outFile << endl
             << "  Password: " << password << endl
             << "  Repeats: " << repeat << endl
             << "  Base64: " << b64 <<endl
             << "  Password Displacement: " << pDisplacement<< endl
             << "  Delete Original: " << RemoveOrigin << endl
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
int displayError(int errorCode){

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

//MAIN FUNCTION
int main(int argc, char*argv[]) {

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


/*SMALL EXPLANATIONS NEEDED TO UNDERSTAND THIS PROGRAM BETTER
A) Technically speaking you can use this program to encrypt jpgs,mp3,zip,etc. But take into consideration that the bigger the file the more time it takes.
    A.1) Test Results:
    A.1) Text File -> "Call Of the Wild" (194 KB) = 10sec
    A.2) Jpg  File -> 3516 x 2335 (648 KB) = 20sec
    A.3) Mp3  File -> "Girl from Ipanema" (5,407 KB) = 20 Minutes
    A.4) zip  Files -> Empty Zip File = <1 sec
    A.5)

B) Each letter represented is represented as an int. 

C) The idea behind using ints instead of chars is mainly because that way you have a bigger range to make as many mathematical operations as you need.
   The main encryption algorithm works this way:
    0. Get the cleartext from input
    1. Encode the cleartext to base64
    2. Sum up each letter of the password till you get a total.
    3. Sum that total to the first letter of the base64 encoded text.
    4. Password Displacement is a simple option to make a bruteforce attack fail.
        4.1 [num] = Modulus the password total against the password length
        4.2 Take [num] character in the cleatext and add the last letter of the password to that character, again check to see it is in range (method explained in 5.1)
        Note: The whole idea behind it is to change one simple character in the text so that if someone is simply 
              substracting numbers backwards he wont be able to get the cleartext because the Base64 will fail de decoding. 
              Its like a simple, but effective lock.
    5. Check to see if it is in the normal ascii printable range (32-126)
        5.1 If it is above that then substract the lenght of the range which is 94.
            Note: It will never be below 126, because you added on step 4. so no need to check for less than 32
    6. Finally Just Output the encoded text.

C) The main decryption algorithm works exactly the inverted version of the encryption.
D) I put many options to turn off or on, basically so that an attackers possibilities are bigger. But the truth is that the best you could possibly do is:
    Base64, Plassword Displacement, Repeat 3 Times.
E) Repeating the algorithm a couple of times is actually the strongest method of protecting your data. It would take up a bunch more time to bruteforce.

*/

/* Notes
    -RANGE OF ASCII KEYS ACCEPTED = 32 to 126
    -ACCEPTS MULTIPLE REPEATED CYPHERING
    -ACCEPTS MULTIPLE ENCRYPTIONS MODES FOR ADDED SECURITY
    -EXTENSIVELY TESTED WITH 'CALL OF THE WILD' TEXT. PASSED WITH FLYING COLORS.
*/

/*SHIT TO DO: 
*   1. Separate encryption/decryption into class; each step becomes a member function
*       1.2 Mejora esto para que flow sea un parametro que se regrese.
*   2. A-Style y corrige los nombres y formatos de las funciones y de las variables
*   3. -open Open the file in readme or watever. [ system(filename); //It will open with the default program
*   3. Multiple -i "inputfiles" saved inside a vector (perhaps) o simplemente una carpeta.
*   4. Check if multiple files selected
*   5. Percentage Completion.
*   6. Print Size of File and use Fixed and SetPrecision formating.
*   8. GUI Development
*   9. Clean ze code and comments!
*/

/*SHIT THAT WONT BE DONE:
*   2. Dibujito Intro -> Porque le quita seriedad al asunto y ademas lo hace recargado
*   5. -debuggin -> Porque me da ladilla y es inecesario.
*   3. Zip into a file -> Porque se vuelve extremadamente grande el programa.
*   3.  
*/

/*SHIT THAT HAS BEEN DONE:
 *   2. Anarchy Vector Algorithm
 *   3. -h ,-header -> Header Option.
 *  3. -pdis -> password displacement option. el concepto de sumar todo el password y mover la primer letra de ese password para adelante el numero de letras.
 *  4. -mejorar el parser. que se de cuenta de que opcion/parametro cagaste y donde.
 *  2. mejorar lo del printing on the go vs. loud
 *  1. arreglar el output text que salga el original y no el b64 cuando -b64 = true;
 *  4. Base64 Option -> Because it relies on external library that refuses to work.
 *  1. On-The-Go Encryption/Decryption
 *  1.Dec Algorithm
 *  9. -easteregg -> command option que imprime un easter egg.
 *  2.No Dependancy on B64 encryption
 *  1. hacer los commandline commands con Args -> main(int argc, char* argv[])
 *  2. que la persona pueda elegir el archivo que quiere
 *  6. -rem      -> command option que borra el original file
 *  7. --about    -> command option que escribe about the program y el website donde encontrarlo
 *  8. --help    -> command option que escribe la ayudita
 *  0. Explicarle al usuario cuando la cago y como arreglar la vaina
 *  1. Looping
 *  3. Hacer que el loud sea mas lindo
 */
