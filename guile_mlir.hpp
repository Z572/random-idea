#pragma once
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_GUILE_MLIR
    #define GUILE_MLIR_PUBLIC __declspec(dllexport)
  #else
    #define GUILE_MLIR_PUBLIC __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_GUILE_MLIR
      #define GUILE_MLIR_PUBLIC __attribute__ ((visibility ("default")))
  #else
      #define GUILE_MLIR_PUBLIC
  #endif
#endif

namespace guile_mlir {

class GUILE_MLIR_PUBLIC Guile_mlir {

public:
  Guile_mlir();
  int get_number() const;

private:

  int number;

};

}

