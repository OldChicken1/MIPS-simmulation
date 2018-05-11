#include "virtualMachine.hpp"

VirtualMachine::VirtualMachine()
{
	for (size_t i = 0; i < 35; i++) {
		Registers[i] = 0x00000000;
	}
	for (size_t i = 0; i < 512; i++) {
		MemoryMap[i] = 0;
	}
	isRegisterF = true;

}




void VirtualMachine::run(MessageQueue*mq)
{
	Message m;
	while (true) {
		mq->wait_and_pop(m);
		if (m=="run") {
			while (true) {
				if (getPc() < getInstructionSize()) {
					pcSimulate();
				}
				if (mq->try_pop(m)) {
					break;
				}
			}
			
		}
		else if(m=="quit")
		{
			break;
		}

	}
	/*while (!mq->try_pop(m)) {
		if (getPc() < getInstructionSize()) {
			pcSimulate();
		}
	}
	breakRun(std::ref(mq));*/
}

void VirtualMachine::breakRun()
{
	if (!th.joinable()) {
		cout << "Error: Thread not joinable" << endl;
	}
	else {
		th.join();
	}
	
}

void VirtualMachine::threadSet(MessageQueue*myqueue)
{
	th = std::thread(&VirtualMachine::run, this, std::ref(myqueue));
}


void VirtualMachine::setValues(const LabelMapType & x, const MemoryType & y, const instructionSet & z
	, const immidiateSet & zz, const TextLabelType & zzz, const vector<immidiate> & zzzz, const size_t&lineNumber)
{
	for (size_t i = 0; i < y.size(); i++) {
		MemoryMap[i] = y[i];
	}
	Labels = x;
	Instructions = z;
	immidiateValues = zz;
	textLabels = zzz;
	mymainVec = zzzz;
	for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
		if (it->name == "main") {
			mainLine = it->value;
		}
	}
	firstInstructLine = lineNumber;
}


uint32_t VirtualMachine::getPc()
{
	return Registers[32];
}


instructionSet VirtualMachine::getInstuctions()
{
	return Instructions;
}


