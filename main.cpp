#include <iostream>
#include "api/conf.h"
#include "api/http.h"
#include "api/module.h"
#include "api/net.h"
#include "api/pp/module.hpp"

// SZA++ Module Implementation
class Test : public zia::apipp::Module {
public:
    ~Test() override = default;

    bool perform() override {
        this->response->setStatus(200, "OK")
                ->addHeader("Set-Cookie", "test")
                ->addHeader("Set-Cookie", "toto")
                ->body = "test";
    }
};

// Basic SZA Module Implementation
class BasicTest : public zia::api::Module {
private:
    zia::api::Conf conf;

public:
    ~BasicTest() override = default;

    bool config(const zia::api::Conf& conf) override {
        this->conf = conf;
        return true;
    }

    bool exec(zia::api::HttpDuplex &duplex) override {
        duplex.resp.status = 200;
        duplex.resp.reason = "OK";

        duplex.resp.headers["Set-Cookie"] = "test";
        duplex.resp.headers["Set-Cookie"] += ", toto";

        for (const auto &item: "test") {
            duplex.resp.body.push_back(static_cast<std::byte>(item));
        }
        duplex.resp.body.pop_back(); // Remove useless \0 caused by the foreach loop

        return true;
    }
};

int main() {
    {
        // SZA++ Example
        Test module = Test();
        zia::api::HttpDuplex duplex;

        module.exec(duplex);

        for (const auto &item: duplex.resp.body) {
            std::cout << static_cast<char>(item);
        }
        std::cout << std::endl;

        for (const auto &item: duplex.resp.headers) {
            std::cout << item.first << "=" << item.second << std::endl;
        }
    }

    {
        // Basic SZA Example
        BasicTest module = BasicTest();
        zia::api::HttpDuplex duplex;

        module.exec(duplex);

        for (const auto &item: duplex.resp.body) {
            std::cout << static_cast<char>(item);
        }
        std::cout << std::endl;

        for (const auto &item: duplex.resp.headers) {
            std::cout << item.first << "=" << item.second << std::endl;
        }
    }
    return 0;
}