/****************************************************************************
*
* This is a part of CTPPS offline software.
* Authors:
*   Laurent Forthomme (laurent.forthomme@cern.ch)
*   Nicola Minafra (nicola.minafra@cern.ch)
*
****************************************************************************/

#ifndef DataFormats_CTPPSReco_CTPPSDiamondRecHit
#define DataFormats_CTPPSReco_CTPPSDiamondRecHit

#include "DataFormats/CTPPSDigi/interface/HPTDCErrorFlags.h"

/// Reconstructed hit in diamond detectors.
class CTPPSDiamondRecHit
{
  public:
    CTPPSDiamondRecHit() :
      x_( 0. ), x_width_( 0. ), y_( 0. ), y_width_( 0. ), z_( 0. ), z_width_( 0. ),
      t_( 0. ), tot_( 0. ), t_precision_( 0. ),
      ts_index_( 0 ), hptdc_err_( 0 ), mh_( false )
    {}
    CTPPSDiamondRecHit( float x, float x_width, float y, float y_width, float z, float z_width, float t, float tot, float t_precision, int oot_idx, const HPTDCErrorFlags& hptdc_err, const bool mh ) :
      x_( x ), x_width_( x_width ), y_( y ), y_width_( y_width ), z_( z ), z_width_( z_width ),
      t_( t ), tot_( tot ), t_precision_( t_precision ),
      ts_index_( oot_idx ), hptdc_err_( hptdc_err ), mh_( mh )
    {}

    static constexpr int TIMESLICE_WITHOUT_LEADING = -10;

    inline void setX( const float& x ) { x_ = x; }
    inline float getX() const { return x_; }

    inline void setY( const float& y ) { y_ = y; }
    inline float getY() const { return y_; }

    inline void setZ( const float& z ) { z_ = z; }
    inline float getZ() const { return z_; }

    inline void setXWidth( const float& xwidth ) { x_width_ = xwidth; }
    inline float getXWidth() const { return x_width_; }

    inline void setYWidth( const float& ywidth ) { y_width_ = ywidth; }
    inline float getYWidth() const { return y_width_; }

    inline void setZWidth( const float& zwidth ) { z_width_ = zwidth; }
    inline float getZWidth() const { return z_width_; }

    inline void setT( const float& t ) { t_ = t; }
    inline float getT() const { return t_; }

    inline void setToT( const float& tot ) { tot_ = tot;  }
    inline float getToT() const { return tot_; }

    inline void setTPrecision( const float& t_precision ) { t_precision_ = t_precision;  }
    inline float getTPrecision() const { return t_precision_; }

    inline void setOOTIndex( const int& i ) { ts_index_ = i; }
    inline int getOOTIndex() const { return ts_index_; }

    inline void setMultipleHits( const bool mh ) { mh_ = mh; }
    inline bool getMultipleHits() const { return mh_; }

    inline void setHPTDCErrorFlags( const HPTDCErrorFlags& err ) { hptdc_err_ = err; }
    inline HPTDCErrorFlags getHPTDCErrorFlags() const { return hptdc_err_; }

  private:
    float x_, x_width_;
    float y_, y_width_;
    float z_, z_width_;
    float t_, tot_, t_precision_;
    /// Time slice index
    int ts_index_;
    HPTDCErrorFlags hptdc_err_;
    bool mh_;
};

//----------------------------------------------------------------------------------------------------

inline bool operator<( const CTPPSDiamondRecHit& l, const CTPPSDiamondRecHit& r )
{
  // only sort by leading edge time
  return ( l.getT() < r.getT() );
}

#endif

