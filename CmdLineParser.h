#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H

#include <string>
#include <map>
#include <stdexcept>

class CmdLineOption {
public:
    virtual ~CmdLineOption() {}
};

class CmdLineOptionInt : public CmdLineOption {
public:
    CmdLineOptionInt(const std::string&, const std::string&) {}
    int getValue() const { return 0; }
};

class CmdLineOptionFloat : public CmdLineOption {
public:
    CmdLineOptionFloat(const std::string&, const std::string&) {}
    float getValue() const { return 0.0f; }
};

class CmdLineOptionFlag : public CmdLineOption {
    int valueKey;
public:
    CmdLineOptionFlag(const std::string&, const std::string&, bool, int key)
        : valueKey(key) {}

    int getValueKey() const { return valueKey; }
};

class CmdLineParser {
public:
    void setMainHelpText(const std::string&) {}
    void setMainHelpTextEnd(const std::string&) {}
    void addMutExclCmdLineOption(CmdLineOption*) {}
    void addCmdLineOption(CmdLineOption*) {}
    void setMutExclUsageText(const std::string&) {}
    void setMinNumberArgs(int) {}

    bool parseCmdLine(int, char**) { return true; }

    CmdLineOption* getCmdLineOption(const std::string&) { return nullptr; }
    CmdLineOption* getMutExclOption() { return nullptr; }

    void printUsage() {}
};

#endif
