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

#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "inttypes.h"

#define MSGPARSERMAXSIZE  32 // Maximum number of characters of the message and of the input stream
#define MSGPARSERMAXFIELDS 2 // Maximum number of entries per message

/*
    MessageParser searches for messages in the input stream.
    The messages must follow the pattern 'BS'entry0'FS'entry2'FS'...'ES', where
    - 'BS' is the symbol for the beginning of the message,
    - 'ES' is the symbol for the end of the message and
    - 'FS' is the field separator.
    The suggested pattern is <entry0,entry1,...>

    Usage:
        - Create an instance of the class
        - Initialize the instance with the desired patterns (using begin method)
        - Append data to the input stream (using append method)
        - Check for new message (using the hasMessage method)
        - If a message was found, i.e., hasMessage returned true, one can
          retrieve the data through the getEntryXXX methods.
*/

class MessageParser
{
public:
    // Initializer
    void begin(char BS='<', char FS=',', char ES='>');
    
    // Appends a char to the input stream
    void append(char ch);

    // Checks for message. If a message was found, parses it, and returns true.
    bool hasMessage();

    // Returns the entre at entryNumber as a string
    void getEntryStr(const int& entryNumber, char*  entry);

    // Returns the entre at entryNumber as an integer
    int  getEntryInt(const int& entryNumber);

    // Returns the entre at entryNumber as a float
    float getEntryFloat(const int& entryNumber);

    // Internal variables
    char                      BS {'<'}; // Begin separator
    char                      FS {','}; // Field separator
    char                      ES {'>'}; // End separator
    char         str[MSGPARSERMAXSIZE]; // The incoming string
    char         msg[MSGPARSERMAXSIZE]; // The message
    uint8_t idxvec[MSGPARSERMAXFIELDS]; // Indexes of the start position of the fields
    uint16_t           numberOfEntries; // Number of entries

private:
    // Find the entries of the message
    void parseMessage();
};

#endif // MESSAGEPARSER_H