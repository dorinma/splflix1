//
// Created by dorin on 20/11/2019.
//

#include <iostream>
#include "../include/Session.h"

using namespace std;

int main(int argc, char** argv){
   /* if(argc!=2)
    {
        cout << "usage splflix input_file" << endl;
        return 0;
    }*/
    Session s(argv[1]);
    cout << "\n";
    s.start();
    return 0;
}
