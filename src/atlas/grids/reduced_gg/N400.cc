// TL799

#include "atlas/grids/reduced_gg/Grids.h"

namespace atlas {
namespace grids {
namespace reduced_gg {

N400::regist N400_builders;

void N400::construct()
{
  int N=400;
  int lon[] = {
    18,
    25,
    32,
    40,
    45,
    50,
    60,
    60,
    72,
    72,
    75,
    81,
    90,
    96,
   100,
   108,
   120,
   120,
   125,
   128,
   144,
   144,
   150,
   160,
   160,
   180,
   180,
   192,
   192,
   200,
   200,
   216,
   216,
   225,
   240,
   240,
   240,
   250,
   250,
   256,
   270,
   288,
   288,
   288,
   300,
   300,
   320,
   320,
   320,
   324,
   360,
   360,
   360,
   360,
   360,
   360,
   375,
   375,
   384,
   400,
   400,
   400,
   405,
   432,
   432,
   432,
   432,
   450,
   450,
   450,
   480,
   480,
   480,
   480,
   480,
   486,
   500,
   500,
   512,
   512,
   540,
   540,
   540,
   540,
   540,
   576,
   576,
   576,
   576,
   576,
   576,
   600,
   600,
   600,
   600,
   640,
   640,
   640,
   640,
   640,
   640,
   640,
   648,
   675,
   675,
   675,
   675,
   675,
   720,
   720,
   720,
   720,
   720,
   720,
   720,
   729,
   729,
   750,
   750,
   750,
   750,
   768,
   768,
   768,
   800,
   800,
   800,
   800,
   800,
   800,
   810,
   864,
   864,
   864,
   864,
   864,
   864,
   864,
   864,
   864,
   864,
   900,
   900,
   900,
   900,
   900,
   900,
   900,
   960,
   960,
   960,
   960,
   960,
   960,
   960,
   960,
   960,
   960,
   960,
   960,
   972,
   972,
  1000,
  1000,
  1000,
  1000,
  1000,
  1000,
  1024,
  1024,
  1024,
  1024,
  1024,
  1080,
  1080,
  1080,
  1080,
  1080,
  1080,
  1080,
  1080,
  1080,
  1080,
  1080,
  1125,
  1125,
  1125,
  1125,
  1125,
  1125,
  1125,
  1125,
  1125,
  1152,
  1152,
  1152,
  1152,
  1152,
  1152,
  1200,
  1200,
  1200,
  1200,
  1200,
  1200,
  1200,
  1200,
  1200,
  1200,
  1200,
  1215,
  1215,
  1215,
  1215,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1280,
  1296,
  1296,
  1296,
  1296,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1350,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1440,
  1458,
  1458,
  1458,
  1458,
  1458,
  1458,
  1458,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1500,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1536,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600,
  1600
  };
  std::vector<double> lat(N);
  eckit::Log::warning() << className() << " uses predicted gaussian latitudes";
  predict_gaussian_latitudes_hemisphere(N,lat.data());
  setup_lat_hemisphere(N,lon,lat.data(),DEG);
}

}
}
}
