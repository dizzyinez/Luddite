#pragma once

enum class Team : uint8_t
{
        PLAYER,
        ENEMY
};

struct C_Team
{
        C_Team() = default;
        C_Team(Team team_) : team(team_) {}
        Team team = Team::ENEMY;
};