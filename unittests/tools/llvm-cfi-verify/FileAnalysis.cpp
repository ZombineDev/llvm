//===- llvm/unittests/tools/llvm-cfi-verify/FileAnalysis.cpp --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "../tools/llvm-cfi-verify/lib/FileAnalysis.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Object/Binary.h"
#include "llvm/Object/COFF.h"
#include "llvm/Object/ELFObjectFile.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdlib>

using Instr = ::llvm::cfi_verify::FileAnalysis::Instr;
using ::testing::Eq;
using ::testing::Field;

namespace llvm {
namespace cfi_verify {
namespace {
class ELFx86TestFileAnalysis : public FileAnalysis {
public:
  ELFx86TestFileAnalysis()
      : FileAnalysis(Triple("x86_64--"), SubtargetFeatures()) {}

  // Expose this method publicly for testing.
  void parseSectionContents(ArrayRef<uint8_t> SectionBytes,
                            uint64_t SectionAddress) {
    FileAnalysis::parseSectionContents(SectionBytes, SectionAddress);
  }

  Error initialiseDisassemblyMembers() {
    return FileAnalysis::initialiseDisassemblyMembers();
  }
};

class BasicFileAnalysisTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    if (Analysis.initialiseDisassemblyMembers()) {
      FAIL() << "Failed to initialise FileAnalysis.";
    }
  }