void VirtualMachine::pcSimulate()
{
	uint32_t a = 0x00000000;
	uint32_t b = 0x00000000;
	uint32_t c = 0x00000000;
	uint32_t d = 0x00000000;
	instruction ins;
	string label;
	string str;
	RegisterKinds reg;
	size_t index;
	ins = Instructions[Registers[32]];
	if (ins.op == NOP) {
		Registers[32]++;
	}
	else if (ins.op == LoadWord) {
		if (ins.args.size() > 4) {
			str = ins.args[0].value();
			reg = getRegister(str);
			size_t offset = stoi(ins.args[2].value());
			label = ins.args[4].value();
			RegisterKinds reg2;
			reg2 = getRegister(label);
			getRegister(label);
			if (isRegisterF) {
				Registers[reg] = MemoryMap[Registers[reg2]];
				uint32_t newAddress = Registers[reg2] + offset;
				uint32_t loadValue = MemoryMap[newAddress + 3];
				loadValue = loadValue << 8;
				loadValue = loadValue | (MemoryMap[newAddress + 2]);
				loadValue = loadValue << 8;
				loadValue = loadValue | (MemoryMap[newAddress + 1]);
				loadValue = loadValue << 8;
				loadValue = loadValue | (MemoryMap[newAddress]);
				Registers[reg] = loadValue;
			}
			else {
				LabelMapType::iterator it;
				it = Labels.find(label);
				if (it != Labels.end()) {
					uint32_t newAddress = it->second + offset;
					uint32_t loadValue = MemoryMap[newAddress + 3]; ;
					loadValue = loadValue << 8;
					loadValue = loadValue | (MemoryMap[newAddress + 2]);
					loadValue = loadValue << 8;
					loadValue = loadValue | (MemoryMap[newAddress + 1]);
					loadValue = loadValue << 8;
					loadValue = loadValue | (MemoryMap[newAddress]);
					Registers[reg] = loadValue;
				}
			}
		}
		else{
			str = ins.args[0].value();
			reg = getRegister(str);
			label = ins.args[2].value();
			RegisterKinds reg2;
			reg2 = getRegister(label);
			getRegister(label);
			if (isRegisterF) {
				Registers[reg] = MemoryMap[Registers[reg2]];
			}
			else {
				LabelMapType::iterator it;
				it = Labels.find(label);
				if (it != Labels.end()) {
					index = it->second;
					a = (a | MemoryMap[index+3]);
					a = a << 8;
					a = (a | MemoryMap[index + 2]);
					a = a << 8;
					a = (a | MemoryMap[index + 1]);
					a = a << 8;
					a = (a | MemoryMap[index]);
					Registers[reg] = a;//not sure
				}
			}
			
		}
		Registers[32]++;
	}
	else if (ins.op == LoadImmid) {
		bool isFound = false;
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
			if (it->name == label) {
				isFound = true;
				a = uint32_t(it->value);
			}
		}
		if (!isFound) {
			a = uint32_t(stoi(label));
			Registers[reg] = a;
		}
		Registers[reg] = a;
		Registers[32]++;
	}
	else if (ins.op == LoadAdd) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		LabelMapType::iterator it;
		it = Labels.find(label);
		if (it != Labels.end()) {
			index = it->second;
			Registers[reg] = uint32_t(index);//not sure
		}
		Registers[32]++;
	}
	else if (ins.op == StoreWord) {
		size_t num;
		bool isLabelF = false;
		uint8_t first = 0;
		/*uint8_t second = 0;
		uint8_t third = 0;
		uint8_t fourth = 0;*/
		
		if (ins.args.size() > 4) {
			str = ins.args[0].value();
			reg = getRegister(str);
			size_t offset = stoi(ins.args[2].value());
			label = ins.args[4].value();
			RegisterKinds reg2;
			reg2 = getRegister(label);
			getRegister(label);
			if (isRegisterF) {
				uint32_t newAddress = Registers[reg2] + offset;
				first = Registers[reg] | first;
				MemoryMap[newAddress] = first;
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 8;
				MemoryMap[newAddress] = uint8_t(first);
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 16;
				MemoryMap[newAddress] = uint8_t(first);
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 24;
				MemoryMap[newAddress] = uint8_t(first);
			}
			else {
				LabelMapType::iterator it;
				it = Labels.find(label);
				if (it != Labels.end()) {
					num = it->second;
				}
				uint32_t newAddress = num + offset;
				first = Registers[reg] | first;
				MemoryMap[newAddress] = first;
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 8;
				MemoryMap[newAddress] = uint8_t(first);
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 16;
				MemoryMap[newAddress] = uint8_t(first);
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 24;
				MemoryMap[newAddress] = uint8_t(first);
			}
			
		}
		else {
			str = ins.args[0].value();
			reg = getRegister(str);
			label = ins.args[2].value();
			RegisterKinds reg2;
			reg2 = getRegister(label);
			getRegister(label);
			if (isRegisterF) {
				uint32_t newAddress = Registers[reg2];
				first = Registers[reg] | first;
				MemoryMap[newAddress] = first;
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 8;
				MemoryMap[newAddress] = uint8_t(first);
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 16;
				MemoryMap[newAddress] = uint8_t(first);
				newAddress++;
				first = -1;
				first &= Registers[reg] >> 24;
				MemoryMap[newAddress] = uint8_t(first);
			}
			else {
				LabelMapType::iterator it;
				it = Labels.find(label);
				if (it != Labels.end()) {
					num = it->second;
					isLabelF = true;
				}
				if (!isLabelF) {
					num = stoi(label);
				}
				first = Registers[reg] | first;
				MemoryMap[num] = uint8_t(first);
				num++;
				first = -1;
				first &= Registers[reg] >> 8;
				MemoryMap[num] = uint8_t(first);
				num++;
				first = -1;
				first &= Registers[reg] >> 16;
				MemoryMap[num] = uint8_t(first);
				num++;
				first = -1;
				first &= Registers[reg] >> 24;
				MemoryMap[num] = uint8_t(first);
			}
			
		}
		Registers[32]++;
	}
	else if (ins.op == Move) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		RegisterKinds reg2;
		reg2 = getRegister(label);
		Registers[reg] = Registers[reg2];
		Registers[32]++;
	}
	else if (ins.op == MoveFromHi) {
		str = ins.args[0].value();
		reg = getRegister(str);
		Registers[reg] = Registers[33];
		Registers[32]++;
	}
	else if (ins.op == MoveFromLo) {
		str = ins.args[0].value();
		reg = getRegister(str);
		Registers[reg] = Registers[34];
		Registers[32]++;
	}
	else if (ins.op == AddS) {
		bool isImmidF = false;
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string label2;
		RegisterKinds reg2;
		RegisterKinds reg3;
		label2 = ins.args[4].value();
		getRegister(label2);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(label2);
			Registers[reg] = Registers[reg2] + Registers[reg3];
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (it->name == label2) {
					uint32_t addNum = it->value;
					Registers[reg] = Registers[reg2] + addNum;
					isImmidF = true;
				}
			}
			if (!isImmidF) {
				Registers[reg] = Registers[reg2] + stoi(label2);
			}
		}
		Registers[32]++;
	}
	else if (ins.op == AddU) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string label2;
		RegisterKinds reg2;
		RegisterKinds reg3;
		label2 = ins.args[4].value();
		getRegister(label2);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(label2);
			Registers[reg] = Registers[reg2] + Registers[reg3];
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (it->name == label2) {
					uint32_t addNum = it->value;
					Registers[reg] = Registers[reg2] + addNum;
				}
			}
			
		}
		Registers[32]++;
	}
	else if (ins.op == SubS) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string label2;
		RegisterKinds reg2;
		RegisterKinds reg3;
		label2 = ins.args[4].value();
		getRegister(label2);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(label2);
			Registers[reg] = Registers[reg2] +(~Registers[reg3]+1);
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (it->name == label2) {
					uint32_t addNum = it->value;
					Registers[reg] = Registers[reg2] + (~addNum+1);
				}
			}
		}
		Registers[32]++;
	}
	else if (ins.op == SubU) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string label2;
		RegisterKinds reg2;
		RegisterKinds reg3;
		label2 = ins.args[4].value();
		getRegister(label2);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(label2);
			Registers[reg] = Registers[reg2] + (~Registers[reg3] + 1);
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (it->name == label2) {
					uint32_t addNum = it->value;
					Registers[reg] = Registers[reg2] + (~addNum + 1);
				}
			}
		}
		Registers[32]++;
	}
	else if (ins.op == MultS) {
		int64_t mult = 0;
		int32_t hi = -1;
		int32_t lo = 0;
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		RegisterKinds reg2;
		reg2 = getRegister(label);
		mult = Registers[reg] * Registers[reg2];
		hi &= mult >> 32;
		lo = lo | mult;
		if (lo == 0) {
			hi = 1;
		}
		if ((Registers[reg]<0 || Registers[reg2]<0) && mult==0) {
			hi = -1;
		}
		Registers[33] = hi;
		Registers[34] = lo;
		Registers[32]++;
	}
	//else if (ins.op == MultU) {
	//	int64_t mult = 0;
	//	int32_t hi = 0;
	//	int32_t lo = 0;
	//	str = ins.args[0].value();
	//	reg = getRegister(str);
	//	label = ins.args[2].value();
	//	RegisterKinds reg2;
	//	reg2 = getRegister(label);
	//	mult = Registers[reg] * Registers[reg2];
	//	hi = int32_t((mult & 0xffffffff00000000) >> 8);
	//	lo = int32_t(mult & 0x00000000ffffffff);
	//	Registers[33] = hi;
	//	Registers[34] = lo;
	//	Registers[32]++;
	//}
	else if (ins.op == Div2S) {
		int32_t quoti = 0;
		int32_t remain = 0;
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		RegisterKinds reg2;
		reg2 = getRegister(label);
		if (Registers[reg2] != 0) {
			quoti = Registers[reg] / Registers[reg2];
			remain = Registers[reg] % Registers[reg2];
			Registers[33] = remain;
			Registers[34] = quoti;
		}
		Registers[32]++;
	}
	//else if (ins.op == Div2U) {
	//	int32_t quoti = 0;
	//	int32_t remain = 0;
	//	str = ins.args[0].value();
	//	reg = getRegister(str);
	//	label = ins.args[2].value();
	//	RegisterKinds reg2;
	//	reg2 = getRegister(label);
	//	if (Registers[reg2] != 0) {
	//		quoti = Registers[reg] / Registers[reg2];
	//		remain = Registers[reg] % Registers[reg2];
	//		Registers[33] = remain;
	//		Registers[34] = quoti;
	//	}
	//	Registers[32]++;
	//}
	else if (ins.op == And) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string imm = ins.args[4].value();
		RegisterKinds reg2;
		RegisterKinds reg3;
		getRegister(imm);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(imm);
			Registers[reg] = Registers[reg2] & Registers[reg3];
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (imm == it->name) {
					Registers[reg] = Registers[reg2] & int32_t(it->value);
				}
			}
		}
		Registers[32]++;
	}
	else if (ins.op == Nor) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string imm = ins.args[4].value();
		RegisterKinds reg2;
		RegisterKinds reg3;
		getRegister(imm);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(imm);
			Registers[reg] = ~(Registers[reg2] | Registers[reg3]);
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (imm == it->name) {
					Registers[reg] = ~(Registers[reg2] | int32_t(it->value));
				}
			}
		}
		Registers[32]++;
	}
	else if (ins.op == Not) {
		bool isFound = false;
		str = ins.args[0].value();
		reg = getRegister(str);
		RegisterKinds reg2;
		string imm = ins.args[2].value();
		for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
			if (imm == it->name) {
				isFound = true;
				Registers[reg] = ~(int32_t(it->value));
			}
		}
		if (!isFound) {
			reg2 = getRegister(imm);
			Registers[reg] = ~Registers[reg2];
		}
		Registers[32]++;
	}
	else if (ins.op == Or) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string imm = ins.args[4].value();
		RegisterKinds reg2;
		RegisterKinds reg3;
		getRegister(imm);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(imm);
			Registers[reg] = Registers[reg2] | Registers[reg3];
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (imm == it->name) {
					Registers[reg] = Registers[reg2] | int32_t(it->value);
				}
			}
		}
		Registers[32]++;
	}
	else if (ins.op == Xor) {
		str = ins.args[0].value();
		reg = getRegister(str);
		label = ins.args[2].value();
		string imm = ins.args[4].value();
		RegisterKinds reg2;
		RegisterKinds reg3;
		getRegister(imm);
		if (isRegisterF) {
			reg2 = getRegister(label);
			reg3 = getRegister(imm);
			Registers[reg] = Registers[reg2] ^ Registers[reg3];
		}
		else {
			reg2 = getRegister(label);
			for (immidiateSet::iterator it = immidiateValues.begin(); it != immidiateValues.end(); it++) {
				if (imm == it->name) {
					Registers[reg] = Registers[reg2] ^ int32_t(it->value);
				}
			}
		}
		Registers[32]++;
	}
	else if (ins.op == Jump) {
		size_t jumpLine;
		str = ins.args[0].value();
		if (str == "main") {
			Registers[32] = 0;
		}
		else {
			for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
				if (it->name == str) {
					jumpLine = it->value;
					Registers[32] = int32_t(jumpLine) ;
				}
			}
		}
		
	}
	else if (ins.op == BranchEq) {
		size_t jumpLine;
		string str2;
		string str3;
		RegisterKinds reg2;
		str = ins.args[0].value();
		str2 = ins.args[2].value();
		str3 = ins.args[4].value();
		reg = getRegister(str);
		reg2 = getRegister(str2);
		if (Registers[reg] == Registers[reg2]) {
			for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
				if (it->name == str3) {
					jumpLine = it->value;
					Registers[32] = int32_t(jumpLine) ;
				}
			}
		}
		else {
			Registers[32]++;
		}
	}
	else if (ins.op == BranchNe) {
		size_t jumpLine;
		string str2;
		string str3;
		RegisterKinds reg2;
		str = ins.args[0].value();
		str2 = ins.args[2].value();
		str3 = ins.args[4].value();
		reg = getRegister(str);
		reg2 = getRegister(str2);
		if (Registers[reg] != Registers[reg2]) {
			for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
				if (it->name == str3) {
					jumpLine = it->value;
					Registers[32] = int32_t(jumpLine);
				}
			}
		}
		else {
			Registers[32]++;
		}
	}
	else if (ins.op == BranchLt) {
		size_t jumpLine;
		string str2;
		string str3;
		RegisterKinds reg2;
		str = ins.args[0].value();
		str2 = ins.args[2].value();
		str3 = ins.args[4].value();
		reg = getRegister(str);
		reg2 = getRegister(str2);
		if (Registers[reg] < Registers[reg2]) {
			for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
				if (it->name == str3) {
					jumpLine = it->value;
					Registers[32] = int32_t(jumpLine);
				}
			}
		}
		else {
			Registers[32]++;
		}
	}
	else if (ins.op == BranchLe) {
		size_t jumpLine;
		string str2;
		string str3;
		RegisterKinds reg2;
		str = ins.args[0].value();
		str2 = ins.args[2].value();
		str3 = ins.args[4].value();
		reg = getRegister(str);
		reg2 = getRegister(str2);
		if (Registers[reg] <= Registers[reg2]) {
			LabelMapType::iterator it;
			it = Labels.find(str3);
			if (it != Labels.end()) {
				jumpLine = it->second-2;
				Registers[32] = int32_t(jumpLine);
			}
			else {
				for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
					if (it->name == str3) {
						jumpLine = it->value;
						Registers[32] = int32_t(jumpLine);
					}
				}
			}
			
		}
		else {
			Registers[32]++;
		}
	}
	else if (ins.op == BranchGt) {
		size_t jumpLine;
		string str2;
		string str3;
		RegisterKinds reg2;
		str = ins.args[0].value();
		str2 = ins.args[2].value();
		str3 = ins.args[4].value();
		reg = getRegister(str);
		reg2 = getRegister(str2);
		if (Registers[reg] > Registers[reg2]) {
			for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
				if (it->name == str3) {
					jumpLine = it->value;
					Registers[32] = int32_t(jumpLine);
				}
			}
		}
		else {
			Registers[32]++;
		}
	}
	else if (ins.op == BranchGe) {
		size_t jumpLine;
		string str2;
		string str3;
		RegisterKinds reg2;
		str = ins.args[0].value();
		str2 = ins.args[2].value();
		str3 = ins.args[4].value();
		reg = getRegister(str);
		reg2 = getRegister(str2);
		if (Registers[reg] >= Registers[reg2]) {
			for (vector<immidiate>::iterator it = mymainVec.begin(); it != mymainVec.end(); it++) {
				if (it->name == str3) {
					jumpLine = it->value;
					Registers[32] = int32_t(jumpLine);
				}
			}
		}
		else {
			Registers[32]++;
		}
	}
}

