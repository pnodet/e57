# E57 Rust Library
[![Build Status](https://github.com/cry-inc/e57/workflows/CI/badge.svg)](https://github.com/cry-inc/e57/actions)
[![Crates.io](https://img.shields.io/crates/v/e57.svg)](https://crates.io/crates/e57)
[![Documentation](https://docs.rs/e57/badge.svg)](https://docs.rs/e57)
[![No Unsafe](https://img.shields.io/badge/unsafe-forbidden-brightgreen.svg)](https://doc.rust-lang.org/nomicon/meet-safe-and-unsafe.html)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Dependencies](https://deps.rs/repo/github/cry-inc/e57/status.svg)](https://deps.rs/repo/github/cry-inc/e57)

A pure Rust library for reading and writing E57 files. No unsafe code, no bloaty dependencies.

Check out the [tools folder](tools/) for some code examples that show how to use the library.

## E57 File Format
The E57 file format is used for storing point clouds and related image data.
Its a neutral file format not tied to any specific vendor or device type and therefore often used
as exchange format between different applications and organizations.
Typically its used for data generated by terrestrial and handheld laser scanners.
It can also handle data generated by airborn laser scanners,
but for that use case the LAS/LAZ file format is more commonly used.

## Known Limitations
* Does not support Point Grouping
* Does not support Index Packets
* Does not support point attributes of type String

## Please report incompatible files!
If you found an E57 file that can be read with other software but produces an error with this crate,
please let me know and create an issue on Github.
The same applies for E57 files that were created by this library and are not correctly read by this or any other software.
I want this library to work with as many files and applications as possible!

Ideally, you can provide a link to the file itself. If that is not possible,
please include the full error message and the name of the software that produced the file.
If possible, please also include the XML section of the file.

## Motivation
The E57 file format is well established for exchanging data produced by terrestrial lasers scanners.
However, there are not many implementations that can read and write this file format.
Most applications use the original C++ reference implementation (see http://www.libe57.org/)
or the well maintained fork from Andy Maloney (see https://github.com/asmaloney/libE57Format).

I thought it would be nice to have a pure Rust solution without any unsafe code.
In my opinion Rust is an excellent choice for parsers of untrusted data,
especially if you plan to use the code in something like a cloud backend.

If you want to handle E57 files inside a Rust project this crate will also avoid
all the issues that come with integrating C++ code.

## Code Coverage
The Visual Studio Code tasks included in this repository contain some tasks for code coverage measurement.
To be able to run them, you need to install `grcov` with the command `cargo install grcov` and the
LLVM tools by running `rustup component add llvm-tools`.
