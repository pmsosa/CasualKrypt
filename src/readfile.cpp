
//Reading chuncks from files//

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

using namespace std;

const char target = 24;
const int size = 200;

void search(string& flow, ifstream& file){



    

    stringstream flowbuffer;
    char buf = 0;


    while (!file.eof()){

         buf = file.get();
        if (buf == target || buf <= -1){

            flow = flowbuffer.str();
            return;
        }

        flowbuffer << buf;




      }

  flow = flowbuffer.str();
  return;

}

int main(){

    //Size Equation
    //Repeats MAX 50 NUMBER
  

    //ENCRYPT READ
    
    ifstream fileEnc;
    fileEnc.open("test.txt", ios::binary);
    
    fstream outEnc("test1.txt", ios::out | ios::binary);
    char buf = 0;
    
    while (!fileEnc.eof()){
        for (int i = 0; i <=size; i++){
          if (!fileEnc.eof()){  
              buf = fileEnc.get();
              if (buf >= 0){
                    outEnc << buf;
              }
          }
          else {
            //Encrypt Data //Output Encrypted Data
            outEnc << target;
            i = size+1;
          }
          
        }
        //Encrypt Data //Output Encrypted Data
        outEnc << target;
        
    }
    
    fileEnc.close();
    outEnc.close();
    

    
    
    //DECRYPTION READING//
    ifstream file;
    file.open("test1.txt", ios::binary);

    fstream out("test2.txt", ios::out | ios::binary);
    
    while (!file.eof()){
    
    string flow = "";
    search(flow,file);
    // Decrypt Data
    cout << flow;
    out << flow;
    
    }
    file.close();

    

return 0;
}


///////////////////////////
// 10.115.120.250 // crd //
// 10.124.104.5   // Std //
// 10.179.120.6   // Stf //
// 10.179.120.4   // cc  //
///////////////////////////


///USLESS CRAP///

  /*
    ENCRYPTION READING

    0. OPEN FILESTREAM

     While (!file.eof){
        1. READ TILL NUM LIMIT
        2. ENCRYPT PIECE
        3. ADD SENTINEL
        4. OUTPUT TO FILE
    }
    */

    /*
        DECRYPTION READING

        0. OPEN FILESTREAM

         While (!file.eof){
            1. READ TILL SENTINEL
            2. DECRYPT PIECE
            3. OUTPUT TO FILE
        }
    */


    /*
    
    ifstream fileEnc;
    fileEnc.open("test.txt", ios::binary);
    
    fstream outEnc("test1.txt", ios::out | ios::binary);
    
    while (!fileEnc.eof()){
        int length =  20;
        char* buffer = new char [length];
        
        
        fileEnc.read (buffer,length);
        
        //If you are at the last part
        
        for (int i = 0; i < length;i++){
            
            if (buffer[i] >0 && fileEnc.tellg() > (streampos)-1){
                cout << (int)buffer[i] << "- "<< (int)fileEnc.tellg()<< endl;
                outEnc << buffer[i];
            }
        }
        outEnc << '.';
        
    
    }
    fileEnc.close();
    outEnc.close();
    */