  ELFx86TestFileAnalysis Analysis;
};

TEST_F(BasicFileAnalysisTest, BasicDisassemblyTraversalTest) {
  Analysis.parseSectionContents(
      {
          0x90,                   // 0: nop
          0xb0, 0x00,             // 1: mov $0x0, %al
          0x48, 0x89, 0xe5,       // 3: mov %rsp, %rbp
          0x48, 0x83, 0xec, 0x18, // 6: sub $0x18, %rsp
          0x48, 0xbe, 0xc4, 0x07, 0x40,
          0x00, 0x00, 0x00, 0x00, 0x00, // 10: movabs $0x4007c4, %rsi
          0x2f,                         // 20: (bad)
          0x41, 0x0e,                   // 21: rex.B (bad)
          0x62, 0x72, 0x65, 0x61, 0x6b, // 23: (bad) {%k1}
      },
      0xDEADBEEF);

  EXPECT_EQ(nullptr, Analysis.getInstruction(0x0));
  EXPECT_EQ(nullptr, Analysis.getInstruction(0x1000));

  // 0xDEADBEEF: nop
  const auto *InstrMeta = Analysis.getInstruction(0xDEADBEEF);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(0xDEADBEEF, InstrMeta->VMAddress);
  EXPECT_EQ(1u, InstrMeta->InstructionSize);
  EXPECT_TRUE(InstrMeta->Valid);

  const auto *NextInstrMeta = Analysis.getNextInstructionSequential(*InstrMeta);
  EXPECT_EQ(nullptr, Analysis.getPrevInstructionSequential(*InstrMeta));
  const auto *PrevInstrMeta = InstrMeta;

  // 0xDEADBEEF + 1: mov $0x0, %al
  InstrMeta = Analysis.getInstruction(0xDEADBEEF + 1);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(NextInstrMeta, InstrMeta);
  EXPECT_EQ(0xDEADBEEF + 1, InstrMeta->VMAddress);
  EXPECT_EQ(2u, InstrMeta->InstructionSize);
  EXPECT_TRUE(InstrMeta->Valid);

  NextInstrMeta = Analysis.getNextInstructionSequential(*InstrMeta);
  EXPECT_EQ(PrevInstrMeta, Analysis.getPrevInstructionSequential(*InstrMeta));
  PrevInstrMeta = InstrMeta;

  // 0xDEADBEEF + 3: mov %rsp, %rbp
  InstrMeta = Analysis.getInstruction(0xDEADBEEF + 3);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(NextInstrMeta, InstrMeta);
  EXPECT_EQ(0xDEADBEEF + 3, InstrMeta->VMAddress);
  EXPECT_EQ(3u, InstrMeta->InstructionSize);
  EXPECT_TRUE(InstrMeta->Valid);

  NextInstrMeta = Analysis.getNextInstructionSequential(*InstrMeta);
  EXPECT_EQ(PrevInstrMeta, Analysis.getPrevInstructionSequential(*InstrMeta));
  PrevInstrMeta = InstrMeta;

  // 0xDEADBEEF + 6: sub $0x18, %rsp
  InstrMeta = Analysis.getInstruction(0xDEADBEEF + 6);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(NextInstrMeta, InstrMeta);
  EXPECT_EQ(0xDEADBEEF + 6, InstrMeta->VMAddress);
  EXPECT_EQ(4u, InstrMeta->InstructionSize);
  EXPECT_TRUE(InstrMeta->Valid);

  NextInstrMeta = Analysis.getNextInstructionSequential(*InstrMeta);
  EXPECT_EQ(PrevInstrMeta, Analysis.getPrevInstructionSequential(*InstrMeta));
  PrevInstrMeta = InstrMeta;

  // 0xDEADBEEF + 10: movabs $0x4007c4, %rsi
  InstrMeta = Analysis.getInstruction(0xDEADBEEF + 10);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(NextInstrMeta, InstrMeta);
  EXPECT_EQ(0xDEADBEEF + 10, InstrMeta->VMAddress);
  EXPECT_EQ(10u, InstrMeta->InstructionSize);
  EXPECT_TRUE(InstrMeta->Valid);

  EXPECT_EQ(nullptr, Analysis.getNextInstructionSequential(*InstrMeta));
  EXPECT_EQ(PrevInstrMeta, Analysis.getPrevInstructionSequential(*InstrMeta));
  PrevInstrMeta = InstrMeta;

  // 0xDEADBEEF + 20: (bad)
  InstrMeta = Analysis.getInstruction(0xDEADBEEF + 20);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(0xDEADBEEF + 20, InstrMeta->VMAddress);
  EXPECT_EQ(1u, InstrMeta->InstructionSize);
  EXPECT_FALSE(InstrMeta->Valid);

  EXPECT_EQ(nullptr, Analysis.getNextInstructionSequential(*InstrMeta));
  EXPECT_EQ(PrevInstrMeta, Analysis.getPrevInstructionSequential(*InstrMeta));

  // 0xDEADBEEF + 21: rex.B (bad)
  InstrMeta = Analysis.getInstruction(0xDEADBEEF + 21);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(0xDEADBEEF + 21, InstrMeta->VMAddress);
  EXPECT_EQ(2u, InstrMeta->InstructionSize);
  EXPECT_FALSE(InstrMeta->Valid);

  EXPECT_EQ(nullptr, Analysis.getNextInstructionSequential(*InstrMeta));
  EXPECT_EQ(nullptr, Analysis.getPrevInstructionSequential(*InstrMeta));

  // 0xDEADBEEF + 6: (bad) {%k1}
  InstrMeta = Analysis.getInstruction(0xDEADBEEF + 23);
  EXPECT_NE(nullptr, InstrMeta);
  EXPECT_EQ(0xDEADBEEF + 23, InstrMeta->VMAddress);
  EXPECT_EQ(5u, InstrMeta->InstructionSize);
  EXPECT_FALSE(InstrMeta->Valid);

  EXPECT_EQ(nullptr, Analysis.getNextInstructionSequential(*InstrMeta));
  EXPECT_EQ(nullptr, Analysis.getPrevInstructionSequential(*InstrMeta));
}

TEST_F(BasicFileAnalysisTest, PrevAndNextFromBadInst) {
  Analysis.parseSectionContents(
      {
          0x90, // 0: nop
          0x2f, // 1: (bad)
          0x90  // 2: nop
      },
      0xDEADBEEF);
  const auto &BadInstrMeta = Analysis.getInstructionOrDie(0xDEADBEEF + 1);
  const auto *GoodInstrMeta =
      Analysis.getPrevInstructionSequential(BadInstrMeta);
  EXPECT_NE(nullptr, GoodInstrMeta);
  EXPECT_EQ(0xDEADBEEF, GoodInstrMeta->VMAddress);
  EXPECT_EQ(1u, GoodInstrMeta->InstructionSize);

  GoodInstrMeta = Analysis.getNextInstructionSequential(BadInstrMeta);
  EXPECT_NE(nullptr, GoodInstrMeta);
  EXPECT_EQ(0xDEADBEEF + 2, GoodInstrMeta->VMAddress);
  EXPECT_EQ(1u, GoodInstrMeta->InstructionSize);
}

TEST_F(BasicFileAnalysisTest, CFITrapTest) {
  Analysis.parseSectionContents(
      {
          0x90,                   // 0: nop
          0xb0, 0x00,             // 1: mov $0x0, %al
          0x48, 0x89, 0xe5,       // 3: mov %rsp, %rbp
          0x48, 0x83, 0xec, 0x18, // 6: sub $0x18, %rsp
          0x48, 0xbe, 0xc4, 0x07, 0x40,
          0x00, 0x00, 0x00, 0x00, 0x00, // 10: movabs $0x4007c4, %rsi
          0x2f,                         // 20: (bad)
          0x41, 0x0e,                   // 21: rex.B (bad)
          0x62, 0x72, 0x65, 0x61, 0x6b, // 23: (bad) {%k1}
          0x0f, 0x0b                    // 28: ud2
      },
      0xDEADBEEF);

  EXPECT_FALSE(Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF)));
  EXPECT_FALSE(
      Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF + 3)));
  EXPECT_FALSE(
      Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF + 6)));
  EXPECT_FALSE(
      Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF + 10)));
  EXPECT_FALSE(
      Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF + 20)));
  EXPECT_FALSE(
      Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF + 21)));
  EXPECT_FALSE(
      Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF + 23)));
  EXPECT_TRUE(
      Analysis.isCFITrap(Analysis.getInstructionOrDie(0xDEADBEEF + 28)));
}

