#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "Gravity.h" // Assume this includes necessary physics calculations
#include "SideBoosters.h"
#include "UI.h"        // Include the Start Menu interface
#include "Obstacles.h" // Include obstacle collision handling
#include "PID.h"      // Include PID controller definitions
#include <thread>
#include <vector>

// Global variables - typically avoid these in headers; consider other design approaches
extern std::vector<const Ship*> Ships;
extern bool isGameOver_Fail;
extern bool isGameOVer_Success;

// Function prototypes
void CheckTouchDown(std::vector<const Ship*>& Ships);
void SuccessMessage();
void FailMessage();
void ShipDataInit(Ship& ship1, Ship& ship2);
void DisplayGameOver();

#endif // GAME_LOGIC_H