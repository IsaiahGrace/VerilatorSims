[![Gtest](https://github.com/IsaiahGrace/VerilatorSims/actions/workflows/gtest.yml/badge.svg?branch=main)](https://github.com/IsaiahGrace/VerilatorSims/actions/workflows/gtest.yml)

# HDL
What can I build with SystemVerilog, Verilator, and C++? I'm not sure yet, but a TIS-100 based CPU looks like and interesting place to start.

## Basic TIS-100 arch
```
                       TOP
    ┌───────────────────────────────────────────┐
    │NODE                                       │
    │    ┌────────┐                    ┌──────┐ │
    │    │   PC   │                    │  RF  │ │
    │   ┌┴────────┴┐                   │      │ │
    │   │ I-CACHE  │                   └──────┘ │
    │   │          │                            │
    │   └──────────┘                            │
LEFT│                         ┌─────────────┐   │RIGHT
    │     ┌─────────────┐     │             │   │
    │     │ DECODE      │     │   ALU       │   │
    │     │             │     │             │   │
    │     │             │     │             │   │
    │     │             │     │             │   │
    │     │             │     │             │   │
    │     └─────────────┘     │             │   │
    │                         └─────────────┘   │
    │                                           │
    └───────────────────────────────────────────┘
                       DOWN
```
