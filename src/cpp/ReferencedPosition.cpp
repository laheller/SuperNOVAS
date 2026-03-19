/**
 * @file
 *
 * @date Created  on Mar 19, 2026
 * @author Attila Kovacs
 */


/// \cond PRIVATE
#define __NOVAS_INTERNAL_API__      ///< Use definitions meant for internal use by SuperNOVAS only
/// \endcond

#include "supernovas.h"



namespace supernovas {

/**
 * Instantiates a referenced position from a position defined relatibve to a specified time and Solary-system
 * barycentric place (and optionally a barycentric velocity)
 *
 * @param p         Aposition vector defined relative to the reference place.
 * @param time      Astrometric time at which this position is defined.
 * @param ssb_pos   (optional) Solar-system barycentric position vector of the place relative to
 *                  which this position is defined (default: SSB).
 */
ReferencedPosition::ReferencedPosition(const Position& p, const Time& time, const Position& ssb_pos) :
        Position(p), _time(time), _origin(ssb_pos) {
  static const char *fn = "ReferencedPosition()";

  if(!is_valid())
     novas_trace_invalid(fn);

   if(!_time.is_valid()) {
     novas_set_errno(EINVAL, fn, "input time is invalid");
     _valid = false;
   }

   if(!_origin.is_valid()) {
     novas_set_errno(EINVAL, fn, "input reference position is invalid");
     _valid = false;
   }
}

/**
 * Returns the Solar system barycentric (SSB) position, relative to which this position is defined.
 *
 * @return    the Solar-system barycentric position that this position is referenced to.
 *
 */
const Position& ReferencedPosition::reference() const {
  return _origin;
}

/**
 * Returns the reference time at which this geometric position is given
 *
 * @return    the time at which this position is defined.
 */
const Time& ReferencedPosition::time() const {
  return _time;
}

/**
 * Returns the nomimnal equatorial place of this position, as would be seen by a stationary
 * (w.r.t. the SSB) observer located at the reference place.
 *
 * @param accuracy    NOVAS_FULL_ACCURACY (0) or NOVAS_REDUCED_ACCURACY (1).
 * @return            the true-of-date (TOD) equatorial place of this position at the time
 *                    light arrives at the Solar-system body or place that is at origin
 *                    of this position. Aberration corrections are applied if the
 *                    reference place is defined to be moving relative to the SSB.
 */
const Equatorial ReferencedPosition::equatorial(enum novas_accuracy accuracy) const {
  double ra = NAN, dec = NAN;
  vector2radec(_array(), &ra, &dec);

  Time tobs = _time + (distance().m() / Constant::c);
  return Equatorial(ra * Unit::hour_angle, dec * Unit::deg, Equinox::tod(tobs));
}

/**
 * Returns the same position but referenced to a new location (and motion) relative to the Solar-system
 * Barycenter.
 *
 * @param ref_pos   The new location, w.r.t. the SSB, relative to which to redefine this position.
 * @return          This position but referenced to the new location (and motion).
 *
 * @sa referenced_to_ssb()
 */
ReferencedPosition ReferencedPosition::referenced_to(const Position& ref_pos) const {
  return (*this + _origin - ref_pos).referenced_to(_time, ref_pos);
}

/**
 * Returns the same position but referenced the Solar-system Barycenter (SSB).
 *
 * @return      This position but referenced to the SSB.
 *
 * @sa referenced_to()
 */
ReferencedPosition ReferencedPosition::referenced_to_ssb() const {
  return (*this + _origin).referenced_to(_time, Position::origin());
}


/**
 * Returns a human-readable string representation of this referenced position.
 *
 * @param decimals    Number of decimal places to show for the position components.
 * @return            a string summary of this referenced position.
 */
std::string ReferencedPosition::to_string(int decimals) const {
  return Position::to_string(decimals) + " at " + _time.to_string() + " from " + _origin.to_string();
}


} // namespace supernovas
