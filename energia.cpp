#include "energia.hpp"
#include <iostream>
#include <string>
#include <cmath>

Energia::Energia(int wh) 
    : wh_{wh} {}
    
int Energia::emWh() const { return wh_; }

bool Energia::operator==(Energia outra) const {
    return wh_ == outra.wh_;
}

bool Energia::operator!=(Energia outra) const {
    return wh_ != outra.wh_;
}

bool Energia::operator<(Energia outra) const {
    return wh_ < outra.wh_;
}

Energia& Energia::operator+=(Energia outra) {
    if (outra.wh_ < 0) {
        return *this;
    }
    wh_ += outra.wh_;
    return *this;
}