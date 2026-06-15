#include "postmachine.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cerr << "Cannot start PM " << std::endl;
        return 1;
    }

    std::ifstream inFile(argv[1]);
    if (!inFile) {
        std::cerr << "Could not open Rulefile" << std::endl;
        return 1;
    }

    
    std::ifstream inFileInput(argv[2]);
    if (!inFileInput) {
        std::cerr << "Could not open Inputfile" << std::endl;
        return 1;
    }

    

    std::ofstream outFile(argv[3]);
    if (!outFile) {
        std::cerr << "Could not write in file" << std::endl;
        return 1;
    }

    long long tapeSize;
    long long maxSteps;
    std::string input_information;

    inFileInput >> tapeSize >> maxSteps >> input_information;

    std::string tape(tapeSize, '0');
    for (size_t i = 0; i < input_information.size() && i < tape.size(); ++i) {
        tape[i] = input_information[i];
    }


    char ch;
    bool isComment = false;
    int ruleCnt = 0;
    const int ruleSize = 1000;
    Rule* ruleMassive = new Rule[ruleSize]();

    while (inFile.get(ch) && ruleCnt < ruleSize) {

        if (ch == '%') {
            isComment = true;
        }
        if (isComment) {
            if (ch == '\n') {
                isComment = false;
            }
            continue;
        }

        if (ch == ' ' || ch == '\n') {
            continue;
        }
        
        if (isdigit(ch) != 0) {
            std::string number_current_stroke;

            number_current_stroke += ch;
            
            //проверим еще один разряд
            while (inFile.get(ch) && (isdigit(ch) != 0)) {
                number_current_stroke += ch;
            }
            ruleMassive[ruleCnt].currentStroke = std::stoi(number_current_stroke);
        }

        while (inFile && (ch == ' ' || ch == '.')) {
            inFile.get(ch);
        }
        
        if (ch == '%') {
            isComment = true;
            continue;
        }

        ruleMassive[ruleCnt].command = ch;

        inFile.get(ch);

        if (ruleMassive[ruleCnt].command == '!') {
            ruleMassive[ruleCnt].destinationStroke = 0;
            ruleMassive[ruleCnt].secondDestinationStroke = 0;

            ruleCnt++;

            continue;
        }

        while (inFile && ch == ' ') {
            inFile.get(ch);
        }
        if (!inFile) {
            break;
        }
        
        if (ruleMassive[ruleCnt].command == '?') {
            
            while (ch == ' '){
                inFile.get(ch);
            }
            std::string destination_stroke;

            if (isdigit(ch) != 0) {

                destination_stroke += ch;
                
                //проверим еще один разряд
                while (inFile.get(ch) && (isdigit(ch) != 0)) {
                    destination_stroke += ch;
                }
                ruleMassive[ruleCnt].destinationStroke = std::stoi(destination_stroke);
            }
            
            while (ch == ' ') {
                inFile.get(ch);
            }
            std::string second_destination_stroke;

            if ((isdigit(ch) != 0)) {
                second_destination_stroke += ch;
                
                //проверим еще один разряд
                while (inFile.get(ch) && (isdigit(ch) != 0)) {
                    second_destination_stroke += ch;
                }
                ruleMassive[ruleCnt].secondDestinationStroke = std::stoi(second_destination_stroke);
            }

            ruleCnt++;
        }
        else{

            while (ch == ' ') {
                inFile.get(ch);
            }

            std::string number_destination_stroke;
            if (isdigit(ch) != 0) {
                number_destination_stroke += ch;

                //проверим еще один разряд
                while (inFile.get(ch) && (isdigit(ch) != 0)) {
                    number_destination_stroke += ch;

                }
                ruleMassive[ruleCnt].destinationStroke = std::stoi(number_destination_stroke);
            }

            ruleMassive[ruleCnt].secondDestinationStroke = 0;

            ruleCnt++;
            
            while (inFile && ch != '\n') {
                inFile.get(ch);
            }
        }
    }

    Program prog;

    for (int i = 0; i < ruleCnt; ++i) {
        prog.add(ruleMassive[i]);
    }    


    PostMachine pm(tapeSize, prog);
    std::string result = pm.calc(tape, static_cast<int>(maxSteps));

    outFile << result;

    inFile.close();
    outFile.close();
    delete[] ruleMassive;
    return 0;
}
