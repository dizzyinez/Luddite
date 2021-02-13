#pragma once
#include "entt.hpp"

namespace utils
{
template<typename T>
void clone_component(const entt::registry &from, entt::registry &to) {
        const auto *data = from.data<T>();
        const auto size = from.size<T>();

        if constexpr (std::is_empty<T>::value) {
                to.insert<T>(data, data + size);
        }
        else
        {
                const auto *raw = from.raw<T>();
                to.insert<T>(data, data + size, raw, raw + size);
        }
}
template<typename T, typename Single, typename ... Rest> //we have to force it to take in at least two otherwise the two template functions become ambiguous
void clone_component(const entt::registry &from, entt::registry &to) {
        clone_component<T>(from, to);
        clone_component<Single, Rest...>(from, to);
}

template <typename ... Ts>
void clone_registry(const entt::registry &from, entt::registry &to)
{
        //clear the componenets from the registry
        // to.clear<Ts...>();

        // destroy all the enitities in the registry to clone to
        to.each([&to](auto entity) {
                        to.destroy(entity);
                });

        //assign all the entities that are going to be clonedZ
        const auto *from_data = from.data();
        const auto from_size = from.size();
        to.assign(from_data, from_data + from_size, entt::null);

        //copy components to the registry
        clone_component<Ts...>(from, to);
}


template <typename ... Ts>
void copy_registry(const entt::registry &from, entt::registry &to)
{
        //clear the componenets from the registry
        to.clear<Ts...>();

        //make sure that the target registry doesn't contain any entities that don't exist in the copying registry
        std::vector<entt::entity> entities_to_destroy;
        to.each([&from, &entities_to_destroy](auto entity) {
                        if (!from.valid(entity))
                                entities_to_destroy.emplace_back(entity);
                });
        for (auto e : entities_to_destroy)
                to.destroy(e);
        //make sure that the target registry has all the entities we want
        from.each([&to](auto entity) {
                        if (!to.valid(entity))
                                to.create(entity);
                });

        clone_component<Ts...>(from, to);
}
template <typename T>
void replace_single_component(entt::registry &from, entt::registry &to)
{
        to.clear<T>();
        to.view<T>().each([&from, &to](entt::entity entity, T& t) {
                        if (!from.valid(entity))
                                to.destroy(entity);
                });
        from.view<T>().each([&to](entt::entity entity, T& t) {
                        if (!to.valid(entity))
                                to.create(entity);
                });
        utils::clone_component<T>(from, to);
}
}