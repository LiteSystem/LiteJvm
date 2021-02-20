#include <vmengine.h>
#include "vmstack.h"
#include "frame.h"
#include "arrayobject.h"
#include "classviewer.h"
#include "stringobject.h"
#include "classruntime.h"
#include "methodarea.h"

#include <iostream>
#include <cassert>
#include <queue>

#include <cmath>
#include <cfloat>
#include <cstdlib>
#include <cstdlib>

#include "stdio.h"
VMEngine::VMEngine() : _isWide(false) {
    initInstructions();
}

VMEngine::~VMEngine() {

}

void VMEngine::startVMEngine(ClassRuntime *classRuntime) {
    VMStack &stackFrame = VMStack::getInstance();

    vector<Value> arguments;
    Value commandLineArgs;
    commandLineArgs.type = ValueType::REFERENCE;
    commandLineArgs.data.object = new ArrayObject(ValueType::REFERENCE);
    arguments.push_back(commandLineArgs);

    stackFrame.addFrame(new Frame(classRuntime, "main", "([Ljava/lang/String;)V", arguments));

    if (doesMethodExist(classRuntime, "<clinit>", "()V")) {
        stackFrame.addFrame(new Frame(classRuntime, "<clinit>", "()V", arguments));
    }

    while (stackFrame.size() > 0) {
        Frame *topFrame = stackFrame.getTopFrame();
        u1 *code = topFrame->getCode(topFrame->pc);
        (*this.*_instructionFunctions[code[0]])();
    }
}

bool VMEngine::doesMethodExist(ClassRuntime *classRuntime, string name, string descriptor) {
    ClassFile *classFile = classRuntime->getClassFile();

    bool found = false;
    method_info method;
    for (int i = 0; i < classFile->methods_count; i++) {
        method = classFile->methods[i];
        string methodName = getFormattedConstant(classFile->constant_pool, method.name_index);
        string methodDesc = getFormattedConstant(classFile->constant_pool, method.descriptor_index);

        if (methodName == name && methodDesc == descriptor) {
            found = true;
            break;
        }
    }

    return found;
}

void VMEngine::populateMultiarray(ArrayObject *array, ValueType valueType, stack<int> count) {
    int currCount = count.top();
    count.pop();
    
    ValueType arrayType = (count.size() > 1) ? ValueType::REFERENCE : valueType;
    
    if (count.size() == 0) {
        for (int i = 0; i < currCount; i++) {
            Value subarrayValue;
            subarrayValue.type = valueType;
            subarrayValue.printType = valueType;
            subarrayValue.data.longValue = 0;
            array->pushValue(subarrayValue);
        }
    } else {
        for (int i = 0; i < currCount; i++) {
            ArrayObject *subarray = new ArrayObject(arrayType);
            populateMultiarray(subarray, valueType, count);
            
            Value subarrayValue;
            subarrayValue.type = ValueType::REFERENCE;
            subarrayValue.data.object = subarray;
            array->pushValue(subarrayValue);
        }
    }
}

void VMEngine::i_nop() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    topFrame->pc += 1;
}

void VMEngine::i_aconst_null() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.type = ValueType::REFERENCE;
    value.data.object = NULL;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iconst_m1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.printType = ValueType::INT;
    value.type = ValueType::INT;
    value.data.intValue = -1;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iconst_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.printType = ValueType::INT;
    value.type = ValueType::INT;
    value.data.intValue = 0;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iconst_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.printType = ValueType::INT;
    value.type = ValueType::INT;
    value.data.intValue = 1;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iconst_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.printType = ValueType::INT;
    value.type = ValueType::INT;
    value.data.intValue = 2;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iconst_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.printType = ValueType::INT;
    value.type = ValueType::INT;
    value.data.intValue = 3;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iconst_4() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.printType = ValueType::INT;
    value.type = ValueType::INT;
    value.data.intValue = 4;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iconst_5() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.printType = ValueType::INT;
    value.type = ValueType::INT;
    value.data.intValue = 5;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_lconst_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value padding;
    padding.type = ValueType::PADDING;

    Value value;
    value.type = ValueType::LONG;
    value.data.longValue = 0;

    topFrame->pushIntoOperandStack(padding);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_lconst_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value padding;
    padding.type = ValueType::PADDING;

    Value value;
    value.type = ValueType::LONG;
    value.data.longValue = 1;

    topFrame->pushIntoOperandStack(padding);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_fconst_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.type = ValueType::FLOAT;
    value.data.floatValue = 0;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_fconst_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.type = ValueType::FLOAT;
    value.data.floatValue = 1;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_fconst_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;
    value.type = ValueType::FLOAT;
    value.data.floatValue = 2;

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_dconst_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value padding;
    padding.type = ValueType::PADDING;

    Value value;
    value.type = ValueType::DOUBLE;
    value.data.doubleValue = 0;

    topFrame->pushIntoOperandStack(padding);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_dconst_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value padding;
    padding.type = ValueType::PADDING;

    Value value;
    value.type = ValueType::DOUBLE;
    value.data.doubleValue = 1;

    topFrame->pushIntoOperandStack(padding);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_bipush() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte = code[1];

    Value value;
    value.printType = ValueType::BYTE;
    value.type = ValueType::INT;
    value.data.intValue = (int32_t) (int8_t) byte; // convertendo para inteiro e estendendo o sinal

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 2;
}

void VMEngine::i_sipush() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t shortValue = (byte1 << 8) | byte2;
    Value value;
    value.printType = ValueType::SHORT;
    value.type = ValueType::INT;
    value.data.intValue = (int32_t) (int16_t) shortValue; // convertendo para inteiro e estendendo o sinal

    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 3;
}

void VMEngine::i_ldc() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    u1 *code = topFrame->getCode(topFrame->pc);
    u1 index = code[1];
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    cp_info entry = constantPool[index-1];

    Value value;
    
    if (entry.tag == CONSTANT_String) {
        cp_info utf8Entry = constantPool[entry.info.string_info.string_index-1];
        assert(utf8Entry.tag == CONSTANT_Utf8);
        
        u1* bytes = utf8Entry.info.utf8_info.bytes;
        char utf8String[utf8Entry.info.utf8_info.length+1];
        int i;
        for (i = 0; i < utf8Entry.info.utf8_info.length; i++) {
            utf8String[i] = bytes[i];
        }
        utf8String[i] = '\0';
        
        value.type = ValueType::REFERENCE;
        value.data.object = new StringObject(utf8String);
    } else if (entry.tag == CONSTANT_Integer) {
        value.printType = ValueType::INT;
        value.type = ValueType::INT;
        value.data.intValue = (int32_t) entry.info.integer_info.bytes;
    } else if (entry.tag == CONSTANT_Float) {
        u4 floatBytes = entry.info.float_info.bytes;
        int s = ((floatBytes >> 31) == 0) ? 1 : -1;
        int e = ((floatBytes >> 23) & 0xff);
        int m = (e == 0) ? (floatBytes & 0x7fffff) << 1 : (floatBytes & 0x7fffff) | 0x800000;
        
        float number = s*m*pow(2, e-150);
        value.type = ValueType::FLOAT;
        value.data.floatValue = number;
    } else {
        cerr << "ldc tentando acessar um elemento da CP invalido: " << entry.tag << endl;
        exit(1);
    }
    
    topFrame->pushIntoOperandStack(value);
    topFrame->pc += 2;
}

void VMEngine::i_ldc_w() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    u1 *code = topFrame->getCode(topFrame->pc);
    u1 byte1 = code[1];
    u1 byte2 = code[2];
    u2 index = (byte1 << 8) | byte2;
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    cp_info entry = constantPool[index-1];
    
    Value value;
    
    if (entry.tag == CONSTANT_String) {
        cp_info utf8Entry = constantPool[entry.info.string_info.string_index-1];
        assert(utf8Entry.tag == CONSTANT_Utf8);
        
        u1* bytes = utf8Entry.info.utf8_info.bytes;
        char utf8String[utf8Entry.info.utf8_info.length+1];
        int i;
        for (i = 0; i < utf8Entry.info.utf8_info.length; i++) {
            utf8String[i] = bytes[i];
        }
        utf8String[i] = '\0';
        
        value.type = ValueType::REFERENCE;
        value.data.object = new StringObject(utf8String);
    } else if (entry.tag == CONSTANT_Integer) {
        value.printType = ValueType::INT;
        value.type = ValueType::INT;
        value.data.intValue = entry.info.integer_info.bytes;
    } else if (entry.tag == CONSTANT_Float) {
        u4 floatBytes = entry.info.float_info.bytes;
        int s = ((floatBytes >> 31) == 0) ? 1 : -1;
        int e = ((floatBytes >> 23) & 0xff);
        int m = (e == 0) ? (floatBytes & 0x7fffff) << 1 : (floatBytes & 0x7fffff) | 0x800000;
        
        float number = s*m*pow(2, e-150);
        value.type = ValueType::FLOAT;
        value.data.floatValue = number;
    } else {
        cerr << "ldc_w tentando acessar um elemento da CP invalido: " << entry.tag << endl;
        exit(1);
    }
    
    topFrame->pushIntoOperandStack(value);
    topFrame->pc += 3;
}

void VMEngine::i_ldc2_w() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    u1 *code = topFrame->getCode(topFrame->pc);
    u1 byte1 = code[1];
    u1 byte2 = code[2];
    u2 index = (byte1 << 8) | byte2;
    
    cp_info *classFile = *(topFrame->getConstantPool());
    cp_info entry = classFile[index-1];
    
    Value value;
    
    if (entry.tag == CONSTANT_Long) {
        u4 highBytes = entry.info.long_info.high_bytes;
        u4 lowBytes = entry.info.long_info.low_bytes;
        
        int64_t longNumber = ((int64_t) highBytes << 32) + lowBytes;
        value.type = ValueType::LONG;
        value.data.longValue = longNumber;
        
        Value padding;
        padding.type = ValueType::PADDING;
        
        topFrame->pushIntoOperandStack(padding);
    } else if (entry.tag == CONSTANT_Double) {
        u4 highBytes = entry.info.double_info.high_bytes;
        u4 lowBytes = entry.info.double_info.low_bytes;
        
        int64_t longNumber = ((int64_t) highBytes << 32) + lowBytes;
        
        int32_t s = ((longNumber >> 63) == 0) ? 1 : -1;
        int32_t e = (int32_t)((longNumber >> 52) & 0x7ffL);
        int64_t m = (e == 0) ? (longNumber & 0xfffffffffffffL) << 1 : (longNumber & 0xfffffffffffffL) | 0x10000000000000L;
        
        double doubleNumber = s*m*pow(2, e-1075);
        value.type = ValueType::DOUBLE;
        value.data.doubleValue = doubleNumber;
        
        Value padding;
        padding.type = ValueType::PADDING;
        
        topFrame->pushIntoOperandStack(padding);
    } else {
        cerr << "ldc2_w tentando acessar um elemento da CP invalido: " << entry.tag << endl;
        exit(1);
    }
    
    topFrame->pushIntoOperandStack(value);
    topFrame->pc += 3;
}

// Pode ser modificado pelo wide
void VMEngine::i_iload() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t)byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	}
	else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > index));
	Value value = topFrame->getLocalVariableValue(index);
	assert(value.type == ValueType::INT);

	topFrame->pushIntoOperandStack(value);
}

// Pode ser modificado pelo wide
void VMEngine::i_lload() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t)byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	}
	else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > (index + 1)));

	Value value = topFrame->getLocalVariableValue(index);
	assert(value.type == ValueType::LONG);

	Value padding;
	padding.type = ValueType::PADDING;

	topFrame->pushIntoOperandStack(padding);
	topFrame->pushIntoOperandStack(value);
}

// Pode ser modificado pelo wide
void VMEngine::i_fload() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t)byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	}
	else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > index));
	Value value = topFrame->getLocalVariableValue(index);
	assert(value.type == ValueType::FLOAT);
	topFrame->pushIntoOperandStack(value);

}

// Pode ser modificado pelo wide
void VMEngine::i_dload() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; // índice do vetor de variáveis locais
	int16_t index = (int16_t) byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	}
	else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > (index + 1)));

	Value value = topFrame->getLocalVariableValue(index);
	assert(value.type == ValueType::DOUBLE);

	Value padding;
	padding.type = ValueType::PADDING;

	topFrame->pushIntoOperandStack(padding);
	topFrame->pushIntoOperandStack(value);
}

// Pode ser modificado pelo wide
void VMEngine::i_aload() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; // índice do vetor de variáveis locais
	int16_t index = (int16_t) byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	}
	else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > index));
	Value value = topFrame->getLocalVariableValue(index);
	assert(value.type == ValueType::REFERENCE);
	topFrame->pushIntoOperandStack(value);
}

