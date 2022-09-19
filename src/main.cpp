#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "core/foil.hpp"
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

	   XFoil
    )pbdoc";

	py::class_<XF>(m,"XFoil")
		.def(py::init<>())
		.def("load",&XF::LOAD)
		.def_property("Re",&XF::getRe,&XF::setRe)
		.def_property("Mach",&XF::getMach,&XF::setMach)
		.def("alfa",&XF::ALFA)
		.def("cl",&XF::CL);

	py::class_<Foil>(m,"Foil")
		.def(py::init<>())
		.def("load",&Foil::Load)
		.def("load_from_file",&Foil::Load_from_file)
		.def("camber",&Foil::camber)
		.def("xCamber",&Foil::xCamber)
		.def("thickness",&Foil::thickness)
		.def("xThickness",&Foil::xThickness)
		.def("x",&Foil::get_x)
		.def("y",&Foil::get_y)
		.def("camberline",&Foil::getCamberLine);

#ifdef VERSION_INFO
	m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
	m.attr("__version__") = "dev";
#endif
}
