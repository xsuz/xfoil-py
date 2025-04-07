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

		XFoilTask
    )pbdoc";

	py::class_<XFoilTask>(m,"XFoilTask")
		.def(py::init<>())
		.def("load",&XFoilTask::Load)
		.def("calc",&XFoilTask::calc)
		.def("cpv",&XFoilTask::cpv)
		.def("iter",&XFoilTask::iter)
		.def("tegap",&XFoilTask::tegap)
		.def("interpolate",&XFoilTask::interpolate)
		.def("getX",&XFoilTask::getX)
		.def("getY",&XFoilTask::getY)
		.def("save",&XFoilTask::save)
		.def("setDebug",&XFoilTask::setDebug);

#ifdef VERSION_INFO
	m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
	m.attr("__version__") = "dev";
#endif
}
