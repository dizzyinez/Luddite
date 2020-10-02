#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>

template <typename T>
class ResourceManager
{
public:
        std::shared_ptr<T> Get(std::string file_path)
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

protected:
        virtual std::shared_ptr<T> Allocate(std::string file_path) = 0;
// {
//         return std::make_shared<T>();
// }


        std::unordered_map<std::string, std::weak_ptr<T> > resources;
};
