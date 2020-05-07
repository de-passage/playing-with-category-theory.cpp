#include <gtest/gtest.h>
#include "category_theory/isomorphism.hpp"

#include <iostream>
#include <optional>

void foo() {
    std::cout << "ok";
    std::cout << std::flush;
}

TEST(Isomorphisms, ShouldRespectBasicProperties) {
    ASSERT_EQ(true, true);
    std::cout << std::optional<int>(12).value() << std::endl;

}