//
//  main.cpp
//  CRC
//
//  Created by Sankalp Agrawal on 27/02/19.
//  Copyright © 2019 Sankalp Agrawal. All rights reserved.
//

#include <iostream>
using namespace std;
typedef uint64_t UINT64;

class CRC{
private:
    UINT64 word = 0, key = 0;
    UINT64 wordLength = 0, keyLength = 0;
public:
    CRC(UINT64 word, UINT64 key) : word(word), key(key){ }
    
    UINT64 FindLength(UINT64 num){
        UINT64 result = 0;
        while(num){
            result++;
            num = num >> 1;
        }
        return result;
    }
    
    UINT64 FindXOR(UINT64 numberOne, UINT64 numberTwo){
        return (numberOne | numberTwo) & (~numberOne | ~numberTwo);
    }
    bool checkZero(UINT64 result){
        bool  isZeroPresent = false;
        int checkBit = 1;
        checkBit <<= (keyLength - 1);
        if ((result & checkBit)){
            isZeroPresent= false;
        }
        else {
            isZeroPresent = true;
        }
        return isZeroPresent;
    }
    
    UINT64 ModuloDivision(){
        UINT64 result = 0;
        keyLength = FindLength(key);
        wordLength = FindLength(word);
        UINT64 window = 1, n = 1, appendBit = 1, appendValue = 0;
        UINT64 prevResultBit = 1;
        bool once = true;
        for (UINT64 i = keyLength-1; i > 0; i--) {
            n = n << 1;
            window |= n;
        }
        window <<= (wordLength-keyLength);
        prevResultBit = (window & word);
        prevResultBit = prevResultBit >> (wordLength-keyLength);
        appendBit <<= wordLength-keyLength;
        while (appendBit) {
            if(once){
                result = FindXOR(prevResultBit, key);
                once = false;
            }else{
                if(!checkZero(result)){
                    result = FindXOR(prevResultBit, key);
                }else{
                    result = FindXOR(prevResultBit, 0);
                }
            }
            appendBit >>= 1;
            appendValue = word & appendBit;
            if(appendValue) appendValue = 1;
            else appendValue = 0;
            result <<= 1;
            result |= appendValue;
            prevResultBit = result;
        }
        result >>=  1;
        return result;
    }
    
    UINT64 EncodeZeros(){
        UINT64 localKeyLength = FindLength(key);
        localKeyLength -= 1;
        word = word << localKeyLength;
        return word;
    }
    UINT64 EncodeCRC(){
        EncodeZeros();
        UINT64 localResult = ModuloDivision();
        word |= localResult;
        return word;
    }
    UINT64 DecodeCRC(){
        UINT64 localResult = ModuloDivision();
        return localResult;
    }
};

int main(int argc, const char * argv[]) {
    UINT64 word = 212;
    UINT64 key = 5;
    CRC checksum(word, key);
    cout << "CRC Algorithm" << endl;
    cout << "Data Word: " << word << endl;
    cout << "Enter Key: " << key << endl;
    cout << "Senders CRC: " << checksum.EncodeCRC() << endl;
    cout << "Receiver remainder: "<< checksum.DecodeCRC() << endl;
    return 0;
}
