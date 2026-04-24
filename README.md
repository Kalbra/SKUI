# SKUI - Custom user interfaces with all levels of logic
![build badge](https://dev.skui.eu/SKUI/SKUI/actions/workflows/build-skui.yml/badge.svg)

![SKUI NodeEditor](/docs/images/SKUI_NodeEditor_Screenshot1.png)

## Roadmap
Since SKUI is it's early developing stage the actual application is not usable for production.
Therefore the most important thing to look at is the roadmap, where you see the progress and the direction of the project.
The developing is split up into [milestones](https://dev.skui.eu/SKUI/SKUI/milestones) that include specific task issues.
A [milestones](https://dev.skui.eu/SKUI/SKUI/milestones) has a rought topic, but might also include issues that aren't
related to that topic. The [milestones](https://dev.skui.eu/SKUI/SKUI/milestones) have rather than in most roadmaps
no deadline. So that the developing can happen without pressure and botched jobs.
The current planing considers the following [milestones](https://dev.skui.eu/SKUI/SKUI/milestones):

### 1. ~~Jufo Comptition~~
For the jugend forscht compedition there needs to be a minimal version to show to the judge. 
It inludes only Nodes and Visuals to send an string via serial connection to stear a servo.

### 2. [GUI Phase](https://dev.skui.eu/SKUI/SKUI/milestone/2)
The GUI phase includes a usable GUI that the User can intuitive use without major bugs.
It does not aim to to reinvent the nodes itself, but to provide enhancements and new features with the existing node system.
One major part of this milestone is the introduction of a working NodeEditor. 
Serialization is not planed for this phase.

### 3. [Python embedding](https://dev.skui.eu/SKUI/SKUI/milestone/3)
Python will be a big part of SKUI. Therefore the embedding of Python is requiered. 
This milestone yet again rethinks the Node as an Python function. 
This solves a lot of problems regarding the connection of Nodes.
The tasks in this milestone can be distributed into the following topics:
- Type System
- Serialization
- Python API

### 4. [Standard library](https://dev.skui.eu/SKUI/SKUI/milestone/4)
For SKUI a standard library is indispensable. It is the requirement to build first hardware demonstrators.
The standard library should at least include:
- Arithmetic
- String manipulation
- Drivers
   - TCP
   - HTTP
   - Serial
   - File IO
- Visuals
    - QML subclass
    - Progressbar
    - Gauge
    - Text field
    - ...

## Build (Windows)
SKUI has no release at the moment, therefore you have to build it yourself.
SKUI is written in C++ and heavily depends on Qt. It uses CMake as it's build system.
The following steps help you to compile the application:

### 1. Installing required toools
First of all you need a working C++ compiler. We use MSCV 2022 as a compiler, 
but you can use any that is comatible to Qt and works on your system.
The easiest way to install MSCV is via Viusal Studio. You can install it by clicking [here](https://visualstudio.microsoft.com/de/downloads/).
Choose the "Desktop developing with C++" installation kit to be on the safe side.

Next up is Qt, we use version 6.5. To install it use one of Qt's installation methods. 
A link to the installation guide of Qt can be found [here](https://doc.qt.io/qt-6/get-and-install-qt.html)

You can install CMake with various techniques. Ship it with Visual Studio, ship it with Qt or 
install it manually from CMake itself. [Link](https://cmake.org/download/) to CMake download page.

To download and work on the project you need to install git. A link to download it can be found [here](https://git-scm.com/install/windows).

Great know you have all the tools you need to build SKUI!

### 2. Download SKUI sources
To build SKUI you first have to copy the sources to a directory of your choice.
Go into a directory you want to use and clone the project via:
```shell
git clone https://dev.skui.eu/SKUI/SKUI.git
```
Go into the downloaded directory:
```shell
cd SKUI
```

Now you are good to go to build SKUI yourself.

### 3. Build SKUI
⚠️ Under unstruction ⚠️

## Contribution
There are several ways you can help to contribute to the project. 
If its writting code, giving feedback or just spreading the idea.
A complete guide of how you can contriblte to SKUI can be found at [CONTRIBUTING.md](/CONTRIBUTING.md) 


## Licence
SKUI is licenced under GNU GPL v3. The complete licence can be found at [LICENSE](/LICENSE).