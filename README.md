# RISC-V-Simulator

## Overview

RISC-V (pronounced "risk-five") is an open standard instruction set architecture (ISA) based on established reduced instruction set computer (RISC) principles.<br>
This is a 32-bit single core based implementation of RISC-V ISS feature specifications and covers all permitted configurations.

## How to build

#### Build requirements

`C++` compiler used in this project is, GNU G++14.

#### Build

```shell
git     clone   https://github.com/ar-pavel/RISC-V-Simulator.git
cd  RISC-V-Simulator
./make
```

## Supported Operations

Operation templates that are supported by this simulator:

| Usage Template &emsp; &emsp; &emsp; &emsp; &emsp; | Type | Description &emsp;                  | Detailed Description &emsp; &emsp; &emsp; &emsp; &emsp; |
| ------------------------------------------------- | :--: | ----------------------------------- | ------------------------------------------------------- |
| add &emsp; &ensp; rd, rs1, rs2                    |  R   | Add                                 | rd ← rs1 + rs2, pc ← pc+4                               |
| addi &emsp; &nbsp; rd, rs1, imm                   |  I   | Add Immediate                       | rd ← rs1 + imm i, pc ← pc+4                             |
| and &emsp; &ensp; rd, rs1, rs2                    |  R   | AND                                 | rd ← rs1 & rs2, pc ← pc+4                               |
| andi &emsp; &nbsp; rd, rs1, imm                   |  I   | AND Immediate                       | rd ← rs1 & imm i, pc ← pc+4                             |
| auipc &nbsp; &ensp; rd, imm                       |  U   | Add Upper Immediate to PC           | rd ← pc + imm u, pc ← pc+4                              |
| beq &emsp; &ensp; rs1, rs2, pcrel_13              |  B   | Branch Equal                        | pc ← pc + ((rs1==rs2) ? imm b : 4)                      |
| bge &emsp; &ensp; rs1, rs2, pcrel_13              |  B   | Branch Greater or Equal             | pc ← pc + ((rs1>=rs2) ? imm b : 4)                      |
| bgeu &ensp; &nbsp; rs1, rs2, pcrel_13             |  B   | Branch Greater or Equal Unsigned    | pc ← pc + ((rs1>=rs2) ? imm b : 4)                      |
| blt &emsp; &emsp; rs1, rs2, pcrel_13              |  B   | Branch Less Than                    | pc ← pc + ((rs1<rs2) ? imm b : 4)                       |
| bltu &emsp; &nbsp; rs1, rs2, pcrel_13             |  B   | Branch Less Than Unsigned           | pc ← pc + ((rs1<rs2) ? imm b : 4)                       |
| bne &emsp; &nbsp; rs1, rs2, pcrel_13              |  B   | Branch Not Equal                    | pc ← pc + ((rs1!=rs2) ? imm b : 4)                      |
| jal &emsp; &ensp; &nbsp; rd, pcrel_21             |  J   | Jump And Link                       | rd ← pc+4, pc ← pc+imm j                                |
| jalr &emsp; &nbsp; &nbsp; rd, imm(rs1)            |  I   | Jump And Link Register              | rd ← pc+4, pc ← (rs1+imm i)&~1                          |
| lb &emsp; &ensp; &ensp; rd, imm(rs1)              |  I   | Load Byte                           | rd ← sx(m8(rs1+imm i)), pc ← pc+4                       |
| lbu &emsp; &ensp; rd, imm(rs1)                    |  I   | Load Byte Unsigned                  | rd ← zx(m8(rs1+imm i)), pc ← pc+4                       |
| lh &emsp; &emsp; rd, imm(rs1)                     |  I   | Load Halfword                       | rd ← sx(m16(rs1+imm i)), pc ← pc+4                      |
| lhu &ensp; &emsp; rd, imm(rs1)                    |  I   | Load Halfword Unsigned              | rd ← zx(m16(rs1+imm i)), pc ← pc+4                      |
| lui &ensp; &emsp; &nbsp; rd, imm                  |  U   | Load Upper Immediate                | rd ← imm u, pc ← pc+4                                   |
| lw &emsp; &emsp; rd, imm(rs1)                     |  I   | Load Word                           | rd ← sx(m32(rs1+imm i)), pc ← pc+4                      |
| or &emsp; &emsp; rd, rs1, rs2                     |  R   | OR                                  | rd ← rs1 \| rs2, pc ← pc+4                              |
| ori &emsp; &emsp; rd, rs1, imm                    |  I   | OR Immediate                        | rd ← rs1 \| imm i, pc ← pc+4                            |
| sb &emsp; &emsp; rs2, imm(rs1)                    |  S   | Store Byte                          | m8(rs1+imm s) ← rs2[7:0], pc ← pc+4                     |
| sh &emsp; &emsp; rs2, imm(rs1)                    |  S   | Store Halfword                      | m16(rs1+imm s) ← rs2[15:0], pc ← pc+4                   |
| sll &emsp; &emsp; rd, rs1, rs2                    |  R   | Shift Left Logical                  | rd ← rs1 << (rs2%XLEN), pc ← pc+4                       |
| slli &emsp; &ensp; rd, rs1, shamt                 |  I   | Shift Left Logical Immediate        | rd ← rs1 << shamt i, pc ← pc+4                          |
| slt &ensp; &ensp; &ensp; rd, rs1, rs2             |  R   | Set Less Than                       | rd ← (rs1 < rs2) ? 1 : 0, pc ← pc+4                     |
| slti &emsp; &nbsp; rd, rs1, imm                   |  I   | Set Less Than Intermediate          | rd ← (rs1 < imm i) ? 1 : 0, pc ← pc+4                   |
| sltiu &nbsp; &ensp; rd, rs1, imm                  |  I   | Set Less Than Intermediate Unsigned | rd ← (rs1 < imm i) ? 1 : 0, pc ← pc+4                   |
| sltu &emsp; &nbsp; rd, rs1, rs2                   |  R   | Set less Than Unsigned              | rd ← (rs1 < rs2) ? 1 : 0, pc ← pc+4                     |
| sra &ensp; &ensp; &ensp; rd, rs1, rs2             |  R   | Shift Right Arithmetic              | rd ← rs1 >> (rs2%XLEN), pc ← pc+4                       |
| srai &emsp; &nbsp; rd, rs1, shamt                 |  I   | Shift Right Arithmetic Immediate    | rd ← rs1 >> shamt i, pc ← pc+4                          |
| srl &ensp; &ensp; &ensp; rd, rs1, rs2             |  R   | Shift Right Logical                 | rd ← rs1 >> (rs2%XLEN), pc ← pc+4                       |
| srli&emsp; &ensp; &nbsp; rd, rs1, shamt           |  I   | Shift Right Logical Immediate       | rd ← rs1 >> shamt i, pc ← pc+4                          |
| sub &ensp; &ensp; &ensp; rd, rs1, rs2             |  R   | Subtract                            | rd ← rs1 - rs2, pc ← pc+4                               |
| sw &emsp; &emsp; rs2, imm(rs1)                    |  S   | Store Word                          | m32(rs1+imm s) ← rs2[31:0], pc ← pc+4                   |
| xor &ensp; &ensp; &ensp; rd, rs1, rs2             |  R   | Exclusive OR                        | rd ← rs1 ^ rs2, pc ← pc+4                               |
| xori &emsp; &nbsp; rd, rs1, imm                   |  I   | Exclusive OR Immediate              | rd ← rs1 ^ imm i, pc ← pc+4                             |
