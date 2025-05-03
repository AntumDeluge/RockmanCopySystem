#ifndef FIXEDPOINT8
#define FIXEDPOINT8

class FixedPoint8 {
  private:
  int m_value;

  public:
  inline FixedPoint8(int a_integer, int a_fraction) {
    m_value = a_integer << 8;
    m_value = m_value | a_fraction;
  };
  inline FixedPoint8(int a_value) {
    m_value = a_value << 8;
  };
  inline operator int() {
    return m_value >> 8;
  };

  // operator +
  inline FixedPoint8 operator+(const FixedPoint8& a_fixedPoint8) {
    FixedPoint8 fixedPoint8(0);
    fixedPoint8.m_value = m_value + a_fixedPoint8.m_value;
    return fixedPoint8;
  };
  inline FixedPoint8 operator+(const int& a_int) {
    FixedPoint8 fixedPoint8(0);
    fixedPoint8.m_value = m_value + (a_int << 8);
    return fixedPoint8;
  };
  inline void operator+=(const FixedPoint8& a_fixedPoint8) {
    m_value += a_fixedPoint8.m_value;
  };
  inline void operator+=(const int& a_int) {
    m_value += (a_int << 8);
  };

  // operator -
  inline FixedPoint8 operator-(const FixedPoint8& a_fixedPoint8) {
    FixedPoint8 fixedPoint8(0);
    fixedPoint8.m_value = m_value - a_fixedPoint8.m_value;
    return fixedPoint8;
  };
  inline FixedPoint8 operator-(const int& a_int) {
    FixedPoint8 fixedPoint8(0);
    fixedPoint8.m_value = m_value - (a_int << 8);
    return fixedPoint8;
  };
  inline void operator-=(const FixedPoint8& a_fixedPoint8) {
    m_value -= a_fixedPoint8.m_value;
  };
  inline void operator-=(const int& a_int) {
    m_value -= (a_int << 8);
  };

  // operator =
  inline void operator=(const FixedPoint8& a_fixedPoint8) {
    m_value = a_fixedPoint8.m_value;
  };
  inline void operator=(const int& a_int) {
    m_value = a_int << 8;
  };

  // operator ==
  inline bool operator==(const FixedPoint8& a_fixedPoint8) {
    return m_value == a_fixedPoint8.m_value;
  };
  inline bool operator==(const int& a_int) {
    return (m_value >> 8) == a_int;
  };

  // operator !=
  inline bool operator!=(const FixedPoint8& a_fixedPoint8) {
    return m_value != a_fixedPoint8.m_value;
  };
  inline bool operator!=(const int& a_int) {
    return (m_value >> 8) != a_int;
  };

  // operator <
  inline bool operator<(const FixedPoint8& a_fixedPoint8) {
    return m_value < a_fixedPoint8.m_value;
  };
  inline bool operator<(const int& a_int) {
    return (m_value >> 8) < a_int;
  };

  // operator >
  inline bool operator>(const FixedPoint8& a_fixedPoint8) {
    return m_value > a_fixedPoint8.m_value;
  };
  inline bool operator>(const int& a_int) {
    return (m_value >> 8) > a_int;
  };

  // debug
  inline void print() {
    printf("%d,%d (%d)", m_value >> 8, m_value & 0x00FF, m_value);
  };
  inline int get() {
    return m_value;
  };
};

#endif
