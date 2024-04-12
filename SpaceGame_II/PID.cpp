#include "PID.h"

// Initialize static members
const float Pid_Controller::cp = -1.0f; // Proportional gain
const float Pid_Controller::ci = -25.0f; // Integral gain
const float Pid_Controller::cd = 0.3f; // Derivative gain

RingBuffer::RingBuffer(size_t _size)
    : buffer(_size, 0.0f), head(0), capacity(_size)
{
}

void RingBuffer::Push(float val)
{
    buffer[head] = val;
    head = (head + 1) % capacity;
}

float RingBuffer::Get(float deltaTime)
{
    float sumResult = 0.0f;
    for (size_t i = 0; i < capacity; i++)
    {
        sumResult += buffer[i] * deltaTime;
    }
    return sumResult;
}

Pid_Controller::Pid_Controller()
    : integralBufferSize(4), IntegralRingBuffer(integralBufferSize), currentError(0.0f), previousError(0.0f), error(0.0f)
{
}

void Pid_Controller::SetError(float desiredValue, float measuredValue)
{
    previousError = currentError;
    error = measuredValue - desiredValue;
    IntegralRingBuffer.Push(error);
    currentError = error;
}

float Pid_Controller::Get_P_output()
{
    return cp * error;
}

float Pid_Controller::Get_I_output(float deltaTime)
{
    return ci * IntegralRingBuffer.Get(deltaTime);
}

float Pid_Controller::Get_D_output(float deltaTime) 
{
    return cd * ((currentError - previousError) / deltaTime);
}
