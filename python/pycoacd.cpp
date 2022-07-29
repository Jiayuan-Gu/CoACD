#include <pybind11/pybind11.h>
#include <time.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "process.h"

using namespace std;
namespace py = pybind11;

int coacd(const string input_model, const string output_name,
          const string logfile, double threshold, unsigned int seed,
          bool preprocess, bool verbose) {
  Params params;

  // params.seed = (unsigned)time(NULL);
  params.seed = seed;
  params.threshold = threshold;
  params.preprocess = preprocess;

  params.input_model = input_model;
  params.output_name = output_name;
  params.logfile = logfile;

  string ext;
  if (params.input_model.length() > 4) {
    ext = params.input_model.substr(params.input_model.length() - 4);
    if (ext != ".obj") {
      cout << "Error: Input must be OBJ format!" << endl;
      return -1;
    }
  } else {
    cout << "Error: Input Filename Error!" << endl;
    return -1;
  }

  if (params.output_name.length() > 4)
    ext = params.output_name.substr(params.output_name.length() - 4);
  else {
    cout << "Error: Output Filename Error! You can set the output filename as "
            "either .OBJ or .WRL!"
         << endl;
    return -1;
  }
  if (params.logfile == "") {
    if (ext == ".obj")
      params.logfile =
          regex_replace(params.output_name, regex(".obj"), "_log.txt");
    else if (ext == ".wrl")
      params.logfile =
          regex_replace(params.output_name, regex(".wrl"), "_log.txt");
    else {
      cout << "Error: Output Filename must be .OBJ or .WRL format!" << endl;
      return -1;
    }
  }

  if (params.threshold < 0.01)
    cout << "Warning: Threshold t exceeds the lower bound and is automatically "
            "set as 0.01!"
         << endl;
  else if (params.threshold > 1)
    cout << "Warning: Threshold t exceeds the higher bound and is "
            "automatically set as 1!"
         << endl;
  params.threshold = min(max(params.threshold, 0.01), 1.0);

  // https://stackoverflow.com/questions/8246317/redirecting-function-output-to-dev-null
  // Backup streambuffers of cout
  streambuf* cout_oldbuf = cout.rdbuf();
  ofstream nf("/dev/null");
  if (!verbose) {
    cout.rdbuf(nf.rdbuf());  // Redirect cout to a buffer
  }

  Model m;

  ofstream of(params.logfile);

  SaveConfig(of, params);

  if (params.preprocess)
    ManifoldPreprocess(params, m, of);
  else
    m.LoadOBJ(params.input_model);
  if (params.pca) m.PCA();
  m.Normalize();

  auto n_convex = Compute(of, m, params);

  of.close();

  if (!verbose) {
    cout.rdbuf(cout_oldbuf);  // Redirect cout back to screen
  }
  nf.close();

  return n_convex;
}

PYBIND11_MODULE(pycoacd, m) {
  m.def("coacd", &coacd,
        R"doc(
Approximate Convex Decomposition for 3D Meshes with Collision-Aware Concavity and Tree Search.

Args:
  input: path for input mesh (.obj)
  output: path for output (.obj or .wrl)
  log: path for output logfile
  threshold: concavity threshold for terminating the decomposition (0.01~1)
  seed: random seed used for sampling
  preprocess: flag to enable manifold preprocessing. 
    If your input is already manifold mesh, disabling the preprocessing can avoid introducing extra artifacts.
  verbose: flag to enable console output.

References:
  https://github.com/SarahWeiii/CoACD
)doc",
        py::arg("input"), py::arg("output"), py::arg("log") = "",
        py::arg("threshold") = 0.05, py::arg("seed") = 0,
        py::arg("preprocess") = true, py::arg("verbose") = true);
}