#ifndef LABA3_RATIONAL_H
#define LABA3_RATIONAL_H

class Rational {
public:
    Rational(int nominator = 0, unsigned int denominator = 1) : nominator_(nominator), denominator_(denominator) {}

    bool operator ==(Rational const& other) const {
        return (nominator_ * other.denominator_ == other.nominator_ * denominator_);
    }
    bool operator !=(Rational const& other) const {
        return !(*this == other);
    }
    bool operator <(Rational const& other) const {
        return (nominator_ * other.denominator_ < other.nominator_ * denominator_);
    }
    bool operator >(Rational const& other) const {
        return other < *this;
    }
    bool operator <=(Rational const& other) const {
        return !(*this > other);
    }
    bool operator >=(Rational const& other) const {
        return !(*this < other);
    }

private:
    int nominator_;
    unsigned int denominator_;
};

#endif
