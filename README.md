# GetColumnsCppApp

A simple C++ utility that takes a directory path and prints the visible column names in Windows Explorer's detail view for that directory using the Windows Shell API and `IColumnManager`.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [License](#license)

## Introduction

This tool helps you retrieve the list of column names that are visible in the Windows Explorer detail view for a specific directory. It utilizes the Windows Shell API, particularly the `IColumnManager` interface, to access the column information.

## Features

- Retrieves visible column names for any directory in Windows Explorer.
- Utilizes modern C++ practices and Windows Shell utility functions.
- Simplified code without unnecessary complexities.

## Prerequisites

- **Operating System**: Windows Vista or later.
- **Development Environment**: Visual Studio (2015 or later recommended).
- **Windows SDK**: Ensure that the Windows SDK is installed and configured in your development environment.

## Building
### Using Visual Studio Developer Command Prompt
1. Clone the repository
```
git clone https://github.com/shea-c4/GetColumnsCppApp.git
```
2. Open a Visual Studio Developer Command Prompt

3. Navigate to the project directory.
```
cd path\to\GetColumnsCppApp
```

4. Compile the source code
``` compiling
cl /EHsc /D_UNICODE /DUNICODE GetColumnsCppApp.cpp /link shell32.lib ole32.lib propsys.lib
```
- Explanation of Flags:
  - /EHsc: Enables standard C++ exception handling.
  - /D_UNICODE /DUNICODE: Defines Unicode support.
  - /link: Specifies the libraries to link against.

5. Run the executable

After successful compilation, run the executable:
``` console
GetColumnsCppApp.exe C:\Windows
```

## License

This project is licensed under the [MIT License](LICENSE.txt)