TEST_F(BasicFileAnalysisTest, FallThroughTest) {
  Analysis.parseSectionContents(
      {
          0x90,                         // 0: nop
          0xb0, 0x00,                   // 1: mov $0x0, %al
          0x2f,                         // 3: (bad)
          0x0f, 0x0b,                   // 4: ud2
          0xff, 0x20,                   // 6: jmpq *(%rax)
          0xeb, 0x00,                   // 8: jmp +0
          0xe8, 0x45, 0xfe, 0xff, 0xff, // 10: callq [some loc]
          0xff, 0x10,                   // 15: callq *(rax)
          0x75, 0x00,                   // 17: jne +0
          0xc3,                         // 19: retq
      },
      0xDEADBEEF);

  EXPECT_TRUE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF)));
  EXPECT_TRUE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 1)));
  EXPECT_FALSE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 3)));
  EXPECT_FALSE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 4)));
  EXPECT_FALSE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 6)));
  EXPECT_FALSE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 8)));
  EXPECT_FALSE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 10)));
  EXPECT_FALSE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 15)));
  EXPECT_TRUE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 17)));
  EXPECT_FALSE(
      Analysis.canFallThrough(Analysis.getInstructionOrDie(0xDEADBEEF + 19)));
}

TEST_F(BasicFileAnalysisTest, DefiniteNextInstructionTest) {
  Analysis.parseSectionContents(
      {
          0x90,                         // 0: nop
          0xb0, 0x00,                   // 1: mov $0x0, %al
          0x2f,                         // 3: (bad)
          0x0f, 0x0b,                   // 4: ud2
          0xff, 0x20,                   // 6: jmpq *(%rax)
          0xeb, 0x00,                   // 8: jmp 10 [+0]
          0xeb, 0x05,                   // 10: jmp 17 [+5]
          0xe8, 0x00, 0x00, 0x00, 0x00, // 12: callq 17 [+0]
          0xe8, 0x78, 0x56, 0x34, 0x12, // 17: callq 0x1234569f [+0x12345678]
          0xe8, 0x04, 0x00, 0x00, 0x00, // 22: callq 31 [+4]
          0xff, 0x10,                   // 27: callq *(rax)
          0x75, 0x00,                   // 29: jne 31 [+0]
          0x75, 0xe0,                   // 31: jne 1 [-32]
          0xc3,                         // 33: retq
          0xeb, 0xdd,                   // 34: jmp 1 [-35]
          0xeb, 0xdd,                   // 36: jmp 3 [-35]
          0xeb, 0xdc,                   // 38: jmp 4 [-36]
      },
      0xDEADBEEF);

  const auto *Current = Analysis.getInstruction(0xDEADBEEF);
  const auto *Next = Analysis.getDefiniteNextInstruction(*Current);
  EXPECT_NE(nullptr, Next);
  EXPECT_EQ(0xDEADBEEF + 1, Next->VMAddress);

  Current = Analysis.getInstruction(0xDEADBEEF + 1);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));

  Current = Analysis.getInstruction(0xDEADBEEF + 3);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));

  Current = Analysis.getInstruction(0xDEADBEEF + 4);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));

  Current = Analysis.getInstruction(0xDEADBEEF + 6);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));

  Current = Analysis.getInstruction(0xDEADBEEF + 8);
  Next = Analysis.getDefiniteNextInstruction(*Current);
  EXPECT_NE(nullptr, Next);
  EXPECT_EQ(0xDEADBEEF + 10, Next->VMAddress);

  Current = Analysis.getInstruction(0xDEADBEEF + 10);
  Next = Analysis.getDefiniteNextInstruction(*Current);
  EXPECT_NE(nullptr, Next);
  EXPECT_EQ(0xDEADBEEF + 17, Next->VMAddress);

  Current = Analysis.getInstruction(0xDEADBEEF + 12);
  Next = Analysis.getDefiniteNextInstruction(*Current);
  EXPECT_NE(nullptr, Next);
  EXPECT_EQ(0xDEADBEEF + 17, Next->VMAddress);

  Current = Analysis.getInstruction(0xDEADBEEF + 17);
  // Note, definite next instruction address is out of range and should fail.
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));
  Next = Analysis.getDefiniteNextInstruction(*Current);

  Current = Analysis.getInstruction(0xDEADBEEF + 22);
  Next = Analysis.getDefiniteNextInstruction(*Current);
  EXPECT_NE(nullptr, Next);
  EXPECT_EQ(0xDEADBEEF + 31, Next->VMAddress);

  Current = Analysis.getInstruction(0xDEADBEEF + 27);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));
  Current = Analysis.getInstruction(0xDEADBEEF + 29);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));
  Current = Analysis.getInstruction(0xDEADBEEF + 31);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));
  Current = Analysis.getInstruction(0xDEADBEEF + 33);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));

  Current = Analysis.getInstruction(0xDEADBEEF + 34);
  Next = Analysis.getDefiniteNextInstruction(*Current);
  EXPECT_NE(nullptr, Next);
  EXPECT_EQ(0xDEADBEEF + 1, Next->VMAddress);

  Current = Analysis.getInstruction(0xDEADBEEF + 36);
  EXPECT_EQ(nullptr, Analysis.getDefiniteNextInstruction(*Current));

  Current = Analysis.getInstruction(0xDEADBEEF + 38);
  Next = Analysis.getDefiniteNextInstruction(*Current);
  EXPECT_NE(nullptr, Next);
  EXPECT_EQ(0xDEADBEEF + 4, Next->VMAddress);
}

