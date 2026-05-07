> ⚠️ **This site is under construction** ⚠️

# Contributing to Skui

First off, thanks for taking the time to contribute! 🫶

The project itself is an early stage, therefore a lot of things are still open and dynamic.
This means that the way you can help is not set in stone.
All types of contributions are encouraged and valued.
See the TABLE OF CONTENT for different ways to help and 
details about how this project handles them.

> If you like the project, but just don't have the time to contribute, that's fine.
> There are other easy ways to support the poject:
> - Star the project
> - Post about it
> - Tell someone
> - Tell another one


# Table of Content
- Ways to Contribute
    - [Getting started with writting code](#getting-started-with-writting-code)
    - [Report a bug](#report-a-bug)
    - [Suggest Enhancements, Features or Usecases](#suggest-enhancements-features-or-usecases)
    - [Proofread Grammar & Spelling](#proofread-grammar--spelling)
    - [Give Feedback](#give-feedback)
- Styleguide
    - C++


## Ways to Contribute

### Getting started with writting code
Most projects suggest to start by editing the documenation to get into the project.
We think this is a bad approach. How should someone explain the project,
if he doesn't even know the project. 
This is just fustrating for the person who wants to contribute.
See Section PROOFREAF if you want to work on the documentation.
For the others, start by looking at the STYLEGUID, 
the CODEBASE and the DEV DOCUMENTATION to get an overview for the project. 
For the beginning refactoring should be your primary objective.
No codebase is perfect, so you will find parts to improve, update or prittier to code.
You can also watchout for GOOD FIRST ISSUES. 
They are easy to implement and perfect for the beginning.

### Report a bug
A good bug report shouldn’t leave others needing to chase you up for more information. 
Therefore, we ask you to investigate carefully, 
collect information and describe the issue in detail in your report.
We use the Gitea Issue Tracker to track bugs and errors.
Submit your bug report with the BUG ISSUE TEMPLATE.
It guides you, how to write a proper bug report.

### Suggest Enhancements, Features or Usecases
As Skui is in its early stages, a lot of functionality is missing.
You can help requesting features and guide the direction of the
project by your ideas. Any kind of request brings the
project foreward and is valueable.

We use the Gitea Issue Tracker. 
You can create a Issue with your request or write an email to 
[kalle@skui.eu](mailto:kalle@skui.eu)

**Important:** 
- Be clear and descriptive with your request
- Explain why this request is important
- Detailed step by step guides help to understand your request
- Providing Screenshots or Videos makes everything more clear
- You can also mention other applications and their approach

### Proofread Grammar & Spelling
At the moment the team has no native speaker, 
nor grammer / spelling experts.
Therefore there are a lot of grammar and spelling mistakes 
in the documentation.

All pull requests fixing any of these mistakes are appreciated 
and welcome :)

### Give Feedback
If you have feedback or just want to say something,
you can write to [kalle@skui.eu](mailto:kalle@skui.eu).


## Code Styleguide
In this section, you will find out how we write good code that meets our standards.

### C++
C++ is a language that allows a lot in therms of code style. There are a ton of different approches about how to do what
and none of them are "the one and only right way". But we will set standards here, not to be right, but to do it the same fachion
for the whole project.

#### Automatic Clang checking
Not all standards are covered here. Some are automaticly ensured by clang.
The definition can be found at the [`.clang-format`](.clang-format).
Clang formatting is ensured by a gitea action before merge. 
But you can also do in manually.

#### File structure
We use `cpp` and `hpp` file extensions to seperate them clearly from `C`.
The naming convention is equal to the variables, lowercase and seperated by `_`.
Directorys follow the same convention. 
Every directory should have it's own `CMakeLists.txt`.

#### Variables
Variable must be precise. Single character variables like `i`, `j`, `a` are permitted in loops if not overused. All variables must be lowercase with `_` as a seperator. Do not specify the type in the variable name. We don't use auto. Do not declare a variable without implementing it:
```c++
// DON'T
int a = 10; // Single character
int variable_1 = 420; // Says nothing about the value
int int_content_page_tab_view_count = 67; // Redundant type, to much information
auto BoxGeometry = QRect(0, 0, 100, 100); // Usage of auto, written uppercase
QWidget *tab_view; // No implemenation of variable
// DO
int page_count = 67;
QRect box_geometry = QRect(0, 0, 100, 100); 
QWidget *tab_view = nullptr;
```

**Member Variables**

Member variables must start with the prefix `m_`.
They should be private with a getter/setter method interface:

```c++
// DON'T
...
public:
    QPoint m_position_hint = QPoint(0, 42); // Member definded in public section, could be used incorrectly

private:
    bool matches = false; // No m_prefix
...
// DO
...
private:
    QPoint m_position_hint = QPoint(0, 42);
    bool m_matches = false;
...
```

#### Methods
Method names must be presice. Methods names must start low and the words are speperated by a upper character. 
Methods must have the least scope possible controlled by `private:`, `protected:` and `public:`. 
Public and protected methods must be documented via Doxygen.
```c++
// DON'T
...
public:
    bool m_focus(); // Used member variable styling
    void addVisual(Visual *visual); // No documenation
...
// DO
...
protected: // Useage of restriced scope
    /** @brief Retruns weather the class has focus or not  
    ...
    */
    bool hasFocus();

    /** @brief Adds a Visual to the Panel.
    ...
     */
    void addVisual(Visual *visual);
...
```

**Nullcheck**

There are two types of pointers as an argument to a method or function.
The pointer is nullable, because the pointer can logically be null.
In this case just use a `if` case:
```c++
void MyClass::initSomething(QWidget *widget) 
{
    if(widget) {
        // Do something with it
    }
}
```
The other case would be, that you have a pointer that should not be null and you might have no logic to handle this case.
Normally you would use a reference for this. But for QObjects, thats
not practical, therefore we use `Q_ASSERT` to crash the program carefully:
```c++
void MyClass::initSomething(QWidget *requiered_widget)
{
    Q_ASSERT(requiered_widget)
    
    // Savely access requiered_widget
}
```
One of these methods must be used to check pointers before usage.
Elsewise it would leed to undefined behaviour with crashing without
error reporting. 

### Setter/Getter methods
Because other classes should not access your member variables directly,
you have to use setter and getter methods. Setters normally look like this: `setValue(int value)`. 
In contrast for getters you could either use `int getValue()` or `int value()`. We use the second style. An exception from this are boolean
return type, there we use `bool isValue()`
Getter methods must be `inline` and `const`. As well as for methods,
getters and settes as a subset of those need to have documentation.
```c++
// DON'T
...
public:
    int value(); // Method is not const and inline. Documentation is missing
    inline const getValue(); // Method uses get-style
...
// DO
...
public:
    /** @brief Set the value
    ...
    */
    void setValue(int value);

    /** @brief Get the value
    ...
    */
    inline const int value();
...
```

### Qt's property system
Qt has a property system and you should use it if your class is a derivitve of `QObject`.

### Marcos
Marcos are a powerful tool, they are written uppercase.
We normally only use conditional macros.
We do not use macros for constants, but `static const`.
```c++
// DON'T
#define debug_build // Is lowercase
#define PAD_SIZE 5 // Use const for constants
// DO
#define DEBUG_BUILD
static const int PAD_SIZE = 5;
```