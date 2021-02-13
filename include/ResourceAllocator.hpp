#ifndef ResourceAllocator_hpp
#define ResourceAllocator_hpp

#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>

template <typename T>
class ResourceAllocator
{
public:
        int GetID(const std::string& filepath)
        {
                auto it = resources.find(filepath);
                if (it != resources.end())
                {
                        if (std::get<2>(it->second) != nullptr)
                        {
                                std::get<1>(it->second) += 1;
                                return std::get<0>(it->second);
                        }
                        else
                        {
                                std::get<1>(it->second) += 1;
                                std::shared_ptr<T> resource = std::make_shared<T>();
                                resource = getResource(filepath);
                                std::get<2>(it->second) = resource;
                        }
                }

                std::shared_ptr<T> resource = std::make_shared<T>();
                resource = getResource(filepath);
                if (resource == nullptr)
                {
                        std::cout << "unable to load " << filepath << std::endl;
                        return -1;
                }

                resources.insert(std::make_pair(filepath, std::make_tuple(currentID, 1, resource)));
                return currentID++;
        }

        std::shared_ptr<T> Get(int id)
        {
                for (auto it = resources.begin(); it != resources.end(); ++it)
                {
                        if (std::get<0>(it->second) == id)
                        {
                                return std::get<2>(it->second);
                        }
                }

                return nullptr;
        }

        std::string getFilePath(int id)
        {
                for (auto it = resources.begin(); it != resources.end(); ++it)
                {
                        if (std::get<0>(it->second) == id)
                        {
                                return it->first;
                        }
                }

                return "";
        }

        bool Has(int id)
        {
                return(Get(id) != nullptr);
        }

        bool Unregister(int id)
        {
                for (auto it = resources.begin(); it != resources.end(); ++it)
                {
                        if (std::get<0>(it->second) == id)
                        {
                                std::get<1>(it->second)--;
                                return true;
                        }
                }
                return false;
        }

        void RemoveAllUnused()
        {
                for (auto it = resources.begin(); it != resources.end(); ++it)
                {
                        if (std::get<1>(it->second) == 0)
                        {
                                removeResource(std::get<2>(it->second));
                                std::get<2>(it->second) = nullptr;
                                //not erasing the map listing because it shouldn't have too many entries in order for the all the O(n) functions to impact preformace (*hopefully-_-)
                        }
                }
        }
        bool RemoveIfUnused(int id)
        {
                for (auto it = resources.begin(); it != resources.end(); ++it)
                {
                        if (std::get<0>(it->second) == id && std::get<1>(it->second) == 0)
                        {
                                removeResource(std::get<2>(it->second));
                                std::get<2>(it->second) = nullptr;
                                //not erasing the map listing because it shouldn't have too many entries in order for the all the O(n) functions to impact preformace (*hopefully-_-)
                                return true;
                        }
                }
        }
private:
        int currentID;
        // std::unordered_map<std::string, std::tuple<int, int, std::shared_ptr<T> > > resources = {{"wow", std::make_tuple(0, 1, nullptr)}}; //id, users, resource
        inline std::unordered_map<std::string, std::tuple<int, int, std::shared_ptr<T> > > resources; //id, users, resource
        virtual std::shared_ptr<T> getResource(const std::string& filepath) = 0;
        virtual void removeResource(std::shared_ptr<T> resource) = 0;
};

#endif
