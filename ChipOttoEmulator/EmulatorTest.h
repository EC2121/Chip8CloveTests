#pragma once

#include "emulator.h"

namespace chipotto {

	class EmulatorTest : public Emulator
	{
	public:
		EmulatorTest();
		~EmulatorTest();
	public:
		uint16_t GetPC();
		uint16_t GetStack();
		uint8_t GetRegisterAt(uint8_t InIndex);
		uint16_t GetI();
		SDL_Texture* GetTexture();
		SDL_Scancode GetKeyboardValuesMapAt(int InIndex);
		uint8_t GetDelayTimer();
		uint8_t GetMemoryMappingAt(int inIndex);
		void CleanRegisters();
	private:

	};

	EmulatorTest::EmulatorTest()
	{
	}

	EmulatorTest::~EmulatorTest()
	{
	}

	inline void EmulatorTest::CleanRegisters()
	{
		for (size_t i = 0; i < Registers.size(); i++)
		{
			Registers[i] = 0;
		}
	}
	inline uint8_t EmulatorTest::GetMemoryMappingAt(int InIndex)
	{
		return MemoryMapping[InIndex];
	}
	inline uint8_t EmulatorTest::GetDelayTimer()
	{
		return DelayTimer;
	}
	inline uint16_t EmulatorTest::GetPC()
	{
		return Emulator::PC;
	}
	inline uint16_t EmulatorTest::GetStack()
	{
		return Emulator::Stack[Emulator::SP];
	}
	inline uint8_t EmulatorTest::GetRegisterAt(uint8_t InIndex)
	{
		return Emulator::Registers[InIndex];
	}
	inline uint16_t EmulatorTest::GetI()
	{
		return I;
	}
	inline SDL_Texture* EmulatorTest::GetTexture()
	{
		return Texture;
	}
	inline SDL_Scancode EmulatorTest::GetKeyboardValuesMapAt(int InIndex)
	{
		return KeyboardValuesMap[InIndex];
	}
}

