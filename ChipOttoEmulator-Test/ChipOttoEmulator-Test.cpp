#define CLOVE_SUITE_NAME MySuite01
#include "clove-unit.h"
#include <stdlib.h>
#include <memory>
#include "emulatorTest.h"

CLOVE_SUITE_SETUP_ONCE()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}
}
CLOVE_TEST(TestOpcode0) {

	chipotto::EmulatorTest TestEmulator;
	uint8_t* pixels = nullptr;
	int pitch;

	TestEmulator.OpcodeD(0xD001);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode0(0x00E0);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
	SDL_LockTexture(TestEmulator.GetTexture(), nullptr, reinterpret_cast<void**>(&pixels), &pitch);
	uint8_t* ret = static_cast<uint8_t*>(calloc(32 * pitch, sizeof(uint8_t)));
	CLOVE_NOT_NULL(ret);

	CLOVE_INT_EQ(0, memcmp(ret, pixels, 32 * pitch * sizeof(uint8_t)));
	free(ret);

}

CLOVE_TEST(TestOpcode0NoCls) {

	chipotto::EmulatorTest TestEmulator;
	uint8_t* pixels = nullptr;
	int pitch;

	TestEmulator.OpcodeD(0xD001);
	SDL_LockTexture(TestEmulator.GetTexture(), nullptr, reinterpret_cast<void**>(&pixels), &pitch);
	uint8_t* ret = static_cast<uint8_t*>(calloc(32 * pitch, sizeof(uint8_t)));
	CLOVE_NOT_NULL(ret);

	CLOVE_INT_NE(0, memcmp(ret, pixels, 32 * pitch * sizeof(uint8_t)));
	free(ret);

}

CLOVE_TEST(TestOpcode01) {
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.Opcode1(0x002);
	uint16_t CurrentPC = TestEmulator.GetPC();
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0, CurrentPC)


	Status = TestEmulator.Opcode1(0x1010);
	CurrentPC = TestEmulator.GetPC();
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(14, CurrentPC)
}

CLOVE_TEST(TestOpcode02) {
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.Opcode2(0x2010);
	CLOVE_INT_EQ(0x200, TestEmulator.GetStack())
	CLOVE_INT_EQ(0x010, TestEmulator.GetPC())
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::NotIncrementPC, (int)Status)
	
}

CLOVE_TEST(TestOpcode02Overflow) {
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status;
	for (size_t i = 0; i < 17; i++)
	{
		Status = TestEmulator.Opcode2(0x2010);
	}
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::StackOverflow, (int)Status)
}

CLOVE_TEST(TestOpcode03)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6010);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode3(0x3010);
	CLOVE_INT_EQ(0x202, TestEmulator.GetPC())
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)

}

CLOVE_TEST(TestOpcode04EqualValues)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6010);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode4(0x3010);
	CLOVE_INT_EQ(0x200, TestEmulator.GetPC())
	Status = TestEmulator.Opcode4(0x3011);
	CLOVE_INT_EQ(0x202, TestEmulator.GetPC())
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)

}

CLOVE_TEST(TestOpcode04NotEqualValues)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6010);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode4(0x3011);
	CLOVE_INT_EQ(0x202, TestEmulator.GetPC())
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)

}

CLOVE_TEST(TestOpcode05)
{
	CLOVE_FAIL();
}

CLOVE_TEST(TestOpcode06)
{
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.Opcode6(0x6010);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x10, TestEmulator.GetRegisterAt(0))
}

CLOVE_TEST(TestOpcode07)
{
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.Opcode7(0x6010);
	Status = TestEmulator.Opcode7(0x6010);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x20, TestEmulator.GetRegisterAt(0))
}

CLOVE_TEST(TestOpcode08LD)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6023);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode8(0x8010);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(TestEmulator.GetRegisterAt(0), TestEmulator.GetRegisterAt(1))
}

CLOVE_TEST(TestOpcode08OR)
{
	CLOVE_FAIL()
}