bool VirtualMachine::isError()
{
	if (!isRegisterF) {
		return true;
	}
	return false;
	
}

RegisterSetType VirtualMachine::getRegisters()
{
	return Registers;
}

size_t VirtualMachine::getInstructionSize()
{
	return Instructions.size();
}

size_t VirtualMachine::getfirstTextLine()
{
	return firstInstructLine;
}

RegisterKinds VirtualMachine::getRegister(const string& str)
{
	if (str == "$0" || str == "$zero") {
		isRegisterF = true;
		return Register00;
	}
	else if (str == "$1" || str == "$at") {
		isRegisterF = true;
		return Register01;
	}
	else if (str == "$2" || str == "$v0") {
		isRegisterF = true;
		return Register02;
	}
	else if (str == "$3" || str == "$v1") {
		isRegisterF = true;
		return Register03;
	}
	else if (str == "$4" || str == "$a0") {
		isRegisterF = true;
		return Register04;
	}
	else if (str == "$5" || str == "$a1") {
		isRegisterF = true;
		return Register05;
	}
	else if (str == "$6" || str == "$a2") {
		isRegisterF = true;
		return Register06;
	}
	else if (str == "$7" || str == "$a3") {
		isRegisterF = true;
		return Register07;
	}
	else if (str == "$8" || str == "$t0") {
		isRegisterF = true;
		return Register08;
	}
	else if (str == "$9" || str == "$t1") {
		isRegisterF = true;
		return Register09;
	}
	else if (str == "$10" || str == "$t2") {
		isRegisterF = true;
		return Register10;
	}
	else if (str == "$11" || str == "$t3") {
		isRegisterF = true;
		return Register11;
	}
	else if (str == "$12" || str == "$t4") {
		isRegisterF = true;
		return Register12;
	}
	else if (str == "$13" || str == "$t5") {
		isRegisterF = true;
		return Register13;
	}
	else if (str == "$14" || str == "$t6") {
		isRegisterF = true;
		return Register14;
	}
	else if (str == "$15" || str == "$t7") {
		isRegisterF = true;
		return Register15;
	}
	else if (str == "$16" || str == "$s0") {
		isRegisterF = true;
		return Register16;
	}
	else if (str == "$17" || str == "$s1") {
		isRegisterF = true;
		return Register17;
	}
	else if (str == "$18" || str == "$s2") {
		isRegisterF = true;
		return Register18;
	}
	else if (str == "$19" || str == "$s3") {
		isRegisterF = true;
		return Register19;
	}
	else if (str == "$20" || str == "$s4") {
		isRegisterF = true;
		return Register20;
	}
	else if (str == "$21" || str == "$s5") {
		isRegisterF = true;
		return Register21;
	}
	else if (str == "$22" || str == "$s6") {
		isRegisterF = true;
		return Register22;
	}
	else if (str == "$23" || str == "$s7") {
		isRegisterF = true;
		return Register23;
	}
	else if (str == "$24" || str == "$t8") {
		isRegisterF = true;
		return Register24;
	}
	else if (str == "$25" || str == "$t9") {
		isRegisterF = true;
		return Register25;
	}
	else if (str == "$26" || str == "$k0") {
		isRegisterF = true;
		return Register26;
	}
	else if (str == "$27" || str == "$k1") {
		isRegisterF = true;
		return Register27;
	}
	else if (str == "$28" || str == "$gp") {
		isRegisterF = true;
		return Register28;
	}
	else if (str == "$29" || str == "$sp") {
		isRegisterF = true;
		return Register29;
	}
	else if (str == "$30" || str == "$fp") {
		isRegisterF = true;
		return Register30;
	}
	else if (str == "$31" || str == "$ra") {
		isRegisterF = true;
		return Register31;
	}
	else if (str == "$pc") {
		isRegisterF = true;
		return ProgramCounter;
	}
	else if (str == "$hi" ) {
		isRegisterF = true;
		return RegisterHi;
	}
	else if (str == "$lo") {
		isRegisterF = true;
		return RegisterLo;
	}
	else {
		isRegisterF = false;

	}
}

int32_t VirtualMachine::printRegisters(string str)
{
	RegisterKinds registerNum = getRegister(str);
	return Registers[registerNum];
}

uint8_t VirtualMachine::getMemoryAdd(uint32_t num)
{
	return MemoryMap[num];
}


VirtualMachine::~VirtualMachine()
{
}
