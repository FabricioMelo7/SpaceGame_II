#ifndef Lander_H // header guard
#define Lander_H
#include <raylib.h>
#include <string>
#pragma once

struct Ship
{
    std::string m_Name = "";
    Texture2D texture;
    Texture2D animatedTexture;
    Rectangle rec;

    const float Thrust_value = 1.3f;
    const float SideThrust_Value = 5.0f;
    const float RequiredLandingSpeed = -20.0f;

    Vector2 Thrust;
    float ship_baseWeight;
    float mass = 0.1f;
    Vector2 f; // Force
    Vector2 position;
    Vector2 dr; //Position Change within the frame
    Vector2 v; //Current Speed
    Vector2 dv; //Speed changed within the frame

    float fuel = 1000.0f;
    float engine_FuelConsumption = 0.5f;

    void Init(std::string name)
    {
        this->m_Name = name;
        this->dr.y = 0.0f;
        this->dr.x = 0.0f;
        this->v.y = 0.0f;
        this->v.x = 0.0f;
        this->dv.y = 0.0f;
        this->dv.x = 0.0f;
        this->Thrust.y = 0.0f;
        this->Thrust.x = 0.0f;
    }   
};
#endif //TICKET_H