void VMEngine::i_iload_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(0);
    assert(value.type == ValueType::INT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iload_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(1);
    assert(value.type == ValueType::INT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iload_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(2);
    assert(value.type == ValueType::INT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iload_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(3);
    assert(value.type == ValueType::INT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_lload_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(1);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(0);
    assert(value.type == ValueType::LONG);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_lload_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(2);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(1);
    assert(value.type == ValueType::LONG);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_lload_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(3);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(2);
    assert(value.type == ValueType::LONG);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_lload_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(4);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(3);
    assert(value.type == ValueType::LONG);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_fload_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(0);
    assert(value.type == ValueType::FLOAT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_fload_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(1);
    assert(value.type == ValueType::FLOAT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_fload_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(2);
    assert(value.type == ValueType::FLOAT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_fload_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(3);
    assert(value.type == ValueType::FLOAT);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_dload_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(1);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(0);
    assert(value.type == ValueType::DOUBLE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_dload_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(2);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(1);
    assert(value.type == ValueType::DOUBLE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_dload_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(3);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(2);
    assert(value.type == ValueType::DOUBLE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_dload_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value;

    value = topFrame->getLocalVariableValue(4);
    assert(value.type == ValueType::PADDING);
    topFrame->pushIntoOperandStack(value);

    value = topFrame->getLocalVariableValue(3);
    assert(value.type == ValueType::DOUBLE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_aload_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(0);
    assert(value.type == ValueType::REFERENCE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_aload_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(1);
    assert(value.type == ValueType::REFERENCE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_aload_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(2);
    assert(value.type == ValueType::REFERENCE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_aload_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->getLocalVariableValue(3);
    assert(value.type == ValueType::REFERENCE);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_iaload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    topFrame->pushIntoOperandStack(array->getValue(index.data.intValue));
    topFrame->pc += 1;
}

void VMEngine::i_laload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    Value padding;
    padding.type = ValueType::PADDING;
    
    topFrame->pushIntoOperandStack(padding);
    topFrame->pushIntoOperandStack(array->getValue(index.data.intValue));
    topFrame->pc += 1;
}

void VMEngine::i_faload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    topFrame->pushIntoOperandStack(array->getValue(index.data.intValue));
    topFrame->pc += 1;
}

void VMEngine::i_daload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    Value padding;
    padding.type = ValueType::PADDING;
    
    topFrame->pushIntoOperandStack(padding);
    topFrame->pushIntoOperandStack(array->getValue(index.data.intValue));
    topFrame->pc += 1;
}

void VMEngine::i_aaload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    topFrame->pushIntoOperandStack(array->getValue(index.data.intValue));
    topFrame->pc += 1;
}

void VMEngine::i_baload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    Value value = array->getValue(index.data.intValue);
    assert(value.type == ValueType::BOOLEAN || value.type == ValueType::BYTE);
    
    if (value.type == ValueType::BOOLEAN) {
        value.data.intValue = (uint32_t) value.data.booleanValue;
        value.printType = ValueType::BOOLEAN;
    } else {
        value.data.intValue = (int32_t) value.data.byteValue;
        value.printType = ValueType::BYTE;
    }
    value.type = ValueType::INT;

    topFrame->pushIntoOperandStack(value);
    topFrame->pc += 1;
}

void VMEngine::i_caload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    Value charValue = array->getValue(index.data.intValue);
    charValue.data.intValue = (uint32_t) charValue.data.charValue;
    charValue.printType = ValueType::CHAR;
    charValue.type = ValueType::INT;
    
    topFrame->pushIntoOperandStack(charValue);
    topFrame->pc += 1;
}

void VMEngine::i_saload() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }
    
    Value shortValue = array->getValue(index.data.intValue);
    shortValue.data.intValue = (int32_t) shortValue.data.shortValue;
    shortValue.printType = ValueType::SHORT;
    shortValue.type = ValueType::INT;
    
    topFrame->pushIntoOperandStack(shortValue);
    topFrame->pc += 1;
}

//
void VMEngine::i_istore() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::INT);

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t) byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	} else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > index));
	topFrame->changeLocalVariable(value, index);
}

// Pode ser modificado pelo wide
void VMEngine::i_lstore() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::LONG);
	topFrame->popTopOfOperandStack(); //padding

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t)byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	} else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > (index + 1)));
	topFrame->changeLocalVariable(value, index);
	Value padding;
	padding.type = ValueType::PADDING;
	topFrame->changeLocalVariable(padding, index + 1);
}

// Pode ser modificado pelo wide
void VMEngine::i_fstore() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::FLOAT);

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t)byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	} else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > index));
	topFrame->changeLocalVariable(value, index);
}

// Pode ser modificado pelo wide
void VMEngine::i_dstore() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::DOUBLE);
	topFrame->popTopOfOperandStack(); //padding

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t)byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	} else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > (index + 1)));
	topFrame->changeLocalVariable(value, index);
	Value padding;
	padding.type = ValueType::PADDING;
	topFrame->changeLocalVariable(padding, index + 1);
}

// Pode ser modificado pelo wide
void VMEngine::i_astore() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::REFERENCE);

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; //índice do vetor de variáveis locais
	int16_t index = (int16_t)byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
		topFrame->pc += 3;
		_isWide = false;
	} else {
		topFrame->pc += 2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > index));
	topFrame->changeLocalVariable(value, index);
}

void VMEngine::i_istore_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    topFrame->changeLocalVariable(value, 0);

    topFrame->pc += 1;
}

void VMEngine::i_istore_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    topFrame->changeLocalVariable(value, 1);

    topFrame->pc += 1;
}

void VMEngine::i_istore_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    topFrame->changeLocalVariable(value, 2);

    topFrame->pc += 1;
}

void VMEngine::i_istore_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    topFrame->changeLocalVariable(value, 3);

    topFrame->pc += 1;
}

void VMEngine::i_lstore_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::LONG);
    topFrame->changeLocalVariable(value, 0);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 1);

    topFrame->pc += 1;
}

void VMEngine::i_lstore_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::LONG);
    topFrame->changeLocalVariable(value, 1);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 2);

    topFrame->pc += 1;
}

void VMEngine::i_lstore_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::LONG);
    topFrame->changeLocalVariable(value, 2);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 3);

    topFrame->pc += 1;
}

void VMEngine::i_lstore_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::LONG);
    topFrame->changeLocalVariable(value, 3);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 4);

    topFrame->pc += 1;
}

void VMEngine::i_fstore_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::FLOAT);
    topFrame->changeLocalVariable(value, 0);

    topFrame->pc += 1;
}

void VMEngine::i_fstore_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::FLOAT);
    topFrame->changeLocalVariable(value, 1);

    topFrame->pc += 1;
}

void VMEngine::i_fstore_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::FLOAT);
    topFrame->changeLocalVariable(value, 2);

    topFrame->pc += 1;
}

void VMEngine::i_fstore_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::FLOAT);
    topFrame->changeLocalVariable(value, 3);

    topFrame->pc += 1;
}

void VMEngine::i_dstore_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::DOUBLE);
    topFrame->changeLocalVariable(value, 0);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 1);

    topFrame->pc += 1;
}

void VMEngine::i_dstore_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::DOUBLE);
    topFrame->changeLocalVariable(value, 1);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 2);

    topFrame->pc += 1;
}

void VMEngine::i_dstore_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::DOUBLE);
    topFrame->changeLocalVariable(value, 2);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 3);

    topFrame->pc += 1;
}

void VMEngine::i_dstore_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::DOUBLE);
    topFrame->changeLocalVariable(value, 3);

    value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::PADDING);
    topFrame->changeLocalVariable(value, 4);

    topFrame->pc += 1;
}

void VMEngine::i_astore_0() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::REFERENCE);
    topFrame->changeLocalVariable(value, 0);

    topFrame->pc += 1;
}

void VMEngine::i_astore_1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::REFERENCE);
    topFrame->changeLocalVariable(value, 1);

    topFrame->pc += 1;
}

void VMEngine::i_astore_2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::REFERENCE);
    topFrame->changeLocalVariable(value, 2);

    topFrame->pc += 1;
}

void VMEngine::i_astore_3() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::REFERENCE);
    topFrame->changeLocalVariable(value, 3);

    topFrame->pc += 1;
}

void VMEngine::i_iastore() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue >= array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    value.printType = ValueType::INT;
    
    assert(value.type == array->arrayContentType());
    array->changeValueAt(index.data.intValue, value);
    
    topFrame->pc += 1;
}

void VMEngine::i_lastore() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::LONG);
    Value padding = topFrame->popTopOfOperandStack();
    assert(padding.type == ValueType::PADDING);
    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue >= array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    assert(value.type == array->arrayContentType());
    array->changeValueAt(index.data.intValue, value);

    topFrame->pc += 1;
}

void VMEngine::i_fastore() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::FLOAT);
    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue >= array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    assert(value.type == array->arrayContentType());
    array->changeValueAt(index.data.intValue, value);
	
    topFrame->pc += 1;
}

void VMEngine::i_dastore() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::DOUBLE);
    Value padding = topFrame->popTopOfOperandStack();
    assert(padding.type == ValueType::PADDING);
    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue >= array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    assert(value.type == array->arrayContentType());
    array->changeValueAt(index.data.intValue, value);
	
    topFrame->pc += 1;
}

void VMEngine::i_aastore() {
	VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

	Value value = topFrame->popTopOfOperandStack(); // Valor armazenado no index do array
	assert(value.type == ValueType::REFERENCE);
    Value index = topFrame->popTopOfOperandStack(); // Index do arary
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack(); // Referência ao array
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue >= array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

	array->changeValueAt(index.data.intValue, value);
    
    topFrame->pc += 1;
}

void VMEngine::i_bastore() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;
    assert(array->arrayContentType() == ValueType::BOOLEAN || array->arrayContentType() == ValueType::BYTE);

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    if (array->arrayContentType() == ValueType::BOOLEAN) {
        value.data.booleanValue = (value.data.intValue != 0) ? true : false;
        value.type = ValueType::BOOLEAN;
        value.printType = ValueType::BOOLEAN;
    } else {
        value.data.byteValue = (uint8_t) value.data.intValue;
        value.type = ValueType::BYTE;
        value.printType = ValueType::BYTE;
    }
    
    array->changeValueAt(index.data.intValue, value);
	
    topFrame->pc += 1;
}

void VMEngine::i_castore() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    value.data.charValue = (uint8_t) value.data.intValue;
    value.printType = ValueType::CHAR;
    value.type = ValueType::CHAR;
    array->changeValueAt(index.data.intValue, value);
	
    topFrame->pc += 1;
}

void VMEngine::i_sastore() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
	ArrayObject *array;

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    Value index = topFrame->popTopOfOperandStack();
    assert(index.type == ValueType::INT);
    Value arrayref = topFrame->popTopOfOperandStack();
    assert(arrayref.type == ValueType::REFERENCE);
    assert((arrayref.data.object)->objectType() == ObjectType::ARRAY);

    array = (ArrayObject *) arrayref.data.object;

    if (array == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    if (index.data.intValue > array->getSize() || index.data.intValue < 0) {
        cerr << "ArrayIndexOutOfBoundsException" << endl;
        exit(2);
    }

    value.data.shortValue = (int16_t) value.data.intValue;
    value.printType = ValueType::SHORT;
    value.type = ValueType::SHORT;
    array->changeValueAt(index.data.intValue, value);
	
    topFrame->pc += 1;
}

void VMEngine::i_pop() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    Value value = topFrame->popTopOfOperandStack();
    assert(value.type != ValueType::LONG);
    assert(value.type != ValueType::DOUBLE);

    topFrame->pc += 1;
}

void VMEngine::i_pop2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    topFrame->popTopOfOperandStack();
    topFrame->popTopOfOperandStack();

    topFrame->pc += 1;
}

void VMEngine::i_dup() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value = topFrame->popTopOfOperandStack();
    assert(value.type != ValueType::LONG);
    assert(value.type != ValueType::DOUBLE);

    topFrame->pushIntoOperandStack(value);
    topFrame->pushIntoOperandStack(value);

    topFrame->pc += 1;
}

void VMEngine::i_dup_x1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    assert(value_1.type != ValueType::LONG);
    assert(value_1.type != ValueType::DOUBLE);
    Value value_2 = topFrame->popTopOfOperandStack();
    assert(value_2.type != ValueType::LONG);
    assert(value_2.type != ValueType::DOUBLE);

    topFrame->pushIntoOperandStack(value_1);
    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_dup_x2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_3 = topFrame->popTopOfOperandStack();

    assert(value_1.type != ValueType::LONG);
    assert(value_1.type != ValueType::DOUBLE);
    assert(value_3.type != ValueType::LONG);
    assert(value_3.type != ValueType::DOUBLE);

    topFrame->pushIntoOperandStack(value_1);
    topFrame->pushIntoOperandStack(value_3);
    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_dup2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    Value value_2 = topFrame->popTopOfOperandStack();
    assert(value_2.type != ValueType::LONG);
    assert(value_2.type != ValueType::DOUBLE);

    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);
    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_dup2_x1() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_3 = topFrame->popTopOfOperandStack();

    assert(value_2.type != ValueType::LONG);
    assert(value_2.type != ValueType::DOUBLE);
    assert(value_3.type != ValueType::LONG);
    assert(value_3.type != ValueType::DOUBLE);

    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);
    topFrame->pushIntoOperandStack(value_3);
    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_dup2_x2() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_3 = topFrame->popTopOfOperandStack();
    Value value_4 = topFrame->popTopOfOperandStack();

    assert(value_2.type != ValueType::LONG);
    assert(value_2.type != ValueType::DOUBLE);
    assert(value_4.type != ValueType::LONG);
    assert(value_4.type != ValueType::DOUBLE);

    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);
    topFrame->pushIntoOperandStack(value_4);
    topFrame->pushIntoOperandStack(value_3);
    topFrame->pushIntoOperandStack(value_2);
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_swap() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    Value value_2 = topFrame->popTopOfOperandStack();

    assert(value_1.type != ValueType::LONG);
    assert(value_1.type != ValueType::DOUBLE);
    assert(value_2.type != ValueType::LONG);
    assert(value_2.type != ValueType::DOUBLE);

    topFrame->pushIntoOperandStack(value_1);
    topFrame->pushIntoOperandStack(value_2);

    topFrame->pc += 1;
}

