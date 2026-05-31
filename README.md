# Compiler Project

## Overview

This project is a self-directed implementation of a compiler written in **C++**, created as an exploration of compiler design, language implementation, intermediate representations, and backend code generation using **LLVM**.

The primary goal was not simply to build a working compiler, but to gain a deeper understanding of the complete compilation pipeline and the transformations required to convert high-level source code into executable machine instructions.

---

## Project Scope

The compiler evolved through several major stages of development:

* Lexical analysis and tokenization
* Parsing and grammar handling (CFG-based design)
* Abstract Syntax Tree (AST) construction
* Control Flow Graph (CFG) generation
* Intermediate Representation (IR) design and transformation
* LLVM backend experimentation
* Instruction-level debugging and validation

---

## Technical Highlights

### Parsing & Syntax Design

Implemented a grammar-driven parser capable of processing structured program input, including nested expressions and control-flow constructs.

### Abstract Syntax Tree (AST)

Constructed AST representations to model source programs in a hierarchical and semantically meaningful structure suitable for later compilation stages.

### Intermediate Representation (IR)

Designed and experimented with intermediate representations that transform high-level program logic into a lower-level form appropriate for analysis, optimization, and code generation.

### Control Flow Graph (CFG)

Implemented CFG generation to model execution paths within a program, enabling reasoning about:

* Conditional branching
* Loops
* Structured control flow
* Program execution order

### LLVM Backend Exploration

Explored LLVM-based code generation by translating internal compiler representations into LLVM IR and investigating instruction-level behavior through debugging and validation workflows.

---

## Challenges Encountered

Building a compiler exposed several practical software engineering and systems programming challenges:

* Managing increasing complexity across multiple compiler stages
* Debugging low-level IR and generated instruction flows
* Handling parser edge cases and control-flow interactions
* Understanding how frontend design decisions affect backend behavior
* Working with limited external guidance due to the project's custom architecture

---

## Lessons Learned

This project significantly improved my understanding of:

* Systems design and modular architecture
* Compiler pipelines and stage separation
* Intermediate Representation (IR) design
* Graph-based program representations (CFGs)
* Tree structures and recursive transformations (ASTs)
* Debugging complex multi-stage systems
* Tradeoffs between correctness, simplicity, and extensibility

One of the most valuable lessons was learning how small architectural decisions can have cascading effects throughout an entire software system (this is the main reason of discontinuing this, this decision was a fundemental part of it).

---

## Current Status

**Development is currently paused.**

The project reached a point where continuing feature development would have provided diminishing educational value without revisiting several architectural decisions.

Development was paused to consolidate the lessons learned and to allow future iterations to begin from a stronger architectural foundation.

---

## Future Direction

The knowledge gained from this project will be applied toward a more structured compiler implementation seperate from this one with:

* Clearly defined language specifications
* Cleaner separation between frontend, IR, and backend stages
* Improved architecture and namespace organization
* Incremental feature expansion
* Better testing and validation workflows

Potential future language features include:

* Functions
* Arrays
* control-flow constructs
* Additional type-system features
* Basic optimization passes

---

## Final Note

Although incomplete in feature scope, this project represents a substantial exploration of compiler engineering, systems programming, and software architecture.

The project served as a practical investigation into how modern compilers are structured and provided valuable experience working with parsing systems, intermediate representations, control-flow analysis, and backend code generation techniques.
