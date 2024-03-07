/*
  The MIT License (MIT)

  Copyright (C) 2022 Guilherme Bertoldo and Jonas Joacir Radtke
  (UTFPR) Federal University of Technology - Parana

  Permission is hereby granted, free of charge, to any person obtaining a 
  copy of this software and associated documentation files (the “Software”), 
  to deal in the Software without restriction, including without limitation 
  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  and/or sell copies of the Software, and to permit persons to whom the Software 
  is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all 
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "MessageParser.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"


/*
    Finds the index of the first occurence 
    of pattern char in str.
    If the pattern is absent, returns -1.
*/
int findFirst(const char *str, char pattern)
{
    for (size_t i = 0; i < strlen(str); ++i)
    {
        if (str[i] == pattern)
            return i;
    }
    return -1;
}

/*
    Finds the index of the last occurence 
    of pattern char in str.
    If the pattern is absent, returns -1.
*/
int findLast(const char *str, char pattern)
{
    for (size_t i = strlen(str); i > 0; --i)
    {
        if (str[i-1] == pattern)
            return i-1;
    }
    return -1;
}

/*
    Shift a string left
*/
void shiftLeft(char *str, int shift)
{
    int sz = strlen(str);

    if (shift < 0)
        return;

    if (shift >= sz)
    {
        str[0] = '\0';
        return;
    }

    for (int i = 0; i < sz - shift; ++i)
    {
        str[i] = str[i + shift];
    }
    str[sz - shift] = '\0';
}

void searchMessage(char *str, char *msg, char bExp, char eExp)
{
    bool searchCompleted = false;

    int idxB, idxE;

    while (!searchCompleted)
    {
        // Getting the first occurence of eExp
        idxE = findFirst(str, eExp);

        // Cleaning the message
        msg[0] = '\0';

        // If end pattern was found
        if (idxE >= 0)
        {
            // Copying the first part of the pattern to msg
            strncpy(msg, str, idxE);

            // Setting the end of message
            msg[idxE] = '\0';

            // Shifting the str (removing the copied part)
            shiftLeft(str, idxE + 1);

            // Looking for the last occurrence of the start pattern in msg
            idxB = findLast(msg, bExp);

            // If start pattern was found
            if (idxB >= 0)
            {
                // Formatting the message
                shiftLeft(msg, idxB + 1);
                searchCompleted = true;
            }
            else
            {
                // Message undefined
                msg[0] = '\0';
                searchCompleted = false;
            }
        }
        // If end pattern was NOT found
        else
        {
            searchCompleted = true;

            // Looking for the last occurrence of the start pattern in str
            idxB = findLast(str, bExp);

            // If start pattern was found
            if (idxB >= 0)
            {
                // Removing unnecessary text
                shiftLeft(msg, idxB);
            }
        }
    }
}

void MessageParser::begin(char bs, char fs, char es)
{
    this->BS = bs;
    this->ES = es;
    this->FS = fs;
}

void MessageParser::append(char ch)
{
    // Ignores new line, return carriage and null characters
    if (!(ch == '\n' || ch == '\r' || ch == '\0'))
    {
        // If the str is full, shift it left
        size_t sz = strlen(str);
        if (sz == MSGPARSERMAXSIZE)
        {
            for (size_t i = 0; i < MSGPARSERMAXSIZE - 1; ++i)
                str[i] = str[i + 1];
            sz--;
        }

        // Copying the input data
        str[sz] = ch;
        str[sz + 1] = '\0';
    }
}

bool MessageParser::hasMessage()
{
    // Searching for a message between BS and ES in str.
    // If a message is found, it is moved from str to msg
    // and the function returns true.

    searchMessage(str, msg, BS, ES);
    parseMessage();

    return (strlen(msg) > 0 ? true : false);
}

void MessageParser::getEntryStr(const int& entryNumber, char*  entry)
{
    // Initializing the output
    entry[0] = '\0';

    // Is the entryNumber valid?
    if ( entryNumber < numberOfEntries )
    {
        // Copy the entry
        int16_t idxB = idxvec[entryNumber];
        int16_t idxE = idxvec[entryNumber+1];
        if ( idxE > idxB ) 
        {
            strncpy(entry,&msg[idxB],idxE-idxB);
            entry[idxE-idxB-1]='\0';
        }
    }
}

int MessageParser::getEntryInt(const int& entryNumber)
{
    char buffer[35];
    getEntryStr(entryNumber,buffer);
    return atoi(buffer);
}

float MessageParser::getEntryFloat(const int& entryNumber)
{
    char buffer[35];
    getEntryStr(entryNumber,buffer);
    return atof(buffer);
}

void MessageParser::parseMessage()
{
    size_t sz = strlen(msg);

    // If the message is empty, the number of entries is zero.
    // Just return...
    if ( sz < 1 )
    {
        numberOfEntries = 0;
        return;
    }

    // If the message is not empty, there exist,
    // at least, one entry.
    numberOfEntries = 1;
    idxvec[0]=0;
    for (size_t i=0; i<sz; ++i)
    {
        if (msg[i]==FS)
        {
            idxvec[numberOfEntries]=i+1;
            numberOfEntries++;
        }
    }
    idxvec[numberOfEntries]=sz+1;
}
