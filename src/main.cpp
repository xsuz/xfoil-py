#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "xf.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

using namespace std;
namespace py = pybind11;

PYBIND11_MODULE(xfoil, m) {
	m.doc() = R"pbdoc(
        wrapper of xfoil (from xflr5 v6.57)
        -----------------------

        .. currentmodule:: xfoil

        .. autosummary::
		:toctree: _generate

		XF
    )pbdoc";

	py::class_<XF>(m,"xf")
		.def(py::init<>())
		.def("load",&XF::Load)
		.def("calc",&XF::calc)
		.def("cpv",&XF::cpv)
		.def("iter",&XF::iter)
		.def("tegap",&XF::tegap)
		.def("interpolate",&XF::interpolate)
		.def("getX",&XF::getX)
		.def("getY",&XF::getY)
		.def("save",&XF::save);

#ifdef VERSION_INFO
	m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
	m.attr("__version__") = "dev";
#endif
}
