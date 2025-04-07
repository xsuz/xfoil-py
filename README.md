# xfoil-py

---

This is a package that enable us to use XFOIL with Python.

[![](https://xsuz.github.io/post/xfoil-with-python/img/xfoil_logo.gif)](https://web.mit.edu/drela/Public/web/xfoil/)

Unlike xfoil, this module works cross-platform.


# Getting Started

Just run this to install xfoil-py:

```shell
$ git clone https://github.com/xsuz/xfoil-py
$ cd xfoil-py
$ pip install .
```

# Example

```python
from xfoil import XFoilTask
f=XFoilTask() # create instance
f.load("DAE41.dat",True) # Load primary foil
f.calc(5,5e+5) # calculate cl,cd,cm and xcp from angle of attack(deg) and reynolds number
f.load("DAE51.dat",False) # Load secondary foil
f.interpolate(0.7) # interpolate two foil : dae41/dae51 = 0.7/0.3
f.cpv(5,5e+5) # calculate Cpv from angle of attack(deg) and reynolds number
f.tegap(0.12) # set trailing edge gap
f.save("tegap.dat") # save foil as "tegap.dat"
x=f.getX() # get position of each point of foil
y=f.getY() # get position of each point of foil
```
