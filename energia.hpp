#ifndef ENERGIA_HPP
#define ENERGIA_HPP

#include <string>

class Energia {
public:
    explicit Energia(int wh = 0);
    int emWh() const;
    bool operator==(Energia outra) const;
    bool operator!=(Energia outra) const;
    bool operator<(Energia outra) const;
    Energia& operator+=(Energia outra);
private:
    int wh_;
};

#endif