void VMEngine::i_iadd() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);

	value_1.data.intValue = value_1.data.intValue + (value_2.data.intValue);
    value_1.printType = ValueType::INT;
    
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_ladd() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); //padding
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);

	value_1.data.longValue = value_1.data.longValue + (value_2.data.longValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_fadd() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::FLOAT);
	assert(value_1.type == ValueType::FLOAT);

	value_1.data.floatValue = value_1.data.floatValue + (value_2.data.floatValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_dadd() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::DOUBLE);
	assert(value_1.type == ValueType::DOUBLE);

	value_1.data.doubleValue = value_1.data.doubleValue + (value_2.data.doubleValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_isub() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::INT;
	value_1.data.intValue = value_1.data.intValue - (value_2.data.intValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_lsub() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);

	value_1.data.longValue = value_1.data.longValue - (value_2.data.longValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_fsub() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::FLOAT);
	assert(value_1.type == ValueType::FLOAT);

	value_1.data.floatValue = value_1.data.floatValue - (value_2.data.floatValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_dsub() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::DOUBLE);
	assert(value_1.type == ValueType::DOUBLE);

	value_1.data.doubleValue = value_1.data.doubleValue - (value_2.data.doubleValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_imul() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::INT;
	value_1.data.intValue = value_1.data.intValue * (value_2.data.intValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_lmul() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);

	value_1.data.longValue = value_1.data.longValue * (value_2.data.longValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_fmul() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::FLOAT);
	assert(value_1.type == ValueType::FLOAT);

	value_1.data.floatValue = value_1.data.floatValue * (value_2.data.floatValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_dmul() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::DOUBLE);
	assert(value_1.type == ValueType::DOUBLE);

	value_1.data.doubleValue = value_1.data.doubleValue * (value_2.data.doubleValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_idiv() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);
	if (value_2.data.intValue == 0) {
		cerr << "ArithmeticException" << endl;
		exit(2);
	}

    value_1.printType = ValueType::INT;
	value_1.data.intValue = value_1.data.intValue / (value_2.data.intValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_ldiv() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);
	if (value_2.data.longValue == 0) {
		cerr << "ArithmeticException" << endl;
		exit(2);
	}

	value_1.data.longValue = value_1.data.longValue / (value_2.data.longValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_fdiv() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::FLOAT);
	assert(value_1.type == ValueType::FLOAT);
	if (value_2.data.floatValue == 0) {
		cerr << "ArithmeticException" << endl;
		exit(2);
	}
	value_1.data.floatValue = value_1.data.floatValue / (value_2.data.floatValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_ddiv() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::DOUBLE);
	assert(value_1.type == ValueType::DOUBLE);
	if (value_2.data.doubleValue == 0) {
		cerr << "ArithmeticException" << endl;
		exit(2);
	}
	value_1.data.doubleValue = value_1.data.doubleValue / (value_2.data.doubleValue);
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_irem() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_1 = topFrame->popTopOfOperandStack();

    assert(value_2.type == ValueType::INT);
    assert(value_1.type == ValueType::INT);
	if (value_2.data.intValue == 0) {
		cerr << "ArithmeticException" << endl;
        exit(2);
	}
	
    value_1.printType = ValueType::INT;
	value_1.data.intValue = value_1.data.intValue - (value_1.data.intValue / value_2.data.intValue)*value_2.data.intValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_lrem() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    topFrame->popTopOfOperandStack();
    Value value_1 = topFrame->popTopOfOperandStack();
    // Sobra um padding na pilha que ficará abaixo do resultado

    assert(value_2.type == ValueType::LONG);
    assert(value_1.type == ValueType::LONG);
	if (value_2.data.longValue == 0) {
		cerr << "ArithmeticException" << endl;
        exit(2);
	}
	// value_1 negativo implica em resultado negativo
	value_1.data.longValue = value_1.data.longValue - (value_1.data.longValue / value_2.data.longValue)*value_2.data.longValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_frem() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_1 = topFrame->popTopOfOperandStack();

    assert(value_2.type == ValueType::FLOAT);
    assert(value_1.type == ValueType::FLOAT);
	if (value_2.data.floatValue == 0) {
		cerr << "ArithmeticException" << endl;
        exit(2);
	}
	// value_1 negativo implica em resultado negativo
	value_1.data.floatValue = value_1.data.floatValue - ((uint32_t)(value_1.data.floatValue / value_2.data.floatValue))*value_2.data.floatValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_drem() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    topFrame->popTopOfOperandStack(); // PADDING
    Value value_1 = topFrame->popTopOfOperandStack();
    // Sobra um padding na pilha que ficará abaixo do resultado

    assert(value_2.type == ValueType::DOUBLE);
    assert(value_1.type == ValueType::DOUBLE);
	if (value_2.data.doubleValue == 0) {
		cerr << "ArithmeticException" << endl;
        exit(2);
	}
	// value_1 negativo implica em resultado negativo
	value_1.data.doubleValue = value_1.data.doubleValue - ((uint64_t)(value_1.data.doubleValue / value_2.data.doubleValue))*value_2.data.doubleValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_ineg() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::INT;
	value_1.data.intValue = -value_1.data.intValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_lneg() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

	// Não precisa tirar o padding
    Value value_1 = topFrame->popTopOfOperandStack();
    assert(value_1.type == ValueType::LONG);

	value_1.data.longValue = -value_1.data.longValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_fneg() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_1 = topFrame->popTopOfOperandStack();
    assert(value_1.type == ValueType::FLOAT);

	value_1.data.floatValue = -value_1.data.floatValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_dneg() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

	// Não precisa tirar o padding
    Value value_1 = topFrame->popTopOfOperandStack();
    assert(value_1.type == ValueType::DOUBLE);

	value_1.data.doubleValue = -value_1.data.doubleValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_ishl() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_1 = topFrame->popTopOfOperandStack();
    assert(value_2.type == ValueType::INT);
    assert(value_1.type == ValueType::INT);

	// value_2 armazena seus 5 primeiros bits
	value_2.data.intValue = 0x1f & value_2.data.intValue;
	value_1.data.intValue = value_1.data.intValue << value_2.data.intValue;
    value_1.printType = ValueType::INT;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_lshl() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_1 = topFrame->popTopOfOperandStack();
    // Sobra um padding na pilha que ficará abaixo do resultado

    assert(value_2.type == ValueType::INT);
    assert(value_1.type == ValueType::LONG);

    value_2.data.longValue = 0x3f & value_2.data.longValue;
    value_1.data.longValue = (value_1.data.longValue) << value_2.data.intValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_ishr() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_1 = topFrame->popTopOfOperandStack();
    assert(value_2.type == ValueType::INT);
    assert(value_1.type == ValueType::INT);

	// value_2 armazena seus 5 primeiros bits
	value_2.data.intValue = 0x1f & value_2.data.intValue;
	value_1.data.intValue = value_1.data.intValue >> value_2.data.intValue;
    value_1.printType = ValueType::INT;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_lshr() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();

    Value value_2 = topFrame->popTopOfOperandStack();
    Value value_1 = topFrame->popTopOfOperandStack();
    // Sobra um padding na pilha que ficará abaixo do resultado

    assert(value_2.type == ValueType::INT);
    assert(value_1.type == ValueType::LONG);

	// value_2 armazena seus 6 primeiros bits
	value_2.data.longValue = 0x3f & value_2.data.longValue;
	value_1.data.longValue = value_1.data.longValue >> value_2.data.longValue;
    topFrame->pushIntoOperandStack(value_1);

    topFrame->pc += 1;
}

void VMEngine::i_iushr() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);

	value_2.data.intValue = 0x1f & value_2.data.intValue;
	value_1.data.intValue = value_1.data.intValue >> value_2.data.intValue;
	if (value_1.data.intValue < 0) {
		value_1.data.intValue = value_1.data.intValue + (2<<~(value_2.data.intValue));
	}
    value_1.printType = ValueType::INT;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_lushr() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::LONG);

	value_2.data.intValue = 0x3f & value_2.data.intValue;
	value_1.data.longValue = value_1.data.longValue >> value_2.data.intValue;
	if (value_1.data.longValue < 0) {
		value_1.data.longValue = value_1.data.longValue + ((int64_t)2 << ~(value_2.data.intValue));
	}
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_iand() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::INT;
	value_1.data.intValue = value_1.data.intValue & value_2.data.intValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_land() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); // PADDING
	Value value_1 = topFrame->popTopOfOperandStack();
	// Sobra um padding na pilha que ficará abaixo do resultado

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);

	// value_2 armazena seus 6 primeiros bits
	value_1.data.longValue = value_1.data.longValue & value_2.data.longValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_ior() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::INT;
	value_1.data.intValue = value_1.data.intValue | value_2.data.intValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_lor() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); // PADDING
	Value value_1 = topFrame->popTopOfOperandStack();
	// Sobra um padding na pilha que ficará abaixo do resultado

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);

	value_1.data.longValue = value_1.data.longValue | value_2.data.longValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_ixor() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_2.type == ValueType::INT);
	assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::INT;
	value_1.data.intValue = value_1.data.intValue ^ value_2.data.intValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_lxor() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); // PADDING
	Value value_1 = topFrame->popTopOfOperandStack();
	// Sobra um padding na pilha que ficará abaixo do resultado

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);

	value_1.data.longValue = value_1.data.longValue ^ value_2.data.longValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_iinc() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    u1 *code = topFrame->getCode(topFrame->pc);
    
    u2 index = 0;
    if (_isWide) {
        index = (code[1] << 8) | code[2];
    } else {
        index += code[1];
    }
    
    Value localVariable = topFrame->getLocalVariableValue(index);
    assert(localVariable.type == ValueType::INT);
    
    int32_t inc;
    if (_isWide) {
        uint16_t constant = (code[3] << 8) | code[4];
        inc = (int32_t) (int16_t) constant;
    } else {
        inc = (int32_t) (int8_t) code[2];
    }
    
    localVariable.data.intValue += inc;
    topFrame->changeLocalVariable(localVariable, index);
    
    topFrame->pc += _isWide ? 5 : 3;
    _isWide = false;
}

void VMEngine::i_i2l() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::INT);

	Value padding;
	padding.type = ValueType::PADDING;

	topFrame->pushIntoOperandStack(padding);

    value_1.data.longValue = (int64_t) value_1.data.intValue;
	value_1.type = ValueType::LONG;

	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_i2f() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::INT);

	value_1.type = ValueType::FLOAT;
	value_1.data.floatValue = (float) value_1.data.intValue;

	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_i2d() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::INT);

	Value padding;
	padding.type = ValueType::PADDING;
	topFrame->pushIntoOperandStack(padding);

	value_1.type = ValueType::DOUBLE;
	value_1.data.doubleValue = (double) value_1.data.intValue;

	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_l2i() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); //padding

	assert(value_1.type == ValueType::LONG);

    value_1.data.intValue = (int32_t) value_1.data.intValue;
    value_1.printType = ValueType::INT;
	value_1.type = ValueType::INT;

	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_l2f() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); //padding

	assert(value_1.type == ValueType::LONG);

	value_1.type = ValueType::FLOAT;
	value_1.data.floatValue = (float) value_1.data.longValue;

	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_l2d() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();
	//manter padding na pilha de operandos

	assert(value_1.type == ValueType::LONG);

	value_1.type = ValueType::DOUBLE;
	value_1.data.doubleValue = (double) value_1.data.longValue;

	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_f2i() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::FLOAT);

    value_1.printType = ValueType::INT;
	value_1.type = ValueType::INT;
	value_1.data.intValue = (int32_t) value_1.data.floatValue;

	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_f2l() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::FLOAT);

	Value padding;
	padding.type = ValueType::PADDING;
	topFrame->pushIntoOperandStack(padding);

	value_1.type = ValueType::LONG;
	value_1.data.longValue = (uint64_t) value_1.data.floatValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_f2d() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::FLOAT);

	Value padding;
	padding.type = ValueType::PADDING;
	topFrame->pushIntoOperandStack(padding);

	value_1.type = ValueType::DOUBLE;
	value_1.data.doubleValue = (double) value_1.data.floatValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_d2i() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); //padding

	assert(value_1.type == ValueType::DOUBLE);

    value_1.printType = ValueType::INT;
	value_1.type = ValueType::INT;
	value_1.data.intValue = (int32_t) value_1.data.doubleValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_d2l() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();
	//manter padding na pilha de operandos

	assert(value_1.type == ValueType::DOUBLE);

	value_1.type = ValueType::LONG;
	value_1.data.longValue = (int64_t) value_1.data.doubleValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_d2f() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack(); //padding

	assert(value_1.type == ValueType::DOUBLE);

	value_1.type = ValueType::FLOAT;
	value_1.data.floatValue = (float) value_1.data.doubleValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_i2b() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::INT);
    
    value_1.printType = ValueType::BYTE;
    
    value_1.data.intValue = (int32_t) (int8_t) value_1.data.intValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_i2c() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::CHAR;
    
    value_1.data.charValue = (uint32_t) (uint8_t) value_1.data.intValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_i2s() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_1 = topFrame->popTopOfOperandStack();

	assert(value_1.type == ValueType::INT);

    value_1.printType = ValueType::SHORT;
    
    value_1.data.intValue = (int32_t) (int16_t) value_1.data.intValue;
	topFrame->pushIntoOperandStack(value_1);

	topFrame->pc += 1;
}

