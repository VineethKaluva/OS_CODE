/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AlphaHistogram.cpp
 * Author: vineeth
 * 
 * Created on January 8, 2018, 9:01 PM
 */

#include "AlphaHistogram.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
AlphaHistogram::AlphaHistogram(string s1) {
    ifstream is(s1);    
    char c;
    int j=0;
  for(int i=33;i<=126;i++)    //numbers to ascii
   {
        char num1=ASCItranslate(i);
        array[j]=num1;
        j++;
   }
    
    for(int y=0;y<=93;y++)
    {
        array2[y]=0;
    }
    
  while (is.get(c))
  {
    for(int k=0;k<=93;k++)
    {
        
        if(array[k]==c)
        {
            int x=0;
            if(array2[k]==0)
            {
                array2[k]+=1;
            }
            else
            {
                x=array2[k];
                x+=1;
                array2[k]=x;
            }
        }
    }
  }
  is.close();  
  
    
}


int AlphaHistogram::count(char n)
{
    int data=0;
    for(int i=0;i<=93;i++)
    {
        if(array[i]==n)
        {
          data=array2[i];  
        }
    }
    
    return data;
}
int AlphaHistogram::ASCItranslate(int c1)
{
    return static_cast<char>(c1);
}