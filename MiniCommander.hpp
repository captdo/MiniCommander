// author: Michael Grupp
// https://github.com/MichaelGrupp/MiniCommander

#ifndef MINICMD
#define MINICMD

#include <set>
#include <map>
#include <vector>
#include <functional>
#include <iostream>

enum class Policy {
    required,
    anyOf,
    optional
};

struct OptionGroup {
    Policy p;
    std::string groupDescription;
    std::map<std::string, std::string> options;
    OptionGroup(Policy p, std::string groupDescription) {
        this->p = p;
        this->groupDescription = groupDescription;
    }
    void addOption(std::string flag, std::string desc = "") {
        options[flag] = desc;
    }
};

class MiniCommander {
public:
    MiniCommander(const int &argc, char **argv) {
        for (int i = 1; i < argc; ++i)
            tokens.push_back(std::string(argv[i]));
    }

    void addOptionGroup(OptionGroup group) {
        optionGroups.push_back(group);
    }

    bool checkFlags() const {
        bool valid = false;
        for (auto& group : optionGroups) {
            Policy p = group.p;
            for (auto& o : group.options) {
                valid = optionExists(o.first);
                if (p == Policy::required && !optionExists(o.first))
                    return false;
                else if (p == Policy::anyOf && optionExists(o.first))
                    break;
                else if (p == Policy::optional)
                    valid = true;  // don't care
            }
            if (!valid)
                return false;
        }
        return valid;
    }

    void printHelpMessage(std::string title = "\nUSAGE") const {
        std::cerr << title << std::endl;
        for (auto& group : optionGroups) {
            std::cerr << "\n[" + group.groupDescription + "]\n";
            for (auto &o : group.options)
                std::cerr << o.first << "\t" << o.second << std::endl;
        }
    }

    const std::string getParameter(const std::string &option) const {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(tokens.begin(), tokens.end(), option);
        if (itr != tokens.end() && ++itr != tokens.end()) {
            if (!isOption(*itr))
                return *itr;  // return parameter if it is not another option
        }
        return "";
    }

    bool optionExists(const std::string &option) const {
        return std::find(tokens.begin(), tokens.end(), option) != tokens.end();
    }

private:
    bool isOption(const std::string& str) const {
        for (auto& group : optionGroups) {
            if (group.options.find(str) != group.options.end())
                return true;
        }
        return false;
    }

    std::vector<std::string> tokens;
    std::vector<OptionGroup> optionGroups;
};

#endif  // MINICMD