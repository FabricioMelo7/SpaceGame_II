#pragma once
#ifndef SIDE_PHYSICS_H
#define SIDE_PHYSICS_H

#include "ShipEntity.h"  
#include <cmath>              

Vector2 createVector2_Side(float x, float y);
Vector2 Euler_Side(Vector2 a, float h);
void SetForce_Side(Ship& ship);
void ApplyEuler_Side(Ship& ship, float deltaTime);
void ApplySideBoosters(Ship& ship, float dealtTime);

#endif // SIDE_PHYSICS_H

