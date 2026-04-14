#pragma once
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace aura {

class CentralRegistry {
private:
    std::map<std::string, std::string> config;
    CentralRegistry() {}

public:
    CentralRegistry(const CentralRegistry&) = delete;
    CentralRegistry& operator=(const CentralRegistry&) = delete;

    static CentralRegistry& instance() {
        static CentralRegistry reg;
        return reg;
    }

    void set(const std::string& key, const std::string& value) {
        config[key] = value;
    }

    std::vector<std::pair<std::string, std::string>> all() const {
        std::vector<std::pair<std::string, std::string>> out;
        for (const auto& kv : config) out.push_back(kv);
        return out;
    }
};

} // namespace aura
