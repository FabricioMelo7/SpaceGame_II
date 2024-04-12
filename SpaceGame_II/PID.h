#pragma once
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <vector>

class RingBuffer
{
private:
    std::vector<float> buffer;
    std::size_t head;
    const std::size_t capacity;

public:
    RingBuffer(std::size_t _size);
    void Push(float val);
    float Get(float deltaTime);
};

class Pid_Controller
{
private:
    const std::size_t integralBufferSize;
    RingBuffer IntegralRingBuffer;
    float currentError;
    float previousError;
    float error;

public:
    Pid_Controller();
    void SetError(float desiredValue, float measuredValue);
    float Get_P_output();
    float Get_I_output(float deltaTime);
    float Get_D_output(float deltaTime);

    static const float cp; // Proportional coefficient
    static const float ci; // Integral coefficient
    static const float cd; // Derivative coefficient
};

#endif // PID_CONTROLLER_H

