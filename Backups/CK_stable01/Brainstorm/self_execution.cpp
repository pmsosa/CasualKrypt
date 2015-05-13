#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char*argv[]) {
    cout << argv[1];
    
    system("a.exe 1");
    
    cout << "Print";

    return 0;
}