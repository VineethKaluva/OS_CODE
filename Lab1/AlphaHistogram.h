/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AlphaHistogram.h
 * Author: vineeth
 *
 * Created on January 8, 2018, 9:01 PM
 */

#ifndef ALPHAHISTOGRAM_H
#define ALPHAHISTOGRAM_H
#include <string>
#include <iostream>
using namespace std;
class AlphaHistogram {
public:
    AlphaHistogram(string s);
    int count(char n);
    int ASCItranslate(int c1);
private:
    char array[93];
    int array2[93];

};

#endif /* ALPHAHISTOGRAM_H */

