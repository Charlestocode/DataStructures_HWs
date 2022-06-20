#include <string.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
#define TEXT_RECORD_MAX_LENGTH 69


class OperationCodeTable {
    private:
        unordered_map<string, string> opcodeTable;

    public:
        void addOpCode(string mnemonic, string code) {
            opcodeTable.insert(pair<string, string>(mnemonic, code));
        }

        string getOpCode(string mnemonic) {
            return opcodeTable[mnemonic];
        }

        int contains(string mnemonic) {
            return opcodeTable.count(mnemonic);
        }
};


class SymbolTable {
    public:
        unordered_map<string, int> symbolTable;

        void addSymAddr(string symbol, int addr) {
            symbolTable.insert(pair<string, int>(symbol, addr));
        }

        int getSymAddr(string symbol) {
            return symbolTable[symbol];
        }

        int contains(string symbol) {
            return symbolTable.count(symbol);
        }
};


void readSourceLine(fstream &file, string &label, string &mnemonic, string &operand) {

    label = "";
    mnemonic = "";
    operand = "";
    string text;
    char *word;

    getline(file, text);
    char charArr[text.length() + 1];
    strcpy(charArr, text.c_str());
    

    // Check if there is a label on this line. Initialize the label as empty string, "".
    // If there is a new label which is not in symbol table, add the label into symbol table.
    if (text[0] != '\t') {   // If the first character is not Tab character, it means there is a label.
        word = strtok(charArr, " \t\r");
        label = word;
    
        // Get the mnemonic.
        word = strtok(NULL, " \t\r");
        mnemonic = word;

        // Check if there is an operand on this line. 
        // If there is an operand, store the operand value in integer.
        word = strtok(NULL, " \t\r");
        if (word != NULL) {
            operand = word;
        }
    }
    else {
        // Get the mnemonic.
        word = strtok(charArr, " \t\r");
        mnemonic = word;

        // Check if there is an operand on this line. 
        // If there is an operand, store the operand value in integer.
        word = strtok(NULL, " \t\r");
        if (word != NULL) {
            operand = word;
        }
    }
}


void readIntermediateLine(fstream &file, string &location, string &label, string &mnemonic, string &operand) {

    location = "";
    label = "";
    mnemonic = "";
    operand = "";
    string text;
    char *word;

    getline(file, text);
    char charArr[text.length() + 1];
    strcpy(charArr, text.c_str());  
    
    
    // Check if there is a location on this line. Initialize the label as empty string, "".
    if (text[0] != '\t') {   // If the first character is not Tab character, it means there is a location.
        word = strtok(charArr, " \t\r");
        location = word;
    
        // Get the label if it exists.
        if (text[location.length() + 1] != '\t') {
            word = strtok(NULL, " \t\r");
            label = word;
        }

        // Get the mnemonic.
        word = strtok(NULL, " \t\r");
        mnemonic = word;

        // Check if there is an operand on this line. 
        // If there is an operand, store the operand value in integer.
        word = strtok(NULL, " \t\r");
        if (word != NULL) {
            operand = word;
        }
    }
    else {
        // Get the mnemonic.
        word = strtok(charArr, " \t\r");
        mnemonic = word;

        // Check if there is an operand on this line. 
        // If there is an operand, store the operand value in integer.
        word = strtok(NULL, " \t\r");
        if (word != NULL) {
            operand = word;
        }
    }
}