void VMEngine::i_lcmp() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value resultado;
    resultado.printType = ValueType::INT;
	resultado.type = ValueType::INT;

	assert(value_2.type == ValueType::LONG);
	assert(value_1.type == ValueType::LONG);

	if (value_1.data.longValue > value_2.data.longValue) {
		resultado.data.intValue = 1;
	} else if (value_1.data.longValue == value_2.data.longValue) {
		resultado.data.intValue = 0;
	} else {
		resultado.data.intValue = -1;
	}

	topFrame->pushIntoOperandStack(resultado);

	topFrame->pc += 1;
}

void VMEngine::i_fcmpl() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();
	Value resultado;
    resultado.printType = ValueType::INT;
	resultado.type = ValueType::INT;

	assert(value_2.type == ValueType::FLOAT);
	assert(value_1.type == ValueType::FLOAT);

	if (isnan(value_1.data.floatValue) || isnan(value_2.data.floatValue)) {
		resultado.data.intValue = -1;
	} else if (value_1.data.floatValue > value_2.data.floatValue) {
		resultado.data.intValue = 1;
	} else if (value_1.data.floatValue == value_2.data.floatValue) {
		resultado.data.intValue = 0;
	} else {
		resultado.data.intValue = -1;
	}

	topFrame->pushIntoOperandStack(resultado);

	topFrame->pc += 1;
}

void VMEngine::i_fcmpg() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();
	Value resultado;
    resultado.printType = ValueType::INT;
	resultado.type = ValueType::INT;

	assert(value_2.type == ValueType::FLOAT);
	assert(value_1.type == ValueType::FLOAT);

	if (isnan(value_1.data.floatValue) || isnan(value_2.data.floatValue)) {
		resultado.data.intValue = 1;
	} else if (value_1.data.floatValue > value_2.data.floatValue) {
		resultado.data.intValue = 1;
	} else if (value_1.data.floatValue == value_2.data.floatValue) {
		resultado.data.intValue = 0;
	} else {
		resultado.data.intValue = -1;
	}

	topFrame->pushIntoOperandStack(resultado);

	topFrame->pc += 1;
}

void VMEngine::i_dcmpl() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value resultado;
    resultado.printType = ValueType::INT;
	resultado.type = ValueType::INT;

	assert(value_2.type == ValueType::DOUBLE);
	assert(value_1.type == ValueType::DOUBLE);

	if ( std::isnan(value_1.data.doubleValue) || std::isnan(value_2.data.doubleValue)) {
		resultado.data.intValue = -1;
	} else if (value_1.data.doubleValue > value_2.data.doubleValue) {
		resultado.data.intValue = 1;
	} else if (value_1.data.doubleValue == value_2.data.doubleValue) {
		resultado.data.intValue = 0;
	} else {
		resultado.data.intValue = -1;
	}

	topFrame->pushIntoOperandStack(resultado);

	topFrame->pc += 1;
}

void VMEngine::i_dcmpg() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value_2 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value value_1 = topFrame->popTopOfOperandStack();
	topFrame->popTopOfOperandStack();
	Value resultado;
    resultado.printType = ValueType::INT;
	resultado.type = ValueType::INT;

	assert(value_2.type == ValueType::DOUBLE);
	assert(value_1.type == ValueType::DOUBLE);

	if (std::isnan(value_1.data.doubleValue) || std::isnan(value_2.data.doubleValue)) {
		resultado.data.intValue = 1;
	} else if (value_1.data.doubleValue > value_2.data.doubleValue) {
		resultado.data.intValue = 1;
	} else if (value_1.data.doubleValue == value_2.data.doubleValue) {
		resultado.data.intValue = 0;
	} else {
		resultado.data.intValue = -1;
	}

	topFrame->pushIntoOperandStack(resultado);

	topFrame->pc += 1;
}

void VMEngine::i_ifeq() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value value = topFrame->popTopOfOperandStack();
    assert(value.type == ValueType::INT);
    
    if (value.data.intValue == 0) {
        u1 *code = topFrame->getCode(topFrame->pc);
        u1 byte1 = code[1];
        u1 byte2 = code[2];
        int16_t branchOffset = (byte1 << 8) | byte2;
        topFrame->pc += branchOffset;
    } else {
        topFrame->pc += 3;
    }
}

void VMEngine::i_ifne() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::INT);
	
	if (value.data.intValue != 0) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_iflt() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::INT);
	
	if (value.data.intValue < 0) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_ifge() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::INT);
	
	if (value.data.intValue >= 0) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_ifgt() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::INT);
	
	if (value.data.intValue > 0) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_ifle() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value = topFrame->popTopOfOperandStack();
	assert(value.type == ValueType::INT);
	
	if (value.data.intValue <= 0) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_icmpeq() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::INT);
	assert(value2.type == ValueType::INT);
	
	if (value1.data.intValue == value2.data.intValue) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_icmpne() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::INT);
	assert(value2.type == ValueType::INT);
	
	if (value1.data.intValue != value2.data.intValue) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
	} else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_icmplt() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::INT);
	assert(value2.type == ValueType::INT);
	
	if (value1.data.intValue < value2.data.intValue) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_icmpge() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::INT);
	assert(value2.type == ValueType::INT);
	
	if (value1.data.intValue >= value2.data.intValue) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_icmpgt() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::INT);
	assert(value2.type == ValueType::INT);
	
	if (value1.data.intValue > value2.data.intValue) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_icmple() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::INT);
	assert(value2.type == ValueType::INT);
	
	if (value1.data.intValue <= value2.data.intValue) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_acmpeq() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::REFERENCE);
	assert(value2.type == ValueType::REFERENCE);
	
	if (value1.data.object == value2.data.object) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_if_acmpne() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	Value value2 = topFrame->popTopOfOperandStack();
	Value value1 = topFrame->popTopOfOperandStack();
	assert(value1.type == ValueType::REFERENCE);
	assert(value2.type == ValueType::REFERENCE);

	if (value1.data.object != value2.data.object) {
		u1 *code = topFrame->getCode(topFrame->pc);
		u1 byte1 = code[1];
		u1 byte2 = code[2];
		int16_t branchOffset = (byte1 << 8) | byte2;
		topFrame->pc += branchOffset;
    } else {
		topFrame->pc += 3;
    }
}

void VMEngine::i_goto() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1];
	u1 byte2 = code[2];
	int16_t branchOffset = (byte1 << 8) | byte2;
	topFrame->pc += branchOffset;
}

void VMEngine::i_jsr() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();
	
	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1];
	u1 byte2 = code[2];
	int16_t branchOffset = (byte1 << 8) | byte2;
	
	Value returnAddr;
	returnAddr.type = ValueType::RETURN_ADDR;
	returnAddr.data.returnAddress = topFrame->pc + 3; 
	topFrame->pushIntoOperandStack(returnAddr);
	
	topFrame->pc += branchOffset;
}

// Pode ser modificado pelo wide
void VMEngine::i_ret() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1]; // índice do vetor de variáveis locais
	uint16_t index = (uint16_t) byte1;

	if (_isWide) {
		u1 byte2 = code[2];
		index = (byte1 << 8) | byte2;
	}

	assert(((int16_t)(topFrame->sizeLocalVariables()) > index));
	Value value = topFrame->getLocalVariableValue(index);

	assert(value.type == ValueType::RETURN_ADDR);
	topFrame->changeLocalVariable(value, index);

	topFrame->pc = value.data.returnAddress;
	_isWide = false;
}

void VMEngine::i_tableswitch() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    u1* code = topFrame->getCode(topFrame->pc);
    u1 padding = 4 - (topFrame->pc + 1) % 4;
    padding = (padding == 4) ? 0 : padding;
    
    u1 defaultbyte1 = code[padding + 1];
    u1 defaultbyte2 = code[padding + 2];
    u1 defaultbyte3 = code[padding + 3];
    u1 defaultbyte4 = code[padding + 4];
    int32_t defaultBytes = (defaultbyte1 << 24) | (defaultbyte2 << 16) | (defaultbyte3 << 8) | defaultbyte4;
    
    u1 lowbyte1 = code[padding + 5];
    u1 lowbyte2 = code[padding + 6];
    u1 lowbyte3 = code[padding + 7];
    u1 lowbyte4 = code[padding + 8];
    uint32_t lowbytes = (lowbyte1 << 24) | (lowbyte2 << 16) | (lowbyte3 << 8) | lowbyte4;
    
    u1 highbyte1 = code[padding + 9];
    u1 highbyte2 = code[padding + 10];
    u1 highbyte3 = code[padding + 11];
    u1 highbyte4 = code[padding + 12];
    uint32_t highbytes = (highbyte1 << 24) | (highbyte2 << 16) | (highbyte3 << 8) | highbyte4;
    
    Value keyValue = topFrame->popTopOfOperandStack();
    assert(keyValue.type == ValueType::INT);
    int32_t key = keyValue.data.intValue;
    
    uint32_t i;
    uint32_t baseIndex = padding + 13;
    int32_t offsets = highbytes - lowbytes + 1;
    bool matched = false;
    for (i = 0; i < offsets; i++) {
        if (key == lowbytes) {
            int32_t offset = (code[baseIndex] << 24) | (code[baseIndex+1] << 16) | (code[baseIndex+2] << 8) | code[baseIndex+3];
            topFrame->pc += offset;
            matched = true;
            break;
        }
        lowbytes++;
        baseIndex += 4;
    }
    
    if (!matched) {
        topFrame->pc += defaultBytes; // salto default
    }
}

void VMEngine::i_lookupswitch() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    u1* code = topFrame->getCode(topFrame->pc);
    u1 padding = 4 - (topFrame->pc + 1) % 4;
    padding = (padding == 4) ? 0 : padding;
    
    u1 defaultbyte1 = code[padding + 1];
    u1 defaultbyte2 = code[padding + 2];
    u1 defaultbyte3 = code[padding + 3];
    u1 defaultbyte4 = code[padding + 4];
    int32_t defaultBytes = (defaultbyte1 << 24) | (defaultbyte2 << 16) | (defaultbyte3 << 8) | defaultbyte4;
    
    u1 npairs1 = code[padding + 5];
    u1 npairs2 = code[padding + 6];
    u1 npairs3 = code[padding + 7];
    u1 npairs4 = code[padding + 8];
    uint32_t npairs = (npairs1 << 24) | (npairs2 << 16) | (npairs3 << 8) | npairs4;

    Value keyValue = topFrame->popTopOfOperandStack();
    assert(keyValue.type == ValueType::INT);
    int32_t key = keyValue.data.intValue;
    
    uint32_t i;
    uint32_t baseIndex = padding + 9;
    bool matched = false;
    for (i = 0; i < npairs; i++) {
        int32_t match = (code[baseIndex] << 24) | (code[baseIndex+1] << 16) | (code[baseIndex+2] << 8) | code[baseIndex+3];
        
        if (key == match) {
            int32_t offset = (code[baseIndex+4] << 24) | (code[baseIndex+5] << 16) | (code[baseIndex+6] << 8) | code[baseIndex+7];
            topFrame->pc += offset;
            matched = true;
            break;
        }
        baseIndex += 8;
    }
    
    if (!matched) {
        topFrame->pc += defaultBytes; // salto default
    }
}

void VMEngine::i_ireturn() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value returnValue = topFrame->popTopOfOperandStack();
    assert(returnValue.type == ValueType::INT);
    
    stackFrame.destroyTopFrame();
    
    Frame *newTopFrame = stackFrame.getTopFrame();
    newTopFrame->pushIntoOperandStack(returnValue);
}

void VMEngine::i_lreturn() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value returnValue = topFrame->popTopOfOperandStack();
    assert(returnValue.type == ValueType::LONG);
    assert(topFrame->popTopOfOperandStack().type == ValueType::PADDING); // o debaixo precisa ser padding
    
    stackFrame.destroyTopFrame();
    
    Frame *newTopFrame = stackFrame.getTopFrame();
    Value padding;
    padding.type = ValueType::PADDING;
    newTopFrame->pushIntoOperandStack(padding);
    newTopFrame->pushIntoOperandStack(returnValue);
}

void VMEngine::i_freturn() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value returnValue = topFrame->popTopOfOperandStack();
    assert(returnValue.type == ValueType::FLOAT);
    
    stackFrame.destroyTopFrame();
    
    Frame *newTopFrame = stackFrame.getTopFrame();
    newTopFrame->pushIntoOperandStack(returnValue);
}

void VMEngine::i_dreturn() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value returnValue = topFrame->popTopOfOperandStack();
    assert(returnValue.type == ValueType::DOUBLE);
    assert(topFrame->popTopOfOperandStack().type == ValueType::PADDING); // o debaixo precisa ser padding
    
    stackFrame.destroyTopFrame();
    
    Frame *newTopFrame = stackFrame.getTopFrame();
    
    Value padding;
    padding.type = ValueType::PADDING;
    newTopFrame->pushIntoOperandStack(padding);
    newTopFrame->pushIntoOperandStack(returnValue);
}

void VMEngine::i_areturn() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value returnValue = topFrame->popTopOfOperandStack();
    assert(returnValue.type == ValueType::REFERENCE);
    
    stackFrame.destroyTopFrame();
    
    Frame *newTopFrame = stackFrame.getTopFrame();
    newTopFrame->pushIntoOperandStack(returnValue);
}

void VMEngine::i_return() {
    VMStack &stackFrame = VMStack::getInstance();
    stackFrame.destroyTopFrame();
}

