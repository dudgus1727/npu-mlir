# ------------------------------------------------------------------------------
# Helper functions macro and variables.
# ------------------------------------------------------------------------------
function(copy_file_to_build_dir src_dir output_dir file)
  set(src ${src_dir}/${file})
  set(dst ${output_dir}/${file})
  add_custom_command(OUTPUT ${dst}
    DEPENDS ${src}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src} ${dst}
    COMMENT "Copying RISCV Target - ${file}...")
endfunction(copy_file_to_build_dir)

# The macro helps to find the correct path of "IntrinsicsRISCV.h"
macro(npu_add_llvm_target target_name)
  include_directories(BEFORE
    ${npu_BUILD_DIR}/backend/include/
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    )
  add_llvm_component_library(LLVM${target_name} ${ARGN})
  set( CURRENT_LLVM_TARGET LLVM${target_name} )
endmacro(npu_add_llvm_target)

set(LLVM_TARGET_RISCV_DIR ${LLVM_MAIN_SRC_DIR}/lib/Target/RISCV)

# ------------------------------------------------------------------------------
# Configure RISC-V npu Extension.
# ------------------------------------------------------------------------------

add_llvm_component_group(RISCVnpuExt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${npu_SOURCE_DIR}/backend/include/)
include_directories(${LLVM_TARGET_RISCV_DIR})

set(LLVM_TARGET_DEFINITIONS RISCVnpuExt.td)

tablegen(LLVM RISCVGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM RISCVGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM RISCVGenCompressInstEmitter.inc -gen-compress-inst-emitter)
tablegen(LLVM RISCVGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM RISCVGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM RISCVGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM RISCVGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM RISCVGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM RISCVGenRegisterBank.inc -gen-register-bank)
tablegen(LLVM RISCVGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM RISCVGenSearchableTables.inc -gen-searchable-tables)
tablegen(LLVM RISCVGenSubtargetInfo.inc -gen-subtarget)

set(LLVM_TARGET_DEFINITIONS ${LLVM_TARGET_RISCV_DIR}/RISCVGISel.td)
tablegen(LLVM RISCVGenGlobalISel.inc -gen-global-isel)
tablegen(LLVM RISCVGenO0PreLegalizeGICombiner.inc -gen-global-isel-combiner
              -combiners="RISCVO0PreLegalizerCombiner")
tablegen(LLVM RISCVGenPreLegalizeGICombiner.inc -gen-global-isel-combiner
              -combiners="RISCVPreLegalizerCombiner")
tablegen(LLVM RISCVGenPostLegalizeGICombiner.inc -gen-global-isel-combiner
              -combiners="RISCVPostLegalizerCombiner")

add_public_tablegen_target(RISCVnpuCommonTableGen)

# ------------------------------------------------------------------------------
# Build RISC-V npu Extension CodeGen.
# ------------------------------------------------------------------------------