int main() {

    fstream opcode_fs, source_fs, SICXEsource_fs;
    fstream intermediate_file_fs, symbolTable_fs;
    fstream source_object_code_fs, object_program_fs;
    opcode_fs.open("./opcode.txt", fstream::in);
    source_fs.open("./source.txt", fstream::in);
    SICXEsource_fs.open("./SICXEsource.txt", fstream::in);
    intermediate_file_fs.open("./intermediate_file.txt", fstream::out);
    symbolTable_fs.open("./symbol_table.txt", fstream::out);
    source_object_code_fs.open("./source_program_with_object_code.txt", fstream::out);
    object_program_fs.open("./object_program.txt", fstream::out);

    string text;
    int startAddr, programLength;
    OperationCodeTable opcodeTable;
    SymbolTable symTable;
    
    
    // Read mnemonic table.
    if (opcode_fs.is_open()) {
        while (getline(opcode_fs, text)) {

            string mnemonic;
            string code;

            char charArr[text.length() + 1];
            strcpy(charArr, text.c_str());

            char *word = strtok(charArr, " ");
            mnemonic = word;
            word = strtok(NULL, " ");
            code = word;

            opcodeTable.addOpCode(mnemonic, code);
        }

        opcode_fs.close();
    }

    
    /* ------------------------------------------ Pass 1 ------------------------------------------- */
    // Read source code.
    if (source_fs.is_open() && intermediate_file_fs.is_open()) {

        string label, mnemonic, operand;
        // Declare a location counter
        int locCtr;


        // Read first input line.
        readSourceLine(source_fs, label, mnemonic, operand);
        if (mnemonic == "START") {
            startAddr = stoi(operand, nullptr, 16);
            locCtr = startAddr;
            // Write line to intermediate file.
            intermediate_file_fs << hex << uppercase << locCtr << '\t' << label << '\t' << mnemonic << '\t' << operand << '\n';

            // Read next input line.
            readSourceLine(source_fs, label, mnemonic, operand);
        }
        else {
            locCtr = 0;
        }
        

        while (mnemonic != "END") {

            // Check if there is a label on this line. Initialize the label as empty string, "".
            // If there is a new label which is not in symbol table, add the label into symbol table.
            if (label != "") {
                if (symTable.contains(label)) {
                    perror("Duplicate symbol");
                }
                else {
                    symTable.addSymAddr(label, locCtr);
                }
            }

            int nextLocCtr;
            if (opcodeTable.contains(mnemonic)) {
                nextLocCtr = locCtr + 3;
            }
            else if (mnemonic == "WORD") {
                nextLocCtr = locCtr + 3;
            }
            else if (mnemonic == "RESW") {
                nextLocCtr = locCtr + 3 * stoi(operand, nullptr, 10);
            }
            else if (mnemonic == "RESB") {
                nextLocCtr = locCtr + stoi(operand, nullptr, 10);
            }
            else if (mnemonic == "BYTE") {
                if (operand[0] == 'X') {
                    int length = (operand.length() - 3) / 2;
                    nextLocCtr = locCtr + length;
                }
                else if (operand[0] == 'C') {
                    int length = operand.length() - 3;
                    nextLocCtr = locCtr + length;
                }
            }
            else {
                perror("Invalid mnemonic code");
            }
            
            // Write line to intermediate file.
            intermediate_file_fs << hex << uppercase << locCtr << '\t' << label << '\t' << mnemonic << '\t' << operand << '\n';
            // Read next input line.
            readSourceLine(source_fs, label, mnemonic, operand);
            locCtr = nextLocCtr;
        }
        

        // Write last line to intermediate file.
        intermediate_file_fs << '\t' << label << '\t' << mnemonic << '\t' << operand << endl;
        programLength = locCtr - startAddr;

        source_fs.close();
        intermediate_file_fs.close();
    }
    
    
    // Output the symbol table.
    if (symbolTable_fs.is_open()) {
        unordered_map<string, int>::iterator it;
        for (it = symTable.symbolTable.begin(); it != symTable.symbolTable.end(); it++) {
            symbolTable_fs << it->first << '\t' << hex << uppercase << it->second << '\n';
        }
    }



    /* ------------------------------------------ Pass 2 ------------------------------------------- */
    intermediate_file_fs.open("./intermediate_file.txt", fstream::in);
    if (intermediate_file_fs.is_open() && source_object_code_fs.is_open() && object_program_fs.is_open()) {

        string location, label, mnemonic, operand;
        string textRecord, buffer;
        string endRecord;
        stringstream ss;
        int curLenTextRecord;
        int startAddrOnLine = startAddr;


        // Read first input line from intermeidate file.
        readIntermediateLine(intermediate_file_fs, location, label, mnemonic, operand);
        if (mnemonic == "START") {
            // Write line to source_object_code file.
            source_object_code_fs << location << '\t' << label << '\t' << mnemonic << '\t' << operand << '\n';
            // Write Header record to object program.
            object_program_fs << 'H' << label << '\t' << hex << uppercase << setfill('0') << setw(6) << startAddr << setfill('0') << setw(6) << programLength << '\n';

            // Read next intput line.
            readIntermediateLine(intermediate_file_fs, location, label, mnemonic, operand);
        }
        // Initialize the Text record with "T" and starting address on the current line.
        ss << "T" << hex << uppercase << setfill('0') << setw(6) << startAddrOnLine;
        ss >> textRecord;
        ss.clear();
        curLenTextRecord = 9;
        
        
        while (mnemonic != "END") {

            int opcode, operandAddr;
            int objectCode, curSizeObjectCode;
            int indexFlag = 0;
            string processedOperand;

            if (opcodeTable.contains(mnemonic)) {

                /* Set `operandAddr` to symbol value if there is a symbol in operand field.
                ** Otherwise, set `operandAddr` to zero. */
                if (operand != "") {

                    // Preprocess operand if the operand ends with ",X" substring.
                    if (operand.length() > 2 && operand.substr(operand.length() - 2, 2) == ",X") {
                        processedOperand = operand.substr(0, operand.length() - 2);
                        indexFlag = 1;
                    } else {
                        processedOperand = operand;
                    }

                    // If the operand is in symbol table.
                    if (symTable.contains(processedOperand)) {
                        operandAddr = symTable.getSymAddr(processedOperand);
                    } else {
                        operandAddr = 0;
                        perror("Undefined symbol");
                    }
                } else {
                    operandAddr = 0;
                }


                /* Assemble the object code instruction. */
                opcode = stoi(opcodeTable.getOpCode(mnemonic), nullptr, 16) << 16;
                // Simple addressing
                if (indexFlag == 0) {
                    objectCode = opcode + operandAddr;
                }
                // Indexed addressing
                else {
                    // The index flag is of bit 15.
                    objectCode = opcode + (1 << 15) + operandAddr;
                }
                curSizeObjectCode = 3;
                curLenTextRecord += 2 * curSizeObjectCode;
                
                // Write line to source_object_code file.
                source_object_code_fs << hex << uppercase << location << '\t' << label << '\t' << mnemonic << '\t' << operand << '\t' << setfill('0') << setw(6) << objectCode << '\n';
            }

            else if (mnemonic == "BYTE" || mnemonic == "WORD") {

                // Write line to source_object_code file.
                source_object_code_fs << hex << uppercase << location << '\t' << label << '\t' << mnemonic << '\t' << operand << '\t';

                // Convert constant to object code.
                // For BYTE.
                if (operand[0] == 'X') {
                    processedOperand = operand.substr(2, operand.length() - 3);
                    objectCode = stoi(processedOperand, nullptr, 16);
                    source_object_code_fs << setfill('0') << setw(2) << objectCode << '\n';

                    curSizeObjectCode = 1;
                    curLenTextRecord += 2 * curSizeObjectCode;
                }
                else if (operand[0] == 'C') {
                    processedOperand = operand.substr(2, operand.length() - 3);
                    string::iterator it;
                    for (it = processedOperand.begin(); it != processedOperand.end(); it++) {
                        objectCode = (objectCode << 8) + *it;
                    }
                    source_object_code_fs << setfill('0') << setw(2) << objectCode << '\n';

                    curSizeObjectCode = processedOperand.length();
                    curLenTextRecord += 2 * curSizeObjectCode;
                }
                // For WORD.
                else {
                    objectCode = stoi(operand, nullptr, 10);
                    source_object_code_fs << setfill('0') << setw(6) << objectCode << '\n';

                    curSizeObjectCode = 3;
                    curLenTextRecord += 2 * curSizeObjectCode;
                }
            }
            // For mnemonic "RESW" and "RESB".
            else {
                source_object_code_fs << location << '\t' << label << '\t' << mnemonic << '\t' << operand << '\n';

                curSizeObjectCode = stoi(operand, nullptr, 16);
            }

            
            // If object code will not fit into the current Text record, write Text record to object program.
            if (curLenTextRecord > TEXT_RECORD_MAX_LENGTH || ((mnemonic == "RESW" || mnemonic == "RESB") && ss.rdbuf()->in_avail() != 0)) {

                ss >> buffer;
                ss.clear();
                int sizeObjectCodeOnLine = (curLenTextRecord - 9 - 2 * curSizeObjectCode) / 2;
                ss << textRecord << hex << uppercase << setfill('0') << setw(2) << sizeObjectCodeOnLine << buffer;
                ss >> textRecord;
                ss.clear();
                object_program_fs << textRecord << '\n';

                // Inititialize new Text record.
                startAddrOnLine = stoi(location, nullptr, 16);
                ss << "T" << hex << uppercase << setfill('0') << setw(6) << startAddrOnLine;
                ss >> textRecord;
                ss.clear();

                curLenTextRecord = 9;
                if (mnemonic != "RESW" && mnemonic != "RESB") {
                    curLenTextRecord += 2 * curSizeObjectCode;
                }
            }


            // Add object code to Text record.
            if (mnemonic != "RESW" && mnemonic != "RESB") {
                ss << hex << uppercase << setfill('0') << setw(2 * curSizeObjectCode) << objectCode;
            }
            

            // Read next input line.
            readIntermediateLine(intermediate_file_fs, location, label, mnemonic, operand);
        }
        

        // Write end line to source_object_code file.
        source_object_code_fs << '\t' <<  '\t' << mnemonic << '\t' << operand << '\n';
        // Write last Text record to object program.
        ss >> buffer;
        ss.clear();
        int sizeObjectCodeOnLine = (curLenTextRecord - 9) / 2;
        ss << textRecord << hex << uppercase << setfill('0') << setw(2) << sizeObjectCodeOnLine << buffer;
        ss >> textRecord;
        ss.clear();
        object_program_fs << textRecord <<'\n';


        // Write End record to object program.
        ss << "E" << hex << uppercase << setfill('0') << setw(6) << startAddr;
        ss >> endRecord;
        object_program_fs << endRecord <<'\n';


        intermediate_file_fs.close();
        source_object_code_fs.close();
        object_program_fs.close();
    }


    SICXEsource_fs.close();
    return 0;
}