#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "inttypes.h"

#define MSGPARSERMAXSIZE  64 // Maximum number of characters of the message and of the input stream
#define MSGPARSERMAXFIELDS 4 // Maximum number of entries per message

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