void VMEngine::i_getstatic() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t fieldIndex = (byte1 << 8) | byte2;
    cp_info fieldCP = constantPool[fieldIndex-1];
    assert(fieldCP.tag == CONSTANT_Fieldref); // precisa ser um fieldRef

    CONSTANT_Fieldref_info fieldRef = fieldCP.info.fieldref_info;

    string className = getFormattedConstant(constantPool, fieldRef.class_index);

    cp_info nameAndTypeCP = constantPool[fieldRef.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info fieldNameAndType = nameAndTypeCP.info.nameAndType_info;

    string fieldName = getFormattedConstant(constantPool, fieldNameAndType.name_index);
    string fieldDescriptor = getFormattedConstant(constantPool, fieldNameAndType.descriptor_index);

    // caso especial
    if (className == "java/lang/System" && fieldDescriptor == "Ljava/io/PrintStream;" ) {
        topFrame->pc += 3;
        return;
    }
    // fim do caso especial
    
    MethodArea &methodArea = MethodArea::getInstance();
    ClassRuntime *classRuntime = methodArea.loadClassNamed(className);

    while (classRuntime != NULL) {
        if (classRuntime->fieldExists(fieldName) == false) {
            if (classRuntime->getClassFile()->super_class == 0) {
                classRuntime = NULL;
            } else {
                string superClassName = getFormattedConstant(classRuntime->getClassFile()->constant_pool, classRuntime->getClassFile()->super_class);
                classRuntime = methodArea.loadClassNamed(superClassName);
            }
        } else {
            break;
        }
    }

    if (classRuntime == NULL) {
        cerr << "NoSuchFieldError" << endl;
        exit(1);
    }

    // se a stack frame mudou, é porque teve <clinit> adicionado, então terminar a execução da instrução para eles serem executados.
    if (stackFrame.getTopFrame() != topFrame) return;
    
    Value staticValue = classRuntime->getValueFromField(fieldName);
    switch (staticValue.type) {
        case ValueType::BOOLEAN:
            staticValue.type = ValueType::INT;
            staticValue.printType = ValueType::BOOLEAN;
            break;
        case ValueType::BYTE:
            staticValue.type = ValueType::INT;
            staticValue.printType = ValueType::BYTE;
            break;
        case ValueType::SHORT:
            staticValue.type = ValueType::INT;
            staticValue.printType = ValueType::SHORT;
            break;
        case ValueType::INT:
            staticValue.type = ValueType::INT;
            staticValue.printType = ValueType::INT;
            break;
        default:
            break;
    }
    
    if (staticValue.type == ValueType::DOUBLE || staticValue.type == ValueType::LONG) {
        Value paddingValue;
        paddingValue.type = ValueType::PADDING;
        topFrame->pushIntoOperandStack(paddingValue);
    }

    topFrame->pushIntoOperandStack(staticValue);

    topFrame->pc += 3;
}

void VMEngine::i_putstatic() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t fieldIndex = (byte1 << 8) | byte2;
    cp_info fieldCP = constantPool[fieldIndex-1];
    assert(fieldCP.tag == CONSTANT_Fieldref); // precisa ser um fieldRef

    CONSTANT_Fieldref_info fieldRef = fieldCP.info.fieldref_info;

    string className = getFormattedConstant(constantPool, fieldRef.class_index);

    cp_info nameAndTypeCP = constantPool[fieldRef.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info fieldNameAndType = nameAndTypeCP.info.nameAndType_info;

    string fieldName = getFormattedConstant(constantPool, fieldNameAndType.name_index);
    string fieldDescriptor = getFormattedConstant(constantPool, fieldNameAndType.descriptor_index);

    MethodArea &methodArea = MethodArea::getInstance();
    ClassRuntime *classRuntime = methodArea.loadClassNamed(className);

    while (classRuntime != NULL) {
        if (classRuntime->fieldExists(fieldName) == false) {
            if (classRuntime->getClassFile()->super_class == 0) {
                classRuntime = NULL;
            } else {
                string superClassName = getFormattedConstant(classRuntime->getClassFile()->constant_pool, classRuntime->getClassFile()->super_class);
                classRuntime = methodArea.loadClassNamed(superClassName);
            }
        } else {
            break;
        }
    }

    if (classRuntime == NULL) {
        cerr << "NoSuchFieldError" << endl;
        exit(1);
    }

    // se a stack frame mudou, é porque teve <clinit> adicionado, então terminar a execução da instrução para eles serem executados.
    if (stackFrame.getTopFrame() != topFrame) return;
    
    Value topValue = topFrame->popTopOfOperandStack();
    if (topValue.type == ValueType::DOUBLE || topValue.type == ValueType::LONG) {
        topFrame->popTopOfOperandStack(); // removendo padding
    } else {
        switch (fieldDescriptor[0]) {
            case 'B':
                topValue.type = ValueType::BYTE;
                topValue.printType = ValueType::BYTE;
                break;
            case 'C':
                topValue.type = ValueType::CHAR;
                topValue.type = ValueType::CHAR;
                break;
            case 'S':
                topValue.type = ValueType::SHORT;
                topValue.type = ValueType::SHORT;
                break;
            case 'Z':
                topValue.type = ValueType::BOOLEAN;
                topValue.type = ValueType::BOOLEAN;
                break;
        }
    }

    classRuntime->putValueIntoField(topValue, fieldName);

    topFrame->pc += 3;
}

void VMEngine::i_getfield() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t fieldIndex = (byte1 << 8) | byte2;
    cp_info fieldCP = constantPool[fieldIndex-1];
    assert(fieldCP.tag == CONSTANT_Fieldref); // precisa ser um fieldRef

    CONSTANT_Fieldref_info fieldRef = fieldCP.info.fieldref_info;

    string className = getFormattedConstant(constantPool, fieldRef.class_index);

    cp_info nameAndTypeCP = constantPool[fieldRef.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info fieldNameAndType = nameAndTypeCP.info.nameAndType_info;

    string fieldName = getFormattedConstant(constantPool, fieldNameAndType.name_index);
    string fieldDescriptor = getFormattedConstant(constantPool, fieldNameAndType.descriptor_index);

    Value objectValue = topFrame->popTopOfOperandStack();
    assert(objectValue.type == ValueType::REFERENCE);
    Object *object = objectValue.data.object;
    assert(object->objectType() == ObjectType::CLASS_INSTANCE);
    ClassInstance *classInstance = (ClassInstance *) object;

    if (!classInstance->fieldExists(fieldName)) {
        cerr << "NoSuchFieldError" << endl;
        exit(1);
    }

    Value fieldValue = classInstance->getValueFromField(fieldName);
    switch (fieldValue.type) {
        case ValueType::BOOLEAN:
            fieldValue.type = ValueType::INT;
            fieldValue.printType = ValueType::BOOLEAN;
            break;
        case ValueType::BYTE:
            fieldValue.type = ValueType::INT;
            fieldValue.printType = ValueType::BYTE;
            break;
        case ValueType::SHORT:
            fieldValue.type = ValueType::INT;
            fieldValue.printType = ValueType::SHORT;
            break;
        case ValueType::INT:
            fieldValue.type = ValueType::INT;
            fieldValue.printType = ValueType::INT;
            break;
        default:
            break;
    }
    
    if (fieldValue.type == ValueType::DOUBLE || fieldValue.type == ValueType::LONG) {
        Value paddingValue;
        paddingValue.type = ValueType::PADDING;
        topFrame->pushIntoOperandStack(paddingValue);
    }

    topFrame->pushIntoOperandStack(fieldValue);

    topFrame->pc += 3;
}

void VMEngine::i_putfield() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t fieldIndex = (byte1 << 8) | byte2;
    cp_info fieldCP = constantPool[fieldIndex-1];
    assert(fieldCP.tag == CONSTANT_Fieldref); // precisa ser um fieldRef

    CONSTANT_Fieldref_info fieldRef = fieldCP.info.fieldref_info;

    string className = getFormattedConstant(constantPool, fieldRef.class_index);

    cp_info nameAndTypeCP = constantPool[fieldRef.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info fieldNameAndType = nameAndTypeCP.info.nameAndType_info;

    string fieldName = getFormattedConstant(constantPool, fieldNameAndType.name_index);
    string fieldDescriptor = getFormattedConstant(constantPool, fieldNameAndType.descriptor_index);

    Value valueToBeInserted = topFrame->popTopOfOperandStack();
    if (valueToBeInserted.type == ValueType::DOUBLE || valueToBeInserted.type == ValueType::LONG) {
        topFrame->popTopOfOperandStack(); // removendo padding
    } else {
        switch (fieldDescriptor[0]) {
            case 'B':
                valueToBeInserted.type = ValueType::BYTE;
                valueToBeInserted.printType = ValueType::BYTE;
                break;
            case 'C':
                valueToBeInserted.type = ValueType::CHAR;
                valueToBeInserted.printType = ValueType::CHAR;
                break;
            case 'S':
                valueToBeInserted.type = ValueType::SHORT;
                valueToBeInserted.printType = ValueType::SHORT;
                break;
            case 'Z':
                valueToBeInserted.type = ValueType::BOOLEAN;
                valueToBeInserted.printType = ValueType::BOOLEAN;
                break;
        }
    }

    Value objectValue = topFrame->popTopOfOperandStack();
    assert(objectValue.type == ValueType::REFERENCE);
    Object *object = objectValue.data.object;
    assert(object->objectType() == ObjectType::CLASS_INSTANCE);
    ClassInstance *classInstance = (ClassInstance *) object;

    classInstance->putValueIntoField(valueToBeInserted, fieldName);

    topFrame->pc += 3;
}

void VMEngine::i_invokevirtual() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    stack<Value> operandStackBackup = topFrame->backupOperandStack();
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t methodIndex = (byte1 << 8) | byte2;
    cp_info methodCP = constantPool[methodIndex-1];
    assert(methodCP.tag == CONSTANT_Methodref); // precisa referenciar um método

    CONSTANT_Methodref_info methodInfo = methodCP.info.methodref_info;

    string className = getFormattedConstant(constantPool, methodInfo.class_index);

    cp_info nameAndTypeCP = constantPool[methodInfo.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info methodNameAndType = nameAndTypeCP.info.nameAndType_info;

    string methodName = getFormattedConstant(constantPool, methodNameAndType.name_index);
    string methodDescriptor = getFormattedConstant(constantPool, methodNameAndType.descriptor_index);

    if (className.find("java/") != string::npos) {
        // simulando println ou print
        if (className == "java/io/PrintStream" && (methodName == "print" || methodName == "println")) {
            if (methodDescriptor != "()V") {
                Value printValue = topFrame->popTopOfOperandStack();

                if (printValue.type == ValueType::INT) {
                    switch (printValue.printType) {
                        case ValueType::BOOLEAN:
                        	printf("");
                            printf("%s", printValue.data.booleanValue == 0 ? "false" : "true");
                            break;
                        case ValueType::BYTE:
                            printf("%d", printValue.data.byteValue);
                            break;
                        case ValueType::CHAR:
                            printf("%c", printValue.data.charValue);
                            break;
                        case ValueType::SHORT:
                            printf("%d", printValue.data.shortValue);
                            break;
                        default:
                            printf("%d", printValue.data.intValue);
                            break;
                    }
                } else {
                    switch (printValue.type) {
                        case ValueType::DOUBLE:
                            topFrame->popTopOfOperandStack(); // removendo padding
                            printf("%f", printValue.data.doubleValue);
                            break;
                        case ValueType::FLOAT:
                            printf("%f", printValue.data.floatValue);
                            break;
                        case ValueType::LONG:
                            topFrame->popTopOfOperandStack(); // removendo padding
                            printf("%lld", printValue.data.longValue);
                            break;
                        case ValueType::REFERENCE:
                            assert(printValue.data.object->objectType() == ObjectType::STRING_INSTANCE);
                            printf("%s", ((StringObject *) printValue.data.object)->getString().c_str());
                            break;
                        case ValueType::BOOLEAN:
                            printf("%s", printValue.data.booleanValue == 0 ? "false" : "true");
                            break;
                        case ValueType::BYTE:
                            printf("%d", printValue.data.byteValue);
                            break;
                        case ValueType::CHAR:
                            printf("%c", printValue.data.charValue);
                            break;
                        case ValueType::SHORT:
                            printf("%d", printValue.data.shortValue);
                            break;
                        default:
                            cerr << "Tentando printar tipo de dado invalido: " << printValue.type << endl;
                            exit(1);
                            break;
                    }
                }
            }

            if (methodName == "println") printf("\n");
        } else if (className == "java/lang/String" && methodName == "equals") {
            Value strValue1 = topFrame->popTopOfOperandStack();
            Value strValue2 = topFrame->popTopOfOperandStack();
            assert(strValue1.type == ValueType::REFERENCE);
            assert(strValue2.type == ValueType::REFERENCE);
            assert(strValue1.data.object->objectType() == ObjectType::STRING_INSTANCE);
            assert(strValue2.data.object->objectType() == ObjectType::STRING_INSTANCE);
            
            StringObject *str1 = (StringObject*) strValue1.data.object;
            StringObject *str2 = (StringObject*) strValue2.data.object;
            
            Value result;
            result.printType = ValueType::INT;
            result.type = ValueType::INT;
            if (str1->getString() == str2->getString()) {
                result.data.intValue = 1;
            } else {
                result.data.intValue = 0;
            }
            topFrame->pushIntoOperandStack(result);
        } else if (className == "java/lang/String" && methodName == "length") {	
            Value strValue = topFrame->popTopOfOperandStack();
            assert(strValue.type == ValueType::REFERENCE);		
            assert(strValue.data.object->objectType() == ObjectType::STRING_INSTANCE);		
                    
            StringObject *str = (StringObject*) strValue.data.object;		
                    
            Value result;
            result.printType = ValueType::INT;
            result.type = ValueType::INT;		
            result.data.intValue = (str->getString()).size();		
            topFrame->pushIntoOperandStack(result);
        } else {
            cerr << "Tentando invocar metodo de instancia invalido: " << methodName << endl;
            exit(1);
        }
    } else {
        uint16_t nargs = 0; // numero de argumentos contidos na pilha de operandos
        uint16_t i = 1; // pulando o primeiro '('
        while (methodDescriptor[i] != ')') {
            char baseType = methodDescriptor[i];
            if (baseType == 'D' || baseType == 'J') {
                nargs += 2;
            } else if (baseType == 'L') {
                nargs++;
                while (methodDescriptor[++i] != ';');
            } else if (baseType == '[') {
                nargs++;
                while (methodDescriptor[++i] == '[');
                if (methodDescriptor[i] == 'L') while (methodDescriptor[++i] != ';');
            } else {
                nargs++;
            }
            i++;
        }

        vector<Value> args;
        for (int i = 0; i < nargs; i++) {
            Value value = topFrame->popTopOfOperandStack();
            if (value.type == ValueType::PADDING) {
                args.insert(args.begin() + 1, value); // adicionando o padding após o valor double/long.
            } else {
                args.insert(args.begin(), value);
            }
        }

        Value objectValue = topFrame->popTopOfOperandStack();
        assert(objectValue.type == ValueType::REFERENCE); // necessita ser uma referência para objeto
        args.insert(args.begin(), objectValue);

        Object *object = objectValue.data.object;
        assert(object->objectType() == ObjectType::CLASS_INSTANCE); // objeto precisa ser uma instância
        ClassInstance *instance = (ClassInstance *) object;

        MethodArea &methodArea = MethodArea::getInstance();
        ClassRuntime *classRuntime = methodArea.loadClassNamed(className);
        
        Frame *newFrame = new Frame(instance, classRuntime, methodName, methodDescriptor, args);

        // se a stack frame mudou, é porque teve <clinit> adicionado, então terminar a execução da instrução para eles serem executados.
        if (stackFrame.getTopFrame() != topFrame) {
            topFrame->setOperandStackFromBackup(operandStackBackup);
            delete newFrame;
            return;
        }

        stackFrame.addFrame(newFrame);
    }

    topFrame->pc += 3;
}

