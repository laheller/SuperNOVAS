/**
 * @file
 *
 * @date Created  on Oct 13, 2025
 * @author Attila Kovacs
 */

#include <iostream>

#include "TestUtil.hpp"

int main() {
  TestUtil test = TestUtil("ReferencedPosition");

  int n = 0;


  if(!test.check("is_valid(pos invalid)", !ReferencedPosition(Position::undefined(), Time::j2000()).is_valid())) n++;
  if(!test.check("is_valid(time invalid)", !ReferencedPosition(Position::origin(), Time::undefined()).is_valid())) n++;
  if(!test.check("is_valid(time invalid)", !ReferencedPosition(Position::origin(), Time::j2000(), Position::undefined()).is_valid())) n++;

  Position p = Position(-1.123456789 * Unit::au, 2.123456789 * Unit::au, -3.123456789 * Unit::au);

  ReferencedPosition a(p, Time::b1950());
  if(!test.check("is_valid()", a.is_valid())) n++;
  if(!test.check(" == position", a == p)) n++;
  if(!test.check("time()", a.time() == Time::b1950())) n++;
  if(!test.check("origin(0)", a.reference() == Position::origin())) n++;

  Position r = Position(1.3 * Unit::AU, -2.2 * Unit::AU, 3.1 * Unit::AU);
  ReferencedPosition b(p, Time::b1950(), r);
  if(!test.check("origin()", b.reference() == r)) n++;

  if(!test.check("referenced_to()", a.referenced_to(r) == (p - r))) n++;
  if(!test.check("referenced_to().origin", b.referenced_to(r).reference() == r)) n++;

  if(!test.check("referenced_to_ssb()", b.referenced_to_ssb() == (p + r))) n++;
  if(!test.check("referenced_to_ssb().origin", b.referenced_to_ssb().reference() == Position::origin())) n++;

  double ra = 0.0, dec = 0.0;
  vector2radec(a._array(), &ra, &dec);
  if(!test.check("equatorial()", a.equatorial().is_valid())) n++;
  if(!test.equals("equatorial().ra()", a.equatorial().ra().hours(), ra, 1e-13)) n++;
  if(!test.equals("equatorial().dec()", a.equatorial().dec().deg(), dec, 1e-12)) n++;


  if(!test.equals("to_string()", a.to_string(),
          "Position (-1.123 AU, 2.123 AU, -3.123 AU) at 1949-12-31T22:09:14.678 UTC from Position (0.000 m, 0.000 m, 0.000 m)")) n++;

  std::cout << "Vector.cpp: " << (n > 0 ? "FAILED" : "OK") << "\n";
  return n;
}
