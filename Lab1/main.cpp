/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vineeth
 *
 * Created on January 8, 2018, 8:59 PM
 */


// test samples are file1.txt and CompleteShakespeare.txt
#include <iostream>
#include <string>
#include <fstream>
#include "AlphaHistogram.h"
using namespace std;

int ASCItranslate(int c1)
{
    return static_cast<char>(c1);
}
int main ()
{
  string mystr; 
  char array[93];
  char c;
  int charcount;
  cout << "file name:: ";
  getline (cin, mystr);
  int j=0;
  for(int i=33;i<=126;i++)    
   {
        char num1=ASCItranslate(i);
        array[j]=num1;
        j++;
   }
  AlphaHistogram histo(mystr);
  int count_array[93];
  for(int i=0;i<=93;i++)
  {
      c=array[i];
      charcount=histo.count(c);
      count_array[i]=charcount;
  }
      for(int i=0;i<=93;i++)
    {
        if(count_array[i]!=0)
            cout<< array[i] << " "<< count_array[i]<< "\n";
    }
  return 0;
}