void VMEngine::i_invokespecial() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    stack<Value> operandStackBackup = topFrame->backupOperandStack();
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t methodIndex = (byte1 << 8) | byte2;
    cp_info methodCP = constantPool[methodIndex-1];
    assert(methodCP.tag == CONSTANT_Methodref); // precisa referenciar um método

    CONSTANT_Methodref_info methodInfo = methodCP.info.methodref_info;

    string className = getFormattedConstant(constantPool, methodInfo.class_index);

    cp_info nameAndTypeCP = constantPool[methodInfo.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info methodNameAndType = nameAndTypeCP.info.nameAndType_info;

    string methodName = getFormattedConstant(constantPool, methodNameAndType.name_index);
    string methodDescriptor = getFormattedConstant(constantPool, methodNameAndType.descriptor_index);
    
    // casos especiais
    if ((className == "java/lang/Object" || className == "java/lang/String") && methodName == "<init>") {
        if (className == "java/lang/String") {
            topFrame->popTopOfOperandStack();
        }
        
        topFrame->pc += 3;
        return;
    }
    // fim dos casos especiais
    
    if (className.find("java/") != string::npos) {
        cerr << "Tentando invocar metodo especial invalido: " << methodName << endl;
        exit(1);
    } else {
        uint16_t nargs = 0; // numero de argumentos contidos na pilha de operandos
        uint16_t i = 1; // pulando o primeiro '('
        while (methodDescriptor[i] != ')') {
            char baseType = methodDescriptor[i];
            if (baseType == 'D' || baseType == 'J') {
                nargs += 2;
            } else if (baseType == 'L') {
                nargs++;
                while (methodDescriptor[++i] != ';');
            } else if (baseType == '[') {
                nargs++;
                while (methodDescriptor[++i] == '[');
                if (methodDescriptor[i] == 'L') while (methodDescriptor[++i] != ';');
            } else {
                nargs++;
            }
            i++;
        }

        vector<Value> args;
        for (int i = 0; i < nargs; i++) {
            Value value = topFrame->popTopOfOperandStack();
            if (value.type == ValueType::PADDING) {
                args.insert(args.begin() + 1, value); // adicionando o padding após o valor double/long.
            } else {
                args.insert(args.begin(), value);
            }
        }

        Value objectValue = topFrame->popTopOfOperandStack();
        assert(objectValue.type == ValueType::REFERENCE); // necessita ser uma referência para objeto
        args.insert(args.begin(), objectValue);

        Object *object = objectValue.data.object;
        assert(object->objectType() == ObjectType::CLASS_INSTANCE); // objeto precisa ser uma instância
        ClassInstance *instance = (ClassInstance *) object;

        MethodArea &methodArea = MethodArea::getInstance();
        ClassRuntime *classRuntime = methodArea.loadClassNamed(className);
        
        Frame *newFrame = new Frame(instance, classRuntime, methodName, methodDescriptor, args);

        // se a stack frame mudou, é porque teve <clinit> adicionado, então terminar a execução da instrução para eles serem executados.
        if (stackFrame.getTopFrame() != topFrame) {
            topFrame->setOperandStackFromBackup(operandStackBackup);
            delete newFrame;
            return;
        }

        stackFrame.addFrame(newFrame);
    }

    topFrame->pc += 3;
}

void VMEngine::i_invokestatic() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    stack<Value> operandStackBackup = topFrame->backupOperandStack();
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t methodIndex = (byte1 << 8) | byte2;
    cp_info methodCP = constantPool[methodIndex-1];
    assert(methodCP.tag == CONSTANT_Methodref); // precisa referenciar um método

    CONSTANT_Methodref_info methodInfo = methodCP.info.methodref_info;

    string className = getFormattedConstant(constantPool, methodInfo.class_index);

    cp_info nameAndTypeCP = constantPool[methodInfo.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info methodNameAndType = nameAndTypeCP.info.nameAndType_info;

    string methodName = getFormattedConstant(constantPool, methodNameAndType.name_index);
    string methodDescriptor = getFormattedConstant(constantPool, methodNameAndType.descriptor_index);

    if (className == "java/lang/Object" && methodName == "registerNatives") {
        topFrame->pc += 3;
        return;
    }
    
    if (className.find("java/") != string::npos) {
        cerr << "Tentando invocar metodo estatico invalido: " << methodName << endl;
        exit(1);
    } else {
        uint16_t nargs = 0; // numero de argumentos contidos na pilha de operandos
        uint16_t i = 1; // pulando o primeiro '('
        while (methodDescriptor[i] != ')') {
            char baseType = methodDescriptor[i];
            if (baseType == 'D' || baseType == 'J') {
                nargs += 2;
            } else if (baseType == 'L') {
                nargs++;
                while (methodDescriptor[++i] != ';');
            } else if (baseType == '[') {
                nargs++;
                while (methodDescriptor[++i] == '[');
                if (methodDescriptor[i] == 'L') while (methodDescriptor[++i] != ';');
            } else {
                nargs++;
            }
            i++;
        }

        vector<Value> args;
        for (int i = 0; i < nargs; i++) {
            Value value = topFrame->popTopOfOperandStack();
            if (value.type == ValueType::PADDING) {
                args.insert(args.begin() + 1, value); // adicionando o padding após o valor double/long.
            } else {
                args.insert(args.begin(), value);
            }
        }

        MethodArea &methodArea = MethodArea::getInstance();
        ClassRuntime *classRuntime = methodArea.loadClassNamed(className);
        Frame *newFrame = new Frame(classRuntime, methodName, methodDescriptor, args);

        // se a stack frame mudou, é porque teve <clinit> adicionado, então terminar a execução da instrução para eles serem executados.
        if (stackFrame.getTopFrame() != topFrame) {
            topFrame->setOperandStackFromBackup(operandStackBackup);
            delete newFrame;
            return;
        }

        stackFrame.addFrame(newFrame);
    }

    topFrame->pc += 3;
}

void VMEngine::i_invokeinterface() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    stack<Value> operandStackBackup = topFrame->backupOperandStack();
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t methodIndex = (byte1 << 8) | byte2;
    cp_info methodCP = constantPool[methodIndex-1];
    assert(methodCP.tag == CONSTANT_Methodref || methodCP.tag == CONSTANT_InterfaceMethodref); // precisa referenciar um método

    CONSTANT_Methodref_info methodInfo = methodCP.info.methodref_info;

    string className = getFormattedConstant(constantPool, methodInfo.class_index);

    cp_info nameAndTypeCP = constantPool[methodInfo.name_and_type_index-1];
    assert(nameAndTypeCP.tag == CONSTANT_NameAndType); // precisa ser um nameAndType

    CONSTANT_NameAndType_info methodNameAndType = nameAndTypeCP.info.nameAndType_info;

    string methodName = getFormattedConstant(constantPool, methodNameAndType.name_index);
    string methodDescriptor = getFormattedConstant(constantPool, methodNameAndType.descriptor_index);

    if (className.find("java/") != string::npos) {
        cerr << "  invalid call method      : " << methodName << endl;
        exit(1);
    } else {
        uint16_t nargs = 0; //   operandos
        uint16_t i = 1; //
        while (methodDescriptor[i] != ')') {
            char baseType = methodDescriptor[i];
            if (baseType == 'D' || baseType == 'J') {
                nargs += 2;
            } else if (baseType == 'L') {
                nargs++;
                while (methodDescriptor[++i] != ';');
            } else if (baseType == '[') {
                nargs++;
                while (methodDescriptor[++i] == '[');
                if (methodDescriptor[i] == 'L') while (methodDescriptor[++i] != ';');
            } else {
                nargs++;
            }
            i++;
        }

        vector<Value> args;
        for (int i = 0; i < nargs; i++) {
            Value value = topFrame->popTopOfOperandStack();
            if (value.type == ValueType::PADDING) {
                args.insert(args.begin() + 1, value); //      double/long.
            } else {
                args.insert(args.begin(), value);
            }
        }

        Value objectValue = topFrame->popTopOfOperandStack();
        assert(objectValue.type == ValueType::REFERENCE); //
        args.insert(args.begin(), objectValue);

        Object *object = objectValue.data.object;
        assert(object->objectType() == ObjectType::CLASS_INSTANCE); // object
        ClassInstance *instance = (ClassInstance *) object;

        MethodArea &methodArea = MethodArea::getInstance();
        methodArea.loadClassNamed(className); //     interface
        
        Frame *newFrame = new Frame(instance, instance->getClassRuntime(), methodName, methodDescriptor, args);

        //
        if (stackFrame.getTopFrame() != topFrame) {
            topFrame->setOperandStackFromBackup(operandStackBackup);
            delete newFrame;
            return;
        }

        stackFrame.addFrame(newFrame);
    }

    topFrame->pc += 5;
}

void VMEngine::i_new() {
    VMStack &stackFrame = VMStack::getInstance();       
    Frame *topFrame = stackFrame.getTopFrame();     
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);

    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t classIndex = (byte1 << 8) | byte2;
    cp_info classCP = constantPool[classIndex-1];
    assert(classCP.tag == CONSTANT_Class);
    
    CONSTANT_Class_info classInfo = classCP.info.class_info; // Formata nome da classe
    string className = getFormattedConstant(constantPool, classInfo.name_index);

    Object *object;
    if (className == "java/lang/String") {
        object = new StringObject();
    } else {
        MethodArea &methodArea = MethodArea::getInstance();
        ClassRuntime *classRuntime = methodArea.loadClassNamed(className);
        object = new ClassInstance(classRuntime); // Cria instancia da classe e coloca na heap
    }
    
    // Armazena referência na pilha
    Value objectref;
    objectref.data.object = object;
    objectref.type = ValueType::REFERENCE;
    topFrame->pushIntoOperandStack(objectref);
    
    topFrame->pc += 3;
}

void VMEngine::i_newarray() {
    VMStack &stackFrame = VMStack::getInstance();       
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value count = topFrame->popTopOfOperandStack(); // Número de elementos no array
    assert(count.type == ValueType::INT);
    
    if (count.data.intValue < 0) {
        cerr << "NegativeArraySizeException" << endl;
        exit(1);
    }
    
    ArrayObject *array; // array que será criado
    Value value; // elemento que irá popular o array
    value.data.longValue = 0; // inicializando Value com 0s
    
    Value padding; // padding poderá ser usado
    padding.type = ValueType::PADDING;
    
    u1 *code = topFrame->getCode(topFrame->pc);
    switch (code[1]) { // argumento representa tipo do array
        case 4:
            array = new ArrayObject(ValueType::BOOLEAN);
            value.type = ValueType::BOOLEAN;
            value.printType = ValueType::BOOLEAN;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
        case 5:
            array = new ArrayObject(ValueType::CHAR);
            value.type = ValueType::CHAR;
            value.printType = ValueType::CHAR;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
        case 6:
            array = new ArrayObject(ValueType::FLOAT);
            value.type = ValueType::FLOAT;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
        case 7:
            array = new ArrayObject(ValueType::DOUBLE);
            value.type = ValueType::DOUBLE;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
        case 8:
            array = new ArrayObject(ValueType::BYTE);
            value.type = ValueType::BYTE;
            value.printType = ValueType::BYTE;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
        case 9:
            array = new ArrayObject(ValueType::SHORT);
            value.type = ValueType::SHORT;
            value.printType = ValueType::SHORT;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
        case 10:
            array = new ArrayObject(ValueType::INT);
            value.type = ValueType::INT;
            value.printType = ValueType::INT;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
        case 11:
            array = new ArrayObject(ValueType::LONG);
            value.type = ValueType::LONG;
            for (int i = 0; i < count.data.intValue; i++) {
                array->pushValue(value);
            }
            break;
    }
    
    Value arrayref; // Referencia pro array na pilha de operandos
    arrayref.type = ValueType::REFERENCE;
    arrayref.data.object = array;
    
    topFrame->pushIntoOperandStack(arrayref);
    topFrame->pc += 2;
}

