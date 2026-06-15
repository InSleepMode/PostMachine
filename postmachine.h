#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <map>

struct Rule {
	int currentStroke; //номер строки
	char command; // буква команды
	int destinationStroke; // переход к строке
	int conditionalOperator; // для условного оператора
	int secondDestinationStroke; // для второго условия
};



class Program {
private:

	Rule* rules;
	size_t size_;
	size_t maxSize = 0;

public:

	Program() : rules(nullptr), size_(0), maxSize(0) {}

	Program(const Program& other);

	void enhance(size_t newMaxsize);

	void add(const Rule& rule);

	void swap(Program& other) noexcept;

	Program& operator=(const Program& other);

	const Rule* getRules() const;

	size_t size() const;

	~Program();
};

class PostMachine {

private:
	unsigned long long headPos;
	long long sizeOfTape;
	Program program;
	int programCounter;
	std::map<int, int> lineToIndex;


public:

	PostMachine(long long tapeSize, const Program& prog);

	PostMachine(const PostMachine& other);
	
	void swap(PostMachine& other) noexcept;

	PostMachine& operator =(const PostMachine& other);

	static void moveRight(unsigned long long& headPos, unsigned long long tapeSize);

	static void moveLeft(unsigned long long& headPos, unsigned long long tapeSize);

	std::string calc(std::string const& init, int maxSteps);

	
};


