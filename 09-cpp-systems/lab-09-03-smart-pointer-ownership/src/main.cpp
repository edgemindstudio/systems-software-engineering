#include <iostream>
#include <memory>
#include <string>

struct Resource {
    explicit Resource(std::string value) : name(std::move(value)) {
        std::cout << "resource_constructed=" << name << '\n';
    }
    ~Resource() {
        std::cout << "resource_destroyed=" << name << '\n';
    }
    std::string name;
};

int main() {
    {
        auto unique = std::make_unique<Resource>("unique");
        std::cout << "unique_name=" << unique->name << '\n';
    }

    std::weak_ptr<Resource> observer;
    {
        auto first = std::make_shared<Resource>("shared");
        observer = first;
        std::cout << "shared_use_count_initial=" << first.use_count() << '\n';
        {
            auto second = first;
            std::cout << "shared_use_count_nested=" << first.use_count() << '\n';
        }
        std::cout << "shared_use_count_after_nested=" << first.use_count() << '\n';
        std::cout << "weak_expired_inside=" << (observer.expired() ? "yes" : "no") << '\n';
    }
    std::cout << "weak_expired_after_owner_scope=" << (observer.expired() ? "yes" : "no") << '\n';

    return 0;
}