void VMEngine::i_anewarray() {
    VMStack &stackFrame = VMStack::getInstance();       
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value count = topFrame->popTopOfOperandStack(); // Número de elementos no array
    assert(count.type == ValueType::INT);
    if (count.data.intValue < 0) {
        cerr << "NegativeArraySizeException" << endl;
        exit(1);
    }
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);
    u1 byte1 = code[1];
    u1 byte2 = code[2];

    uint16_t classIndex = (byte1 << 8) | byte2; // Índice na pool de constantes
    cp_info classCP = constantPool[classIndex-1];
    assert(classCP.tag == CONSTANT_Class);
    
    CONSTANT_Class_info classInfo = classCP.info.class_info; // Formata nome da classe
    string className = getFormattedConstant(constantPool, classInfo.name_index);

    if (className != "java/lang/String") {
        int i = 0;
        while (className[i] == '[') i++;
        if (className[i] == 'L') {
            MethodArea &methodArea = MethodArea::getInstance();
            methodArea.loadClassNamed(className.substr(i+1, className.size()-i-2)); // carrega a classe de referência (se ainda não foi).
        }
    }

    // criando objeto da classe instanciada
    Value objectref;
    objectref.type = ValueType::REFERENCE;
    objectref.data.object = new ArrayObject(ValueType::REFERENCE);
    
    // populando array com NULL
    Value nullValue;
    nullValue.type = ValueType::REFERENCE;
    nullValue.data.object = NULL;
    for (int i = 0; i < count.data.intValue; i++) {
        ((ArrayObject *) objectref.data.object)->pushValue(nullValue);
    }

    topFrame->pushIntoOperandStack(objectref);
    
    topFrame->pc += 3;
}

void VMEngine::i_arraylength() {
    VMStack &stackFrame = VMStack::getInstance();       
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value arrayref = topFrame->popTopOfOperandStack();  
    assert(arrayref.type == ValueType::REFERENCE);
    if (arrayref.data.object == NULL) {
        cerr << "NullPointerException" << endl;
        exit(1);
    }
    
    Value length;
    length.type = ValueType::INT;
    length.data.intValue = ((ArrayObject *) arrayref.data.object)->getSize();
    
    topFrame->pushIntoOperandStack(length);
    topFrame->pc += 1 ;
}

void VMEngine::i_athrow() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    topFrame->pc += 1;
}

void VMEngine::i_checkcast() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    MethodArea &methodArea = MethodArea::getInstance();
    
    u1 *code = topFrame->getCode(topFrame->pc);
    u1 byte1 = code[1];
    u1 byte2 = code[2];
    
    u2 cpIndex = (byte1 << 8) | byte2;
    cp_info *constantPool = *(topFrame->getConstantPool());
    cp_info cpElement = constantPool[cpIndex-1];
    assert(cpElement.tag == CONSTANT_Class);
    string className = getFormattedConstant(constantPool, cpIndex);
    
    Value objectrefValue = topFrame->popTopOfOperandStack();
    assert(objectrefValue.type == ValueType::REFERENCE);
    
    Value resultValue;
    resultValue.type = ValueType::INT;
    
    if (objectrefValue.data.object == NULL) {
        cerr << "ClassCastException" << endl;
        exit(1);
    } else {
        Object *obj = objectrefValue.data.object;
        
        if (obj->objectType() == ObjectType::CLASS_INSTANCE) {
            ClassInstance *classInstance = (ClassInstance *) obj;
            ClassRuntime *classRuntime = classInstance->getClassRuntime();
            
            bool found = false;
            while (!found) {
                ClassFile *classFile = classRuntime->getClassFile();
                string currClassName = getFormattedConstant(classFile->constant_pool, classFile->this_class);
                
                if (currClassName == className) {
                    found = true;
                } else {
                    if (classFile->super_class == 0) {
                        break;
                    } else {
                        string superClassName = getFormattedConstant(classFile->constant_pool, classFile->this_class);
                        classRuntime = methodArea.loadClassNamed(superClassName);
                    }
                }
            }
            
            resultValue.data.intValue = found ? 1 : 0;
        } else if (obj->objectType() == ObjectType::STRING_INSTANCE) {
            resultValue.data.intValue = (className == "java/lang/String" || className == "java/lang/Object") ? 1 : 0;
        } else {
            if (className == "java/lang/Object") {
                resultValue.data.intValue = 1;
            } else {
                resultValue.data.intValue = 0;
            }
        }
    }
    
    topFrame->pushIntoOperandStack(resultValue);
    
    topFrame->pc += 3;
}

void VMEngine::i_instanceof() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    MethodArea &methodArea = MethodArea::getInstance();

    u1 *code = topFrame->getCode(topFrame->pc);
    u1 byte1 = code[1];
    u1 byte2 = code[2];
    
    u2 cpIndex = (byte1 << 8) | byte2;
    cp_info *constantPool = *(topFrame->getConstantPool());
    cp_info cpElement = constantPool[cpIndex-1];
    assert(cpElement.tag == CONSTANT_Class);
    string className = getFormattedConstant(constantPool, cpIndex);
    
    Value objectrefValue = topFrame->popTopOfOperandStack();
    assert(objectrefValue.type == ValueType::REFERENCE);
    
    Value resultValue;
    resultValue.type = ValueType::INT;

    if (objectrefValue.data.object == NULL) {
        resultValue.data.intValue = 0;
    } else {
        Object *obj = objectrefValue.data.object;
        
        if (obj->objectType() == ObjectType::CLASS_INSTANCE) {
            ClassInstance *classInstance = (ClassInstance *) obj;
            ClassRuntime *classRuntime = classInstance->getClassRuntime();
            
            bool found = false;
            while (!found) {
                ClassFile *classFile = classRuntime->getClassFile();
                string currClassName = getFormattedConstant(classFile->constant_pool, classFile->this_class);
                
                if (currClassName == className) {
                    found = true;
                } else {
                    if (classFile->super_class == 0) {
                        break;
                    } else {
                        string superClassName = getFormattedConstant(classFile->constant_pool, classFile->this_class);
                        classRuntime = methodArea.loadClassNamed(superClassName);
                    }
                }
            }
            
            resultValue.data.intValue = found ? 1 : 0;
        } else if (obj->objectType() == ObjectType::STRING_INSTANCE) {
            resultValue.data.intValue = (className == "java/lang/String" || className == "java/lang/Object") ? 1 : 0;
        } else {
            if (className == "java/lang/Object") {
                resultValue.data.intValue = 1;
            } else {
                resultValue.data.intValue = 0;
            }
        }
    }
    
    topFrame->pushIntoOperandStack(resultValue);
    
    topFrame->pc += 3;
}

void VMEngine::i_monitorenter() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    topFrame->pc += 1;
}

void VMEngine::i_monitorexit() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    topFrame->pc += 1;
}

void VMEngine::i_wide() {
	VMStack &stackFrame = VMStack::getInstance();       
    Frame *topFrame = stackFrame.getTopFrame();
	_isWide = true;
	topFrame->pc += 1;
}

void VMEngine::i_multianewarray() {
    VMStack &stackFrame = VMStack::getInstance();       
    Frame *topFrame = stackFrame.getTopFrame();
    
    cp_info *constantPool = *(topFrame->getConstantPool());
    u1 *code = topFrame->getCode(topFrame->pc);
    u1 byte1 = code[1];
    u1 byte2 = code[2];
    u1 dimensions = code[3];
    assert(dimensions >= 1);

    uint16_t classIndex = (byte1 << 8) | byte2;
    cp_info classCP = constantPool[classIndex-1];
    assert(classCP.tag == CONSTANT_Class);
    
    CONSTANT_Class_info classInfo = classCP.info.class_info;
    string className = getFormattedConstant(constantPool, classInfo.name_index);
    
    // obter o tipo dentro de className:
    ValueType valueType;
    int i = 0;
    while (className[i] == '[') i++;
    
    string multiArrayType = className.substr(i+1, className.size()-i-2); // em caso de ser uma referência (e.g. [[[Ljava/lang/String;)
    
    switch (className[i]) {
        case 'L':
            if (multiArrayType != "java/lang/String") {
                MethodArea &methodArea = MethodArea::getInstance();
                methodArea.loadClassNamed(multiArrayType); // verifica se existe classe com esse nome
            }
            valueType = ValueType::REFERENCE;
            break;
        case 'B':
            valueType = ValueType::BYTE;
            break;
        case 'C':
            valueType = ValueType::CHAR;
            break;
        case 'D':
            valueType = ValueType::DOUBLE;
            break;
        case 'F':
            valueType = ValueType::FLOAT;
            break;
        case 'I':
            valueType = ValueType::INT;
            break;
        case 'J':
            valueType = ValueType::LONG;
            break;
        case 'S':
            valueType = ValueType::SHORT;
            break;
        case 'Z':
            valueType = ValueType::BOOLEAN;
            break;
        default:
            cerr << "Descritor invalido em multianewarray" << endl;
            exit(1);
    }
    
    stack<int> count;
    for (int i = 0; i < dimensions; i++) {
        Value dimLength = topFrame->popTopOfOperandStack();
        assert(dimLength.type == ValueType::INT);
        count.push(dimLength.data.intValue);
    }
    
    ArrayObject *array = new ArrayObject((dimensions > 1) ? ValueType::REFERENCE : valueType);
    populateMultiarray(array, valueType, count);
    
    Value arrayValue;
    arrayValue.type = ValueType::REFERENCE;
    arrayValue.data.object = array;
    
    topFrame->pushIntoOperandStack(arrayValue);
    
    topFrame->pc += 4;
}

void VMEngine::i_ifnull() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value referenceValue = topFrame->popTopOfOperandStack();
    assert(referenceValue.type == ValueType::REFERENCE);
    
    if (referenceValue.data.object == NULL) {
        u1 *code = topFrame->getCode(topFrame->pc);
        u1 byte1 = code[1];
        u1 byte2 = code[2];
        int16_t branch =  (byte1 << 8) | byte2;
        topFrame->pc += branch;
    } else {
        topFrame->pc += 3;
    }
}

void VMEngine::i_ifnonnull() {
    VMStack &stackFrame = VMStack::getInstance();
    Frame *topFrame = stackFrame.getTopFrame();
    
    Value referenceValue = topFrame->popTopOfOperandStack();
    assert(referenceValue.type == ValueType::REFERENCE);
    
    if (referenceValue.data.object != NULL) {
        u1 *code = topFrame->getCode(topFrame->pc);
        u1 byte1 = code[1];
        u1 byte2 = code[2];
        int16_t branch =  (byte1 << 8) | byte2;
        topFrame->pc += branch;
    } else {
        topFrame->pc += 3;
    }
}

void VMEngine::i_goto_w() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1];
	u1 byte2 = code[2];
	u1 byte3 = code[3];
	u1 byte4 = code[4];
	int32_t branchOffset = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

	topFrame->pc += branchOffset;
	assert(topFrame->pc < (int32_t)topFrame->sizeCode());
}

void VMEngine::i_jsr_w() {
	VMStack &stackFrame = VMStack::getInstance();
	Frame *topFrame = stackFrame.getTopFrame();

	u1 *code = topFrame->getCode(topFrame->pc);
	u1 byte1 = code[1];
	u1 byte2 = code[2];
	u1 byte3 = code[3];
	u1 byte4 = code[4];
	int32_t branchOffset = (byte1 << 24) | (byte2 << 16) | (byte3 << 8)| byte4;

	Value returnAddr;
	returnAddr.type = ValueType::RETURN_ADDR;
	returnAddr.data.returnAddress = topFrame->pc + 5;
	topFrame->pushIntoOperandStack(returnAddr);

	topFrame->pc += branchOffset;
	assert(topFrame->pc < (int32_t)topFrame->sizeCode());
}

