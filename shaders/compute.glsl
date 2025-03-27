#version 330 core

uniform float particleSize;
uniform float particleMass;
uniform float gravity;
uniform float restDensity;
uniform float stiffness;
uniform float repulsionStrength;
uniform float attractionStrength;
uniform float smoothingLength;


void ComputeDensity()
{
    float h = smoothingLength;
    float h2 = h * h;
}

void ComputePressure()
{

}

void ComputePressureForces()
{

}
