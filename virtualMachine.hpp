#ifndef VIRTUALMACHINE
#define VIRTUALMACHINE
#include <string>
#include <unordered_map>
#include "parser.hpp"
#include <array>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
enum RegisterKinds {
	Register00,
	Register01,
	Register02,
	Register03,
	Register04,
	Register05,
	Register06,
	Register07,
	Register08,
	Register09,
	Register10,
	Register11,
	Register12,
	Register13,
	Register14,
	Register15,
	Register16,
	Register17,
	Register18,
	Register19,
	Register20,
	Register21,
	Register22,
	Register23,
	Register24,
	Register25,
	Register26,
	Register27,
	Register28,
	Register29,
	Register30,
	Register31,
	ProgramCounter,
	RegisterHi,
	RegisterLo
};

typedef std::array<int32_t, 35> RegisterSetType;
typedef std::unordered_map<std::string, size_t> LabelMapType;
//typedef std::vector<uint8_t> MemoryType;
typedef std::array<uint8_t, 512> MemorySet;
template<typename T>
class message_queue {
public:
	void push(const T & value) {
		std::unique_lock<std::mutex> lock(the_mutex);
		the_queue.push(value);
		lock.unlock();
		the_condition_variable.notify_one();
	}
	bool empty() const {
		std::lock_guard<std::mutex> lock(the_mutex);
		return the_queue.empty();
	}
	bool try_pop(T & popped_value) {
		std::lock_guard<std::mutex> lock(the_mutex);
		if (the_queue.empty()) {
			return false;
		}
		popped_value = the_queue.front();
		the_queue.pop();
		return true;
	}
	void wait_and_pop(T &popped_value) {
		std::unique_lock<std::mutex> lock(the_mutex);
		while (the_queue.empty()) {
			the_condition_variable.wait(lock);
		}
		popped_value = the_queue.front();
		the_queue.pop();
	}
private:
	std::queue<T> the_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;
};

typedef string Message;
typedef message_queue<Message> MessageQueue;

class VirtualMachine {
public:
	VirtualMachine();
	void run(MessageQueue*myqueue);
	void breakRun();
	void threadSet(MessageQueue*myqueue);
	void setValues(const LabelMapType&x, const MemoryType& y, const instructionSet& z, const immidiateSet & zz 
		, const TextLabelType & zzz, const vector<immidiate> & zzzz, const size_t &lineNumber);
	uint32_t getPc();
	instructionSet getInstuctions();
	void pcSimulate();
	bool isError();
	RegisterSetType getRegisters();
	size_t getInstructionSize();
	size_t getfirstTextLine();
	RegisterKinds getRegister(const string& str);
	int32_t printRegisters(string str);
	uint8_t getMemoryAdd(uint32_t num);
	~VirtualMachine();

private:
	RegisterSetType Registers;
	MemorySet MemoryMap;
	LabelMapType Labels;
	instructionSet Instructions;
	immidiateSet immidiateValues;
	TextLabelType textLabels;
	vector<immidiate> mymainVec;
	bool isRegisterF;

	size_t mainLine;
	size_t firstInstructLine;
	std::thread th;
	
};

#endif