TEST_F(BasicFileAnalysisTest, ControlFlowXRefsTest) {
  Analysis.parseSectionContents(
      {
          0x90,                         // 0: nop
          0xb0, 0x00,                   // 1: mov $0x0, %al
          0x2f,                         // 3: (bad)
          0x0f, 0x0b,                   // 4: ud2
          0xff, 0x20,                   // 6: jmpq *(%rax)
          0xeb, 0x00,                   // 8: jmp 10 [+0]
          0xeb, 0x05,                   // 10: jmp 17 [+5]
          0xe8, 0x00, 0x00, 0x00, 0x00, // 12: callq 17 [+0]
          0xe8, 0x78, 0x56, 0x34, 0x12, // 17: callq 0x1234569f [+0x12345678]
          0xe8, 0x04, 0x00, 0x00, 0x00, // 22: callq 31 [+4]
          0xff, 0x10,                   // 27: callq *(rax)
          0x75, 0x00,                   // 29: jne 31 [+0]
          0x75, 0xe0,                   // 31: jne 1 [-32]
          0xc3,                         // 33: retq
          0xeb, 0xdd,                   // 34: jmp 1 [-35]
          0xeb, 0xdd,                   // 36: jmp 3 [-35]
          0xeb, 0xdc,                   // 38: jmp 4 [-36]
      },
      0xDEADBEEF);
  const auto *InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF);
  std::set<const Instr *> XRefs =
      Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(XRefs.empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 1);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_THAT(XRefs, UnorderedElementsAre(
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF)),
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 31)),
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 34))));

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 3);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_THAT(XRefs, UnorderedElementsAre(
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 1)),
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 36))));

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 4);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_THAT(XRefs, UnorderedElementsAre(
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 38))));

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 6);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 8);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 10);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_THAT(XRefs, UnorderedElementsAre(
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 8))));

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 12);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 17);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_THAT(XRefs, UnorderedElementsAre(
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 10)),
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 12))));

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 22);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 27);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 29);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 31);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_THAT(XRefs, UnorderedElementsAre(
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 22)),
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 29))));

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 33);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_THAT(XRefs, UnorderedElementsAre(
                         Field(&Instr::VMAddress, Eq(0xDEADBEEF + 31))));

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 34);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 36);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());

  InstrMetaPtr = &Analysis.getInstructionOrDie(0xDEADBEEF + 38);
  XRefs = Analysis.getDirectControlFlowXRefs(*InstrMetaPtr);
  EXPECT_TRUE(Analysis.getDirectControlFlowXRefs(*InstrMetaPtr).empty());
}

} // anonymous namespace
} // end namespace cfi_verify
} // end namespace llvm

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  llvm::cl::ParseCommandLineOptions(argc, argv);

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllDisassemblers();

  return RUN_ALL_TESTS();
}