void VMEngine::initInstructions() {
    _instructionFunctions[0x00] = &VMEngine::i_nop;
    _instructionFunctions[0x01] = &VMEngine::i_aconst_null;
    _instructionFunctions[0x02] = &VMEngine::i_iconst_m1;
    _instructionFunctions[0x03] = &VMEngine::i_iconst_0;
    _instructionFunctions[0x04] = &VMEngine::i_iconst_1;
    _instructionFunctions[0x05] = &VMEngine::i_iconst_2;
    _instructionFunctions[0x06] = &VMEngine::i_iconst_3;
    _instructionFunctions[0x07] = &VMEngine::i_iconst_4;
    _instructionFunctions[0x08] = &VMEngine::i_iconst_5;
    _instructionFunctions[0x09] = &VMEngine::i_lconst_0;
    _instructionFunctions[0x0a] = &VMEngine::i_lconst_1;
    _instructionFunctions[0x0b] = &VMEngine::i_fconst_0;
    _instructionFunctions[0x0c] = &VMEngine::i_fconst_1;
    _instructionFunctions[0x0d] = &VMEngine::i_fconst_2;
    _instructionFunctions[0x0e] = &VMEngine::i_dconst_0;
    _instructionFunctions[0x0f] = &VMEngine::i_dconst_1;
    _instructionFunctions[0x10] = &VMEngine::i_bipush;
    _instructionFunctions[0x11] = &VMEngine::i_sipush;
    _instructionFunctions[0x12] = &VMEngine::i_ldc;
    _instructionFunctions[0x13] = &VMEngine::i_ldc_w;
    _instructionFunctions[0x14] = &VMEngine::i_ldc2_w;
    _instructionFunctions[0x15] = &VMEngine::i_iload;
    _instructionFunctions[0x16] = &VMEngine::i_lload;
    _instructionFunctions[0x17] = &VMEngine::i_fload;
    _instructionFunctions[0x18] = &VMEngine::i_dload;
    _instructionFunctions[0x19] = &VMEngine::i_aload;
    _instructionFunctions[0x1a] = &VMEngine::i_iload_0;
    _instructionFunctions[0x1b] = &VMEngine::i_iload_1;
    _instructionFunctions[0x1c] = &VMEngine::i_iload_2;
    _instructionFunctions[0x1d] = &VMEngine::i_iload_3;
    _instructionFunctions[0x1e] = &VMEngine::i_lload_0;
    _instructionFunctions[0x1f] = &VMEngine::i_lload_1;
    _instructionFunctions[0x20] = &VMEngine::i_lload_2;
    _instructionFunctions[0x21] = &VMEngine::i_lload_3;
    _instructionFunctions[0x22] = &VMEngine::i_fload_0;
    _instructionFunctions[0x23] = &VMEngine::i_fload_1;
    _instructionFunctions[0x24] = &VMEngine::i_fload_2;
    _instructionFunctions[0x25] = &VMEngine::i_fload_3;
    _instructionFunctions[0x26] = &VMEngine::i_dload_0;
    _instructionFunctions[0x27] = &VMEngine::i_dload_1;
    _instructionFunctions[0x28] = &VMEngine::i_dload_2;
    _instructionFunctions[0x29] = &VMEngine::i_dload_3;
    _instructionFunctions[0x2a] = &VMEngine::i_aload_0;
    _instructionFunctions[0x2b] = &VMEngine::i_aload_1;
    _instructionFunctions[0x2c] = &VMEngine::i_aload_2;
    _instructionFunctions[0x2d] = &VMEngine::i_aload_3;
    _instructionFunctions[0x2e] = &VMEngine::i_iaload;
    _instructionFunctions[0x2f] = &VMEngine::i_laload;
    _instructionFunctions[0x30] = &VMEngine::i_faload;
    _instructionFunctions[0x31] = &VMEngine::i_daload;
    _instructionFunctions[0x32] = &VMEngine::i_aaload;
    _instructionFunctions[0x33] = &VMEngine::i_baload;
    _instructionFunctions[0x34] = &VMEngine::i_caload;
    _instructionFunctions[0x35] = &VMEngine::i_saload;
    _instructionFunctions[0x36] = &VMEngine::i_istore;
    _instructionFunctions[0x37] = &VMEngine::i_lstore;
    _instructionFunctions[0x38] = &VMEngine::i_fstore;
    _instructionFunctions[0x39] = &VMEngine::i_dstore;
    _instructionFunctions[0x3a] = &VMEngine::i_astore;
    _instructionFunctions[0x3b] = &VMEngine::i_istore_0;
    _instructionFunctions[0x3c] = &VMEngine::i_istore_1;
    _instructionFunctions[0x3d] = &VMEngine::i_istore_2;
    _instructionFunctions[0x3e] = &VMEngine::i_istore_3;
    _instructionFunctions[0x3f] = &VMEngine::i_lstore_0;
    _instructionFunctions[0x40] = &VMEngine::i_lstore_1;
    _instructionFunctions[0x41] = &VMEngine::i_lstore_2;
    _instructionFunctions[0x42] = &VMEngine::i_lstore_3;
    _instructionFunctions[0x43] = &VMEngine::i_fstore_0;
    _instructionFunctions[0x44] = &VMEngine::i_fstore_1;
    _instructionFunctions[0x45] = &VMEngine::i_fstore_2;
    _instructionFunctions[0x46] = &VMEngine::i_fstore_3;
    _instructionFunctions[0x47] = &VMEngine::i_dstore_0;
    _instructionFunctions[0x48] = &VMEngine::i_dstore_1;
    _instructionFunctions[0x49] = &VMEngine::i_dstore_2;
    _instructionFunctions[0x4a] = &VMEngine::i_dstore_3;
    _instructionFunctions[0x4b] = &VMEngine::i_astore_0;
    _instructionFunctions[0x4c] = &VMEngine::i_astore_1;
    _instructionFunctions[0x4d] = &VMEngine::i_astore_2;
    _instructionFunctions[0x4e] = &VMEngine::i_astore_3;
    _instructionFunctions[0x4f] = &VMEngine::i_iastore;
    _instructionFunctions[0x50] = &VMEngine::i_lastore;
    _instructionFunctions[0x51] = &VMEngine::i_fastore;
    _instructionFunctions[0x52] = &VMEngine::i_dastore;
    _instructionFunctions[0x53] = &VMEngine::i_aastore;
    _instructionFunctions[0x54] = &VMEngine::i_bastore;
    _instructionFunctions[0x55] = &VMEngine::i_castore;
    _instructionFunctions[0x56] = &VMEngine::i_sastore;
    _instructionFunctions[0x57] = &VMEngine::i_pop;
    _instructionFunctions[0x58] = &VMEngine::i_pop2;
    _instructionFunctions[0x59] = &VMEngine::i_dup;
    _instructionFunctions[0x5a] = &VMEngine::i_dup2_x1;
    _instructionFunctions[0x5b] = &VMEngine::i_dup2_x2;
    _instructionFunctions[0x5c] = &VMEngine::i_dup2;
    _instructionFunctions[0x5d] = &VMEngine::i_dup2_x1;
    _instructionFunctions[0x5e] = &VMEngine::i_dup2_x2;
    _instructionFunctions[0x5f] = &VMEngine::i_swap;
    _instructionFunctions[0x60] = &VMEngine::i_iadd;
    _instructionFunctions[0x61] = &VMEngine::i_ladd;
    _instructionFunctions[0x62] = &VMEngine::i_fadd;
    _instructionFunctions[0x63] = &VMEngine::i_dadd;
    _instructionFunctions[0x64] = &VMEngine::i_isub;
    _instructionFunctions[0x65] = &VMEngine::i_lsub;
    _instructionFunctions[0x66] = &VMEngine::i_fsub;
    _instructionFunctions[0x67] = &VMEngine::i_dsub;
    _instructionFunctions[0x68] = &VMEngine::i_imul;
    _instructionFunctions[0x69] = &VMEngine::i_lmul;
    _instructionFunctions[0x6a] = &VMEngine::i_fmul;
    _instructionFunctions[0x6b] = &VMEngine::i_dmul;
    _instructionFunctions[0x6c] = &VMEngine::i_idiv;
    _instructionFunctions[0x6d] = &VMEngine::i_ldiv;
    _instructionFunctions[0x6e] = &VMEngine::i_fdiv;
    _instructionFunctions[0x6f] = &VMEngine::i_ddiv;
    _instructionFunctions[0x70] = &VMEngine::i_irem;
    _instructionFunctions[0x71] = &VMEngine::i_lrem;
    _instructionFunctions[0x72] = &VMEngine::i_frem;
    _instructionFunctions[0x73] = &VMEngine::i_drem;
    _instructionFunctions[0x74] = &VMEngine::i_ineg;
    _instructionFunctions[0x75] = &VMEngine::i_lneg;
    _instructionFunctions[0x76] = &VMEngine::i_fneg;
    _instructionFunctions[0x77] = &VMEngine::i_dneg;
    _instructionFunctions[0x78] = &VMEngine::i_ishl;
    _instructionFunctions[0x79] = &VMEngine::i_lshl;
    _instructionFunctions[0x7a] = &VMEngine::i_ishr;
    _instructionFunctions[0x7b] = &VMEngine::i_lshr;
    _instructionFunctions[0x7c] = &VMEngine::i_iushr;
    _instructionFunctions[0x7d] = &VMEngine::i_lushr;
    _instructionFunctions[0x7e] = &VMEngine::i_iand;
    _instructionFunctions[0x7f] = &VMEngine::i_land;
    _instructionFunctions[0x80] = &VMEngine::i_ior;
    _instructionFunctions[0x81] = &VMEngine::i_lor;
    _instructionFunctions[0x82] = &VMEngine::i_ixor;
    _instructionFunctions[0x83] = &VMEngine::i_lxor;
    _instructionFunctions[0x84] = &VMEngine::i_iinc;
    _instructionFunctions[0x85] = &VMEngine::i_i2l;
    _instructionFunctions[0x86] = &VMEngine::i_i2f;
    _instructionFunctions[0x87] = &VMEngine::i_i2d;
    _instructionFunctions[0x88] = &VMEngine::i_l2i;
    _instructionFunctions[0x89] = &VMEngine::i_l2f;
    _instructionFunctions[0x8a] = &VMEngine::i_l2d;
    _instructionFunctions[0x8b] = &VMEngine::i_f2i;
    _instructionFunctions[0x8c] = &VMEngine::i_f2l;
    _instructionFunctions[0x8d] = &VMEngine::i_f2d;
    _instructionFunctions[0x8e] = &VMEngine::i_d2i;
    _instructionFunctions[0x8f] = &VMEngine::i_d2l;
    _instructionFunctions[0x90] = &VMEngine::i_d2f;
    _instructionFunctions[0x91] = &VMEngine::i_i2b;
    _instructionFunctions[0x92] = &VMEngine::i_i2c;
    _instructionFunctions[0x93] = &VMEngine::i_i2s;
    _instructionFunctions[0x94] = &VMEngine::i_lcmp;
    _instructionFunctions[0x95] = &VMEngine::i_fcmpl;
    _instructionFunctions[0x96] = &VMEngine::i_fcmpg;
    _instructionFunctions[0x97] = &VMEngine::i_dcmpl;
    _instructionFunctions[0x98] = &VMEngine::i_dcmpg;
    _instructionFunctions[0x99] = &VMEngine::i_ifeq;
    _instructionFunctions[0x9a] = &VMEngine::i_ifne;
    _instructionFunctions[0x9b] = &VMEngine::i_iflt;
    _instructionFunctions[0x9c] = &VMEngine::i_ifge;
    _instructionFunctions[0x9d] = &VMEngine::i_ifgt;
    _instructionFunctions[0x9e] = &VMEngine::i_ifle;
    _instructionFunctions[0x9f] = &VMEngine::i_if_icmpeq;
    _instructionFunctions[0xa0] = &VMEngine::i_if_icmpne;
    _instructionFunctions[0xa1] = &VMEngine::i_if_icmplt;
    _instructionFunctions[0xa2] = &VMEngine::i_if_icmpge;
    _instructionFunctions[0xa3] = &VMEngine::i_if_icmpgt;
    _instructionFunctions[0xa4] = &VMEngine::i_if_icmple;
    _instructionFunctions[0xa5] = &VMEngine::i_if_acmpeq;
    _instructionFunctions[0xa6] = &VMEngine::i_if_acmpne;
    _instructionFunctions[0xa7] = &VMEngine::i_goto;
    _instructionFunctions[0xa8] = &VMEngine::i_jsr;
    _instructionFunctions[0xa9] = &VMEngine::i_ret;
    _instructionFunctions[0xaa] = &VMEngine::i_tableswitch;
    _instructionFunctions[0xab] = &VMEngine::i_lookupswitch;
    _instructionFunctions[0xac] = &VMEngine::i_ireturn;
    _instructionFunctions[0xad] = &VMEngine::i_lreturn;
    _instructionFunctions[0xae] = &VMEngine::i_freturn;
    _instructionFunctions[0xaf] = &VMEngine::i_dreturn;
    _instructionFunctions[0xb0] = &VMEngine::i_areturn;
    _instructionFunctions[0xb1] = &VMEngine::i_return;
    _instructionFunctions[0xb2] = &VMEngine::i_getstatic;
    _instructionFunctions[0xb3] = &VMEngine::i_putstatic;
    _instructionFunctions[0xb4] = &VMEngine::i_getfield;
    _instructionFunctions[0xb5] = &VMEngine::i_putfield;
    _instructionFunctions[0xb6] = &VMEngine::i_invokevirtual;
    _instructionFunctions[0xb7] = &VMEngine::i_invokespecial;
    _instructionFunctions[0xb8] = &VMEngine::i_invokestatic;
    _instructionFunctions[0xb9] = &VMEngine::i_invokeinterface;
    _instructionFunctions[0xbb] = &VMEngine::i_new;
    _instructionFunctions[0xbc] = &VMEngine::i_newarray;
    _instructionFunctions[0xbd] = &VMEngine::i_anewarray;
    _instructionFunctions[0xbe] = &VMEngine::i_arraylength;
    _instructionFunctions[0xbf] = &VMEngine::i_athrow;
    _instructionFunctions[0xc0] = &VMEngine::i_checkcast;
    _instructionFunctions[0xc1] = &VMEngine::i_instanceof;
    _instructionFunctions[0xc2] = &VMEngine::i_monitorenter;
    _instructionFunctions[0xc3] = &VMEngine::i_monitorexit;
    _instructionFunctions[0xc4] = &VMEngine::i_wide;
    _instructionFunctions[0xc5] = &VMEngine::i_multianewarray;
    _instructionFunctions[0xc6] = &VMEngine::i_ifnull;
    _instructionFunctions[0xc7] = &VMEngine::i_ifnonnull;
    _instructionFunctions[0xc8] = &VMEngine::i_goto_w;
    _instructionFunctions[0xc9] = &VMEngine::i_jsr_w;
}
