#pragma once
#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>

template <typename T, typename ID = std::string>
class ResourceAllocator
{
public:
        static std::shared_ptr<T> Get(ID id)
        {
                auto it = resources.find(id);
                if (it != resources.end())
                {
                        if (!it->second.expired())
                        {
                                // std::cout << "resource already loaded at path: " << id << std::endl;
                                return std::shared_ptr<T>(it->second);
                        }
                        else
                        {
                                std::cout << "resource expired: " << id << std::endl;
                                std::shared_ptr<T> resource = Allocate(id);
                                it->second = resource;
                                return resource;
                        }
                }
                else
                {
                        std::shared_ptr<T> resource = Allocate(id);
                        resources.insert(std::make_pair(id, resource));
                        return resource;
                }
        }
        static bool Has(ID id)
        {
                auto it = resources.find(id);
                if (it != resources.end())
                {
                        if (!it->second.expired())
                        {
                                return true;
                        }
                }
                return false;
        }

        static std::shared_ptr<T> Add(ID id, std::shared_ptr<T> resource)
        {
                auto it = resources.find(id);
                if (it != resources.end())
                {
                        if (it->second.expired())
                        {
                                it->second = resource;
                                return resource;
                        }
                        std::cout << "attempted to add " << id << " to the allocator, but it already exists!" << std::endl;
                        return std::shared_ptr<T>(it->second);
                }
                else
                {
                        resources.insert(std::make_pair(id, resource));
                        return resource;
                }
        }

        static bool AddFakeUser(ID id)
        {
                auto it = fake_users.find(id);
                if (it != fake_users.end())
                {
                        return false;
                }
                else
                {
                        fake_users.insert(std::make_pair(id, Get(id)));
                        return true;
                }
        }

        static bool AddFakeUser(ID id, std::shared_ptr<T> resource)
        {
                auto it = fake_users.find(id);
                if (it != fake_users.end())
                {
                        return false;
                }
                else
                {
                        fake_users.insert(std::make_pair(id, resource));
                        return true;
                }
        }

        static bool RemoveFakeUser(ID id)
        {
                auto it = fake_users.find(id);
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
        static std::shared_ptr<T> Allocate(ID id);
// {
//         return std::make_shared<T>();
// }

        static inline std::unordered_map<ID, std::shared_ptr<T> > fake_users;
        static inline std::unordered_map<ID, std::weak_ptr<T> > resources;
};






template <typename T, typename ID = std::string>
class ResourceAllocator_not_static
{
public:
        std::shared_ptr<T> Get(ID id)
        {
                auto it = resources.find(id);
                if (it != resources.end())
                {
                        if (!it->second.expired())
                        {
                                // std::cout << "resource already loaded at path: " << id << std::endl;
                                return std::shared_ptr<T>(it->second);
                        }
                        else
                        {
                                std::cout << "resource expired at path: " << id << std::endl;
                                std::shared_ptr<T> resource = Allocate(id);
                                it->second = resource;
                                return resource;
                        }
                }
                else
                {
                        std::shared_ptr<T> resource = Allocate(id);
                        resources.insert(std::make_pair(id, resource));
                        return resource;
                }
        }
        bool Has(ID id)
        {
                auto it = resources.find(id);
                if (it != resources.end())
                {
                        if (!it->second.expired())
                        {
                                return true;
                        }
                }
                return false;
        }

        std::shared_ptr<T> Add(ID id, std::shared_ptr<T> resource)
        {
                auto it = resources.find(id);
                if (it != resources.end())
                {
                        if (it->second.expired())
                        {
                                it->second = resource;
                                return resource;
                        }
                        std::cout << "attempted to add " << id << " to the allocator, but it already exists!" << std::endl;
                        return std::shared_ptr<T>(it->second);
                }
                else
                {
                        resources.insert(std::make_pair(id, resource));
                        return resource;
                }
        }

        bool AddFakeUser(ID id)
        {
                auto it = fake_users.find(id);
                if (it != fake_users.end())
                {
                        return false;
                }
                else
                {
                        fake_users.insert(std::make_pair(id, Get(id)));
                        return true;
                }
        }

        bool AddFakeUser(ID id, std::shared_ptr<T> resource)
        {
                auto it = fake_users.find(id);
                if (it != fake_users.end())
                {
                        return false;
                }
                else
                {
                        fake_users.insert(std::make_pair(id, resource));
                        return true;
                }
        }

        bool RemoveFakeUser(ID id)
        {
                auto it = fake_users.find(id);
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
        std::shared_ptr<T> Allocate(ID id);
        std::unordered_map<ID, std::shared_ptr<T> > fake_users = {};
        std::unordered_map<ID, std::weak_ptr<T> > resources = {};
};
