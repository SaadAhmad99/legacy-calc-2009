#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include <string>
#include <map>

class CmdLineOption {
public:
    virtual ~CmdLineOption() {}
};

class CmdLineOptionInt : public CmdLineOption {
    int value;
public:
    CmdLineOptionInt(const std::string&, const std::string&) : value(0) {}
    int getValue() const { return value; }
};

class CmdLineOptionFloat : public CmdLineOption {
    float value;
public:
    CmdLineOptionFloat(const std::string&, const std::string&) : value(0.0f) {}
    float getValue() const { return value; }
};

class CmdLineOptionFlag : public CmdLineOption {
    int key;
public:
    CmdLineOptionFlag(const std::string&, const std::string&, bool, int k)
        : key(k) {}
    int getValueKey() const { return key; }
};

class CmdLineParser {
public:
    CmdLineParser() {}
    void setMainHelpText(const std::string&) {}
    void setMainHelpTextEnd(const std::string&) {}
    void addMutExclCmdLineOption(CmdLineOption*) {}
    void setMutExclUsageText(const std::string&) {}
    void addCmdLineOption(CmdLineOption*) {}
    void setMinNumberArgs(int) {}

    bool parseCmdLine(int, char**) { return true; }
    void printUsage() {}

    CmdLineOption* getCmdLineOption(const std::string&) { return nullptr; }
    CmdLineOption* getMutExclOption() { return nullptr; }
};

#endif