CLOVE_TEST(TestOpcode08AND)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6023);
	TestEmulator.Opcode6(0x6131);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode8(0x8012);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x23 & 0x31, TestEmulator.GetRegisterAt(0))

}

CLOVE_TEST(TestOpcode08XOR)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6023);
	TestEmulator.Opcode6(0x6131);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode8(0x8013);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x23 ^ 0x31, TestEmulator.GetRegisterAt(0))
}

CLOVE_TEST(TestOpcode08ADDNoCarry)
{
	chipotto::EmulatorTest TestEmulator;

	TestEmulator.Opcode6(0x6001);
	TestEmulator.Opcode6(0x6101);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode8(0x8014);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x02, (int)TestEmulator.GetRegisterAt(0))
	CLOVE_INT_EQ(0x00, (int)TestEmulator.GetRegisterAt(0xF))
}

CLOVE_TEST(TestOpcode08ADDCarry)
{
	chipotto::EmulatorTest TestEmulator;

	TestEmulator.Opcode6(0x60FE);
	TestEmulator.Opcode6(0x6102);
	chipotto::OpcodeStatus Status = TestEmulator.Opcode8(0x8014);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x01, (int)TestEmulator.GetRegisterAt(0xF))

}

CLOVE_TEST(TestOpcode08SUB)
{
	CLOVE_FAIL()
}


CLOVE_TEST(TestOpcode08SHR)
{
	CLOVE_FAIL()
}

CLOVE_TEST(TestOpcode08SUBN)
{
	CLOVE_FAIL()
}

CLOVE_TEST(TestOpcode08SHL)
{
	chipotto::EmulatorTest TestEmulator;

	TestEmulator.Opcode6(0x6001);

	chipotto::OpcodeStatus Status = TestEmulator.Opcode8(0x800E);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x00, TestEmulator.GetRegisterAt(0xF))
	CLOVE_INT_EQ(0x02, TestEmulator.GetRegisterAt(0))
}

CLOVE_TEST(TestOpcode08SHL2)
{
	chipotto::EmulatorTest TestEmulator;

	TestEmulator.Opcode6(0x60F2);

	chipotto::OpcodeStatus Status = TestEmulator.Opcode8(0x800E);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status)
	CLOVE_INT_EQ(0x01, TestEmulator.GetRegisterAt(0xF))
	CLOVE_INT_EQ(0xE4, TestEmulator.GetRegisterAt(0))
}

CLOVE_TEST(TestOpcode09)
{
	CLOVE_FAIL()
}

CLOVE_TEST(TestOpcodeA)
{
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeA(0xA540);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
	CLOVE_INT_EQ(0x540, TestEmulator.GetI());
}

CLOVE_TEST(TestOpcodeB)
{
	CLOVE_FAIL()
}

CLOVE_TEST(TestOpcodeC)
{
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeC(0xC001);
	uint8_t rand = std::rand() % 256;
	uint8_t random_mask = 0xC001 & 0xFF;
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
	CLOVE_INT_EQ(rand & random_mask, TestEmulator.GetRegisterAt(0));
}

CLOVE_TEST(TestOpcodeD)
{
	chipotto::EmulatorTest TestEmulator;

	chipotto::OpcodeStatus Status = TestEmulator.OpcodeD(0xD005);
	uint8_t TestPixel[] = {1,1,1,1,1};

	uint8_t* Pixels = nullptr;
	int Pitch;
	int Result = SDL_LockTexture(TestEmulator.GetTexture(), nullptr, reinterpret_cast<void**>(&Pixels), &Pitch);

	int MemcmpResult = std::memcmp(TestPixel, Pixels,5);
	CLOVE_INT_EQ(0, MemcmpResult);


}

CLOVE_TEST(TestOpcodeF_LD_Vx_DT)
{
	chipotto::EmulatorTest TestEmulator;

	chipotto::OpcodeStatus Status = TestEmulator.OpcodeD(0xF107);
	uint8_t DelayTimer = TestEmulator.GetDelayTimer();
	CLOVE_INT_EQ(DelayTimer, TestEmulator.GetRegisterAt(1));
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
}

CLOVE_TEST(TestOpcodeF_LD_Vx_K)
{
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF20A);
	uint8_t DelayTimer = TestEmulator.GetDelayTimer();
	CLOVE_INT_EQ(DelayTimer, TestEmulator.GetRegisterAt(2));
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::WaitForKeyboard, (int)Status);
}

CLOVE_TEST(TestOpcodeF_LD_DT_Vx)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6002);
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF015);
	uint8_t DelayTimer = TestEmulator.GetDelayTimer();
	CLOVE_INT_EQ((int)TestEmulator.GetDelayTimer(), 0x02);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
}

CLOVE_TEST(TestOpcodeF_LD_ST_Vx)
{
	chipotto::EmulatorTest TestEmulator;
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF018);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
}

CLOVE_TEST(TestOpcodeF_ADD_I_Vx)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6003);
	TestEmulator.OpcodeA(0xA005);
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF01E);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
	CLOVE_INT_EQ(0x08, TestEmulator.GetI());
}

CLOVE_TEST(TestOpcodeF_LD_F_Vx)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6005);
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF029);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
	CLOVE_INT_EQ(5 * 0x05, TestEmulator.GetI());
}

CLOVE_TEST(TestOpcodeF_LD_B_Vx)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6050);
	TestEmulator.OpcodeA(0xA005);
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF033);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
	uint8_t MemoryMappingAtI = TestEmulator.GetMemoryMappingAt(TestEmulator.GetI());
	CLOVE_INT_EQ((0x50 / 100), MemoryMappingAtI);

	CLOVE_INT_EQ(0x50 - (MemoryMappingAtI * 100) / 10, TestEmulator.GetMemoryMappingAt(TestEmulator.GetI() + 1));

	CLOVE_INT_EQ(0x50 % 10, TestEmulator.GetMemoryMappingAt(TestEmulator.GetI() + 2));
}

CLOVE_TEST(TestOpcodeF_LD_I_Vx)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.Opcode6(0x6001);
	TestEmulator.Opcode6(0x6104);
	TestEmulator.Opcode6(0x6205);
	TestEmulator.Opcode6(0x6310);
	TestEmulator.OpcodeA(0xA191);
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF355);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);

	uint16_t i = TestEmulator.GetI();
	CLOVE_INT_EQ(0x01, TestEmulator.GetMemoryMappingAt(i));
	CLOVE_INT_EQ(0x04, TestEmulator.GetMemoryMappingAt(i + 1));
	CLOVE_INT_EQ(0x05, TestEmulator.GetMemoryMappingAt(i + 2));
	CLOVE_INT_EQ(0x10, TestEmulator.GetMemoryMappingAt(i + 3));
}

CLOVE_TEST(TestOpcodeF_LD_Vx_I)
{
	chipotto::EmulatorTest TestEmulator;
	TestEmulator.OpcodeA(0xA100);
	TestEmulator.Opcode6(0x6002);
	TestEmulator.Opcode6(0x6110);
	TestEmulator.Opcode6(0x6240);
	//Mapping
	TestEmulator.OpcodeF(0xF255);
	TestEmulator.CleanRegisters();
	//register
	chipotto::OpcodeStatus Status = TestEmulator.OpcodeF(0xF265);
	CLOVE_INT_EQ((int)chipotto::OpcodeStatus::IncrementPC, (int)Status);
	
	CLOVE_INT_EQ(0x02, TestEmulator.GetRegisterAt(0));
	CLOVE_INT_EQ(0x10, TestEmulator.GetRegisterAt(1));
	CLOVE_INT_EQ(0x40, TestEmulator.GetRegisterAt(2));
}

CLOVE_SUITE_TEARDOWN_ONCE()
{
	SDL_Quit();
}