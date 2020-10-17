#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>

template <typename T>
class ResourceAllocator
{
public:
        static std::shared_ptr<T> Get(std::string file_path)
        {
                auto it = resources.find(file_path);
                if (it != resources.end())
                {
                        if (!it->second.expired())
                        {
                                std::cout << "resource already loaded at path: " << file_path << std::endl;
                                return std::shared_ptr<T>(it->second);
                        }
                        else
                        {
                                std::cout << "resource expired at path: " << file_path << std::endl;
                                std::shared_ptr<T> resource = Allocate(file_path);
                                it->second = resource;
                                return resource;
                        }
                }
                else
                {
                        std::shared_ptr<T> resource = Allocate(file_path);
                        resources.insert(std::make_pair(file_path, resource));
                        return resource;
                }
        }

        static std::shared_ptr<T> Add(std::string name, std::shared_ptr<T> resource)
        {
                auto it = resources.find(name);
                if (it != resources.end())
                {
                        if (it->second.expired)
                        {
                                it->second = resource;
                                return resource;
                        }
                        std::cout << "attempted to add " << name << " to the allocator, but it already exists!" << std::endl;
                        return std::shared_ptr<T>(it->second);
                }
                else
                {
                        resources.insert(std::make_pair(name, resource));
                        return resource;
                }
        }

        static bool AddFakeUser(std::string file_path)
        {
                auto it = fake_users.find(file_path);
                if (it != fake_users.end())
                {
                        return false;
                }
                else
                {
                        fake_users.insert(std::make_pair(file_path, Get(file_path)));
                        return true;
                }
        }

        static bool RemoveFakeUser(std::string file_path)
        {
                auto it = fake_users.find(file_path);
                if (it != fake_users.end())
                {
                        fake_users.erase(it);
                        return true;
                }
                else
                {
                        return false;
                }
        }

protected:
        static std::shared_ptr<T> Allocate(std::string file_path);
// {
//         return std::make_shared<T>();
// }

        static inline std::unordered_map<std::string, std::shared_ptr<T> > fake_users;
        static inline std::unordered_map<std::string, std::weak_ptr<T> > resources;
};
