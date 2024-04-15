#include "MovementController.h"

Rectangle MovementController::GetShipRectangle(const Ship* ship) 
{
    float scale = 2.0f;
    return Rectangle{ ship->position.x, ship->position.y, ship->texture.width* scale, ship->texture.height* scale };
}

MovementController::MovementController(float initialAltitude)
    : initialAltitude(initialAltitude) {}

void MovementController::UpdatePosition_1(Ship& ship1, float& altitudeCounter, float deltaTime)
{
    if (IsKeyDown(KEY_W) && altitudeCounter > 0.0f && ship1.fuel > 0) //Activate thrusters
    {
        thrustOn_1 = true;
        if (ship1.Thrust.y <= 20)
        {
            ship1.Thrust.y += ship1.Thrust_value;
        }

        ship1.fuel -= ship1.engine_FuelConsumption;
    }

    else // Removes upwards inertia after thruster's stop
    {
        thrustOn_1 = false;
        ship1.Thrust.y = 0.0f;

        if (ship1.v.y > 0)
        {
            ship1.v.y -= 0.5f;
        }
    }
}

void MovementController::UpdatePosition_Side_1(Ship& ship1, float deltaTime)
{
    if (IsKeyDown(KEY_A) && ship1.fuel > 0) //MOVING LEFT
    {
        if (ship1.Thrust.x > -7)
        {
            ship1.Thrust.x = -ship1.SideThrust_Value;
        }
    }

    else if (IsKeyDown(KEY_D) && ship1.fuel > 0) //MOVING RIGHT
    {
        if (ship1.Thrust.x < 7)
        {
            ship1.Thrust.x = ship1.SideThrust_Value;
        }
    }

    else
    {
        ship1.Thrust.x = 0.0f;
    }
}

void MovementController::UpdatePosition_2(Ship& ship2, float altitudeCounter2, float deltaTime)
{
    if (IsKeyDown(KEY_UP) && altitudeCounter2 > 0.0f && ship2.fuel > 0) //Activate thrusters
    {
        thrustOn_2 = true;
        if (ship2.Thrust.y <= 20)
        {
            ship2.Thrust.y += ship2.Thrust_value;
        }

        ship2.fuel -= ship2.engine_FuelConsumption;
    }

    else // Removes upwards inertia after thruster's stop
    {
        thrustOn_2 = false;
        ship2.Thrust.y = 0.0f;

        if (ship2.v.y > 0)
        {
            ship2.v.y -= 0.5f;
        }
    }
}

void MovementController::UpdatePosition_Side_2(Ship& ship2, float deltaTime)
{
    if (IsKeyDown(KEY_LEFT) && ship2.fuel > 0) //MOVING LEFT
    {
        if (ship2.Thrust.x > -7)
        {
            ship2.Thrust.x = -ship2.SideThrust_Value;
        }
    }

    else if (IsKeyDown(KEY_RIGHT) && ship2.fuel > 0) //MOVING RIGHT
    {
        if (ship2.Thrust.x < 7)
        {
            ship2.Thrust.x = ship2.SideThrust_Value;
        }
    }

    else
    {
        ship2.Thrust.x = 0.0f;
    }
}

Pid_Controller VerticalBoosterPID;

void MovementController::AutoLand_Vertical(Ship& ship, float deltaTime)
{
    thrustOn_1 = true;    

    float desiredLandingSpeed = 346 / 17.0f - ship.position.y / 24.0f;
    desiredLandingSpeed = std::min(-desiredLandingSpeed, -1.0f);
    VerticalBoosterPID.SetError(desiredLandingSpeed, ship.v.y);

    float output_P = VerticalBoosterPID.Get_P_output();
    float output_i = VerticalBoosterPID.Get_I_output(deltaTime);
    float output_d = VerticalBoosterPID.Get_D_output(deltaTime);
    float output = output_i + output_P + output_d;

    ship.Thrust.y = (output * ship.Thrust_value);
}