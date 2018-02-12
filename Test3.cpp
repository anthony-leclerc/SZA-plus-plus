//
// Created by scorsi on 08/02/18.
//

#include "api/pp/conf.hpp"

using namespace std::literals::string_literals;

using namespace zia::apipp;
/// TODO:  Helpers to set values in ConfElem constructor.
/// TODO:  Add set version with copyable value arguments.
void test3() {
    auto conf = ConfElem()
        .set(ConfMap())
        .set_at("string_test", ConfElem().set("first_value"s))
        .set_at("integer_test", ConfElem().set(42))
        .set_at("double_test", ConfElem().set(42.42))
        .set_at("nested_map_test", ConfElem()
            .set(ConfMap())
            .set_at("op", ConfElem().set(1)))
        .set_at("array_test",
                ConfElem().set(ConfArray())
                    .push(ConfElem().set(true))
                    .push(ConfElem().set(false)));

    std::cout << conf.get_at("string_test").get<std::string>() << std::endl;
    std::cout << conf.get_at("integer_test").get<int>() << std::endl;
    std::cout << conf.get_at("integer_test").get<long long>() << std::endl;
    std::cout << conf.get_at("double_test").get<double>() << std::endl;
    std::cout << conf.get_at("double_test").get<float>() << std::endl;

    std::cout << conf.get_at("nested_map_test").get_at("op").get<int>() << std::endl;

    std::cout << conf.get_at("array_test").get_at(0).get<bool>() << std::endl;
    std::cout << conf.get_at("array_test").get_at(1).get<bool>() << std::endl;

    /// Really ugly syntax...
    /// std::cout << (*conf)["array_test"]->operator[](0)->get<bool>() << std::endl;
    std::cout << conf["array_test"][0].get<bool>() << std::endl;

    /// Test with copy
    {
        auto conf3 = std::make_shared<ConfElem>();
        conf3->set(ConfArray());

        auto conf4 = std::make_shared<ConfElem>();
        conf4->set("toto"s);

        conf3->push(conf4);
        conf3->push(ConfElem().set("titi"s));

        /// Copy a previous config object but doesn't keep references.
        auto conf2 = conf;
        conf2.set(ConfMap()).set_at("data", conf3);
        conf2.set_at("string_test", ConfElem().set("In Copied Map."s));

        conf4->set("Oui"s);

        std::cout << conf.get_at("string_test").get<std::string>() << std::endl;
        std::cout << conf2.get_at("string_test").get<std::string>() << std::endl;
        std::cout << conf2.get_at("data").get_at(0).get<std::string>() << std::endl;
        std::cout << conf2.get_at("data").get_at(1).get<std::string>() << std::endl;
    }

    /// Test with constructor helpers
    {
        auto conf3 = std::make_shared<ConfElem>(ConfArray());
        ///conf3->set(ConfArray());

        auto conf4 = std::make_shared<ConfElem>("toto");
        ///conf4->set("toto"s);
        auto confNope = std::make_shared<ConfElem>(std::vector<int>());
        auto confMap = std::make_shared<ConfElem>(ConfMap());
        auto confInt = std::make_shared<ConfElem>(426879);
        auto confLong = std::make_shared<ConfElem>(8566756747856);
        auto confBool = std::make_shared<ConfElem>(true);
        auto confFloat = std::make_shared<ConfElem>(42.42);
        auto confDouble = std::make_shared<ConfElem>(42.42856756567);
        conf3->push(conf4);
        conf3->push(ConfElem("titi"s));

        auto conf2 = conf.set(ConfMap()).set_at("data", conf3);
        conf4->set("Oui"s);

        std::cout << conf2.get_at("data").get_at(0).get<std::string>() << std::endl;
        std::cout << conf2.get_at("data").get_at(1).get<std::string>() << std::endl;
    }

//    auto conftest = ConfElem(42);
}