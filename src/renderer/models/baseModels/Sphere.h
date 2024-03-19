#pragma once

#include "../Model.h"

class Sphere : public Model
{
private:
    void generateSphere(float radius, unsigned int segments);

public:
    Sphere(float radius, unsigned int segments);


    void Draw() const override;

};
