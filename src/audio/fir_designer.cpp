#include "plasma/audio/fir_designer.hpp"
#include "plasma/dsp/fir_designer.hpp"

namespace plasma::audio {

vector<double> fir_designer::create_kaiser_taps(double fc, double df, double att)
{
    return plasma::dsp::fir_designer::create_kaiser_taps(fc, df, att);
}

fir fir_designer::create_kaiser_filter(double fc, double df, double att)
{
    fir filter;
    filter.set_taps(create_kaiser_taps(fc, df, att));
    return filter;
}

fir_interpolator fir_designer::create_kaiser_interpolator(unsigned int k, double df, double att)
{
    fir_interpolator filter;
    filter.set_factor(k);
    filter.set_taps(create_kaiser_taps(0.5 / k, df, att));
    return filter;
}

fir_decimator fir_designer::create_kaiser_decimator(unsigned int k, double df, double att)
{
    fir_decimator filter;
    filter.set_factor(k);
    filter.set_taps(create_kaiser_taps(0.5 / k, df, att));
    return filter;
}

} // namespace
