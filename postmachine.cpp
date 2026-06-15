#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "postmachine.h"



Program::Program(const Program& other) : size_(other.size_), maxSize(other.maxSize) {
	rules = new Rule[maxSize];
	for (size_t i = 0; i < size_; ++i) {
		rules[i] = other.rules[i];
	}
}

void Program::enhance(size_t newMaxsize) {
	Rule* newRules = new Rule[newMaxsize];
	for (size_t i = 0; i < size_; ++i) {
		newRules[i] = rules[i];
	}
	delete[] rules;
	rules = newRules;
	maxSize = newMaxsize;
}

void Program::add(const Rule& rule) {
	if (size_ >= maxSize) {
		enhance(size_ == 0 ? 1 : size_ * 2);
	}
	rules[size_++] = rule;
}


void Program::swap(Program& other) noexcept {
	std::swap(rules, other.rules);
	std::swap(size_, other.size_);
	std::swap(maxSize, other.maxSize);
}

Program& Program::operator=(const Program& other) {
	if (this != &other) {
		Program temp(other);
		swap(temp);
	}
	return *this;
}

const Rule* Program::getRules() const {
	return rules;
}

size_t Program::size() const {
	return size_;
}

Program::~Program() {
	delete[] rules;
}



//PostMachine class

PostMachine::PostMachine(long long tapeSize, const Program& prog) :
	headPos(0), sizeOfTape(tapeSize), program(prog), programCounter(0) {
	
	const Rule* rules = program.getRules();
	for (size_t i = 0; i < program.size(); ++i) {
		lineToIndex[rules[i].currentStroke] = static_cast<int>(i);
	}

}

PostMachine::PostMachine(const PostMachine& other) :
	headPos(other.headPos), sizeOfTape(other.sizeOfTape), program(other.program), programCounter(other.programCounter) {}

void PostMachine::swap(PostMachine& other) noexcept {
	std::swap(headPos, other.headPos);
	std::swap(sizeOfTape, other.sizeOfTape);
	std::swap(program, other.program);
	std::swap(programCounter, other.programCounter);
}

PostMachine& PostMachine::operator =(const PostMachine& other) {
	if (this != &other) {
		PostMachine temp(other);
		swap(temp);

	}
	return *this;
}

void PostMachine::moveRight(unsigned long long& headPos, unsigned long long tapeSize) {
	headPos = (headPos + 1) % tapeSize;
}

void PostMachine::moveLeft(unsigned long long& headPos, unsigned long long tapeSize) {
	headPos = (headPos - 1 + tapeSize) % tapeSize;
}


std::string PostMachine::calc(std::string const& init, int maxSteps) {

	std::string Lenta = init;

	int startLine = -1;

	auto it = lineToIndex.begin();
	
	int counter = it->second;

	headPos = 0;

	bool stopped = false;
	
	const Rule* rules = program.getRules();

	for (int i = 0; i < maxSteps; ++i) {

		const Rule& rule = rules[counter];
		
		switch (rule.command) {

		case 'V':
			Lenta[headPos] = '1';
			counter = lineToIndex[rule.destinationStroke];
			break;
		case 'X':
			Lenta[headPos] = '0';
			counter = lineToIndex[rule.destinationStroke];
			break;

		case 'L':
			moveLeft(headPos, sizeOfTape);
			counter = lineToIndex[rule.destinationStroke];
			break;

		case 'R':
			moveRight(headPos, sizeOfTape);
			counter = lineToIndex[rule.destinationStroke];
			break;

		case '?':
			if (Lenta[headPos] == '1') {
				if (lineToIndex.find(rule.secondDestinationStroke) == lineToIndex.end()) {
					return "Error";
				}
				counter = lineToIndex[rule.secondDestinationStroke];
			}
			else {
				if (lineToIndex.find(rule.destinationStroke) == lineToIndex.end()) {
					return "Error";
				}
				counter = lineToIndex[rule.destinationStroke];
			}
			break;

		case '!':

			stopped = true;
			break;

		default:
			std::cerr << "Unknown command" << '\n';

		}

		if (stopped) {
			break;
		}

	}
	if (!stopped) {
		return "Not applicable";
	}


	unsigned long long lastOne = -1;
	unsigned long long current = headPos;
	for (unsigned long long i = 0; i < Lenta.size(); ++i) {
		if (Lenta[current] == '1') {
			lastOne = current;
		}
		current = (current + 1) % Lenta.size();
	}

	if (lastOne == -1) {
		return "0";
	}
	std::string result;

	//выведем получившуюся ленту
	if (lastOne >= headPos) {
		for (unsigned long long i = headPos; i <= lastOne; ++i) {
			result += Lenta[i];
		}
	}
	else {
		//учитывая что ленца кольцевая выводим от начала головки до конца ленты
		for (unsigned long long i = headPos; i < Lenta.size(); ++i) {
			result += Lenta[i];
		}
		//так же от начала до позиции последней единицы 
		for (unsigned long long i = 0; i <= lastOne; ++i) {
			result += Lenta[i];
		}
	}

	return result;
}