# Copy related files to build directory.
file(GLOB UPSTREAM_LLVM_TARGET_RISCV_FILE
  RELATIVE ${LLVM_TARGET_RISCV_DIR}
  "${LLVM_TARGET_RISCV_DIR}/*.cpp"
  "${LLVM_TARGET_RISCV_DIR}/*.h"
)
foreach(file ${UPSTREAM_LLVM_TARGET_RISCV_FILE})
  copy_file_to_build_dir(${LLVM_TARGET_RISCV_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${file})
endforeach()

# Copy related files to build directory.
file(GLOB UPSTREAM_LLVM_TARGET_RISCV_GISEL_FILE
  RELATIVE ${LLVM_TARGET_RISCV_DIR}/GISel/
  "${LLVM_TARGET_RISCV_DIR}/GISel/*.cpp"
  "${LLVM_TARGET_RISCV_DIR}/GISel/*.h"
)
foreach(file ${UPSTREAM_LLVM_TARGET_RISCV_GISEL_FILE})
  copy_file_to_build_dir(${LLVM_TARGET_RISCV_DIR}/GISel/ ${CMAKE_CURRENT_BINARY_DIR} ${file})
endforeach()

# Build npuRISCVCodeGen target.
npu_add_llvm_target(npuRISCVCodeGen
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVAsmPrinter.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVCodeGenPrepare.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVDeadRegisterDefinitions.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMakeCompressible.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVExpandAtomicPseudoInsts.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVExpandPseudoInsts.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVFoldMasks.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVFrameLowering.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVGatherScatterLowering.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInsertVSETVLI.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInsertReadWriteCSR.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInsertWriteVXRM.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInstrInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVISelDAGToDAG.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVISelLowering.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMachineFunctionInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMacroFusion.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMergeBaseOffset.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVOptWInstrs.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVPostRAExpandPseudoInsts.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVRedundantCopyElimination.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMoveMerger.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVPushPopOptimizer.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVRegisterInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVRVVInitUndef.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVSubtarget.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetMachine.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetObjectFile.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetTransformInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVCallLowering.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInstructionSelector.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVLegalizerInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVPostLegalizerCombiner.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVO0PreLegalizerCombiner.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVPreLegalizerCombiner.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVRegisterBankInfo.cpp
  # Add *.h files to track the copies above.
  # Target RISCV
  ${CMAKE_CURRENT_BINARY_DIR}/RISCV.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVFrameLowering.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInstrInfo.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVISelDAGToDAG.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVISelLowering.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMachineFunctionInfo.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMacroFusion.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVRegisterInfo.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVSubtarget.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetMachine.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetObjectFile.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetTransformInfo.h
  # GISel
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVCallLowering.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVLegalizerInfo.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVRegisterBankInfo.h

  LINK_COMPONENTS
  npuAnalysis
  npuAsmPrinter
  npuCodeGen
  CodeGenTypes
  npuCore
  npuIPO
  npuGlobalISel
  MC
  npuRISCVDesc
  npuRISCVInfo
  npuSelectionDAG
  Support
  npuTarget
  npuTransformUtils

  TargetParser

  ADD_TO_COMPONENT
  RISCVnpuExt

  )

# ------------------------------------------------------------------------------
# Build RISC-V npu Extension Asm Parser.
# ------------------------------------------------------------------------------

# Copy related files to build directory.
file(GLOB UPSTREAM_LLVM_TARGET_RISCV_ASMPARSER_FILE
  RELATIVE ${LLVM_TARGET_RISCV_DIR}/AsmParser/
  "${LLVM_TARGET_RISCV_DIR}/AsmParser/*.cpp"
)
foreach(file ${UPSTREAM_LLVM_TARGET_RISCV_ASMPARSER_FILE})
  copy_file_to_build_dir(${LLVM_TARGET_RISCV_DIR}/AsmParser ${CMAKE_CURRENT_BINARY_DIR} ${file})
endforeach()

# Build LLVMnpuRISCVAsmParser target.
add_llvm_component_library(LLVMnpuRISCVAsmParser
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVAsmParser.cpp
  LINK_COMPONENTS
  MC
  MCParser
  npuRISCVDesc
  npuRISCVInfo
  Support

  ADD_TO_COMPONENT
  RISCVnpuExt
  )

# ------------------------------------------------------------------------------
# Build RISC-V npu Extension Desc.
# ------------------------------------------------------------------------------

# Copy related files to build directory.
file(GLOB UPSTREAM_LLVM_TARGET_RISCV_DESC_FILE
  RELATIVE ${LLVM_TARGET_RISCV_DIR}/MCTargetDesc/
  "${LLVM_TARGET_RISCV_DIR}/MCTargetDesc/*.cpp"
  "${LLVM_TARGET_RISCV_DIR}/MCTargetDesc/*.h"
)
foreach(file ${UPSTREAM_LLVM_TARGET_RISCV_DESC_FILE})
  copy_file_to_build_dir(${LLVM_TARGET_RISCV_DIR}/MCTargetDesc ${CMAKE_CURRENT_BINARY_DIR} ${file})
endforeach()

# Build LLVMnpuRISCVDesc target.
add_llvm_component_library(LLVMnpuRISCVDesc
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVAsmBackend.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVBaseInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVELFObjectWriter.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInstPrinter.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCAsmInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCCodeEmitter.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCExpr.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCObjectFileInfo.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCTargetDesc.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMatInt.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetStreamer.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVELFStreamer.cpp

  # Add *.h files to track the copies above.
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVAsmBackend.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVBaseInfo.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVELFStreamer.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVFixupKinds.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVInstPrinter.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMatInt.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCAsmInfo.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCExpr.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCObjectFileInfo.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVMCTargetDesc.h
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetStreamer.h

  LINK_COMPONENTS
  MC
  npuRISCVInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  RISCVnpuExt
)

# ------------------------------------------------------------------------------
# Build RISC-V npu Extension Info.
# ------------------------------------------------------------------------------

# Copy related files to build directory.
file(GLOB UPSTREAM_LLVM_TARGET_RISCV_TARGETINFO_FILE
  RELATIVE ${LLVM_TARGET_RISCV_DIR}/TargetInfo/
  "${LLVM_TARGET_RISCV_DIR}/TargetInfo/*.cpp"
  "${LLVM_TARGET_RISCV_DIR}/TargetInfo/*.h"
)
foreach(file ${UPSTREAM_LLVM_TARGET_RISCV_TARGETINFO_FILE})
  copy_file_to_build_dir(${LLVM_TARGET_RISCV_DIR}/TargetInfo ${CMAKE_CURRENT_BINARY_DIR} ${file})
endforeach()

# Build LLVMnpuRISCVInfo target.
add_llvm_component_library(LLVMnpuRISCVInfo
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetInfo.cpp
  # Add *.h files to track the copies above.
  ${CMAKE_CURRENT_BINARY_DIR}/RISCVTargetInfo.h

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  RISCVnpuExt